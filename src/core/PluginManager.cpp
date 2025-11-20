#include "PluginManager.h"
#include "DebugManager.h"
#include "ErrorManager.h"
#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QProcess>
#include <QtConcurrent>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include <QMutexLocker>

// PluginManager implementation
PluginManager& PluginManager::instance() {
    static PluginManager instance;
    return instance;
}

PluginManager::PluginManager()
    : QObject(nullptr)
    , m_pluginDirectory(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/plugins")
    , m_autoLoadEnabled(true)
{
    DEBUG_TRACE("PluginManager", "Initializing plugin manager");
    
    // Create plugin directory if it doesn't exist
    QDir().mkpath(m_pluginDirectory);
    
    // Auto-discover plugins if enabled
    if (m_autoLoadEnabled) {
        QtConcurrent::run([this]() {
            QMutexLocker locker(&m_mutex);
            scanPluginDirectory();
        });
    }
    
    DEBUG_INFO("PluginManager", QString("Plugin manager initialized with directory: %1").arg(m_pluginDirectory));
}

QStringList PluginManager::discoverPlugins() {
    QMutexLocker locker(&m_mutex);
    DEBUG_TRACE("PluginManager", "Discovering plugins");
    
    QStringList discovered;
    QStringList pluginDirs = {
        m_pluginDirectory,
        QCoreApplication::applicationDirPath() + "/plugins",
        QStandardPaths::locate(QStandardPaths::AppDataLocation, QString(), QStandardPaths::LocateDirectory) + "/vibe-sync/plugins"
    };
    
    for (const QString& dir : pluginDirs) {
        if (QDir(dir).exists()) {
            QStringList found = scanDirectoryForPlugins(dir);
            for (const QString& plugin : found) {
                if (!m_plugins.contains(plugin)) {
                    discovered << plugin;
                }
            }
        }
    }
    
    DEBUG_INFO("PluginManager", QString("Discovered %1 new plugins").arg(discovered.size()));
    return discovered;
}

bool PluginManager::loadPlugin(const QString& pluginId) {
    QMutexLocker locker(&m_mutex);
    DEBUG_TRACE("PluginManager", QString("Loading plugin: %1").arg(pluginId));
    
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        ERROR_WARNING("PluginManager", QString("Plugin not found: %1").arg(pluginId));
        return false;
    }
    
    PluginInfo& info = it.value();
    
    if (info.state != PluginState::Unloaded && info.state != PluginState::Error) {
        DEBUG_DEBUG("PluginManager", QString("Plugin %1 already loaded (state: %2)").arg(pluginId).arg(static_cast<int>(info.state)));
        return info.state == PluginState::Loaded || info.state == PluginState::Active;
    }
    
    info.state = PluginState::Loading;
    info.error.clear();
    
    // Load the plugin using QPluginLoader
    info.loader = std::make_unique<QPluginLoader>(info.filePath);
    
    if (!info.loader->load()) {
        info.state = PluginState::Error;
        info.error = info.loader->errorString();
        ERROR_ERROR("PluginManager", QString("Failed to load plugin %1: %2").arg(pluginId).arg(info.error));
        emit pluginError(pluginId, info.error);
        return false;
    }
    
    // Get the plugin instance
    QObject* pluginObject = info.loader->instance();
    if (!pluginObject) {
        info.state = PluginState::Error;
        info.error = "Failed to get plugin instance";
        ERROR_ERROR("PluginManager", QString("Failed to get instance for plugin %1").arg(pluginId));
        emit pluginError(pluginId, info.error);
        return false;
    }
    
    // Cast to IPlugin interface
    info.interface = qobject_cast<IPlugin*>(pluginObject);
    if (!info.interface) {
        info.state = PluginState::Error;
        info.error = "Plugin does not implement IPlugin interface";
        ERROR_ERROR("PluginManager", QString("Plugin %1 does not implement IPlugin").arg(pluginId));
        emit pluginError(pluginId, info.error);
        info.loader->unload();
        return false;
    }
    
    // Initialize the plugin
    if (!info.interface->initialize(info.config)) {
        info.state = PluginState::Error;
        info.error = info.interface->lastError();
        ERROR_ERROR("PluginManager", QString("Failed to initialize plugin %1: %2").arg(pluginId).arg(info.error));
        emit pluginError(pluginId, info.error);
        info.loader->unload();
        return false;
    }
    
    info.state = PluginState::Loaded;
    DEBUG_INFO("PluginManager", QString("Successfully loaded plugin: %1 v%2").arg(pluginId).arg(info.version));
    
    locker.unlock();
    emit pluginLoaded(pluginId);
    
    return true;
}

bool PluginManager::unloadPlugin(const QString& pluginId) {
    QMutexLocker locker(&m_mutex);
    DEBUG_TRACE("PluginManager", QString("Unloading plugin: %1").arg(pluginId));
    
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        ERROR_WARNING("PluginManager", QString("Plugin not found for unloading: %1").arg(pluginId));
        return false;
    }
    
    PluginInfo& info = it.value();
    
    if (info.state == PluginState::Unloaded) {
        DEBUG_DEBUG("PluginManager", QString("Plugin %1 is already unloaded").arg(pluginId));
        return true;
    }
    
    // Shutdown the plugin
    if (info.interface) {
        info.interface->shutdown();
    }
    
    // Unload the library
    if (info.loader) {
        info.loader->unload();
    }
    
    info.state = PluginState::Unloaded;
    info.interface = nullptr;
    info.loader.reset();
    
    DEBUG_INFO("PluginManager", QString("Successfully unloaded plugin: %1").arg(pluginId));
    
    locker.unlock();
    emit pluginUnloaded(pluginId);
    
    return true;
}

bool PluginManager::reloadPlugin(const QString& pluginId) {
    DEBUG_TRACE("PluginManager", QString("Reloading plugin: %1").arg(pluginId));
    
    if (!unloadPlugin(pluginId)) {
        return false;
    }
    
    // Re-scan to update plugin info
    QMutexLocker locker(&m_mutex);
    scanPluginDirectory();
    
    return loadPlugin(pluginId);
}

QStringList PluginManager::loadedPlugins() const {
    QMutexLocker locker(&m_mutex);
    QStringList loaded;
    
    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
        if (it.value().state == PluginState::Loaded || it.value().state == PluginState::Active) {
            loaded << it.key();
        }
    }
    
    return loaded;
}

QStringList PluginManager::availablePlugins() const {
    QMutexLocker locker(&m_mutex);
    return QStringList(m_plugins.keys());
}

QStringList PluginManager::pluginsByType(PluginType type) const {
    QMutexLocker locker(&m_mutex);
    QStringList plugins;
    
    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
        if (it.value().type == type) {
            plugins << it.key();
        }
    }
    
    return plugins;
}

IPlugin* PluginManager::getPlugin(const QString& pluginId) const {
    QMutexLocker locker(&m_mutex);
    
    auto it = m_plugins.find(pluginId);
    if (it != m_plugins.end() && it.value().interface) {
        return it.value().interface;
    }
    
    return nullptr;
}

PluginState PluginManager::getPluginState(const QString& pluginId) const {
    QMutexLocker locker(&m_mutex);
    
    auto it = m_plugins.find(pluginId);
    if (it != m_plugins.end()) {
        return it.value().state;
    }
    
    return PluginState::Unloaded;
}

QJsonObject PluginManager::getPluginConfig(const QString& pluginId) const {
    QMutexLocker locker(&m_mutex);
    
    auto it = m_plugins.find(pluginId);
    if (it != m_plugins.end()) {
        return it.value().config;
    }
    
    return QJsonObject();
}

bool PluginManager::setPluginConfig(const QString& pluginId, const QJsonObject& config) {
    QMutexLocker locker(&m_mutex);
    
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        return false;
    }
    
    it.value().config = config;
    
    // Update running plugin if loaded
    if (it.value().interface && (it.value().state == PluginState::Loaded || it.value().state == PluginState::Active)) {
        locker.unlock();
        return it.value().interface->setConfig(config);
    }
    
    return true;
}

bool PluginManager::enablePlugin(const QString& pluginId) {
    DEBUG_TRACE("PluginManager", QString("Enabling plugin: %1").arg(pluginId));
    
    IPlugin* plugin = getPlugin(pluginId);
    if (!plugin) {
        ERROR_WARNING("PluginManager", QString("Plugin not found for enabling: %1").arg(pluginId));
        return false;
    }
    
    if (plugin->state() == PluginState::Error) {
        ERROR_WARNING("PluginManager", QString("Cannot enable plugin %1 in error state").arg(pluginId));
        return false;
    }
    
    QMutexLocker locker(&m_mutex);
    auto it = m_plugins.find(pluginId);
    if (it != m_plugins.end()) {
        it.value().state = PluginState::Active;
    }
    
    DEBUG_INFO("PluginManager", QString("Plugin enabled: %1").arg(pluginId));
    return true;
}

bool PluginManager::disablePlugin(const QString& pluginId) {
    DEBUG_TRACE("PluginManager", QString("Disabling plugin: %1").arg(pluginId));
    
    IPlugin* plugin = getPlugin(pluginId);
    if (!plugin) {
        ERROR_WARNING("PluginManager", QString("Plugin not found for disabling: %1").arg(pluginId));
        return false;
    }
    
    QMutexLocker locker(&m_mutex);
    auto it = m_plugins.find(pluginId);
    if (it != m_plugins.end()) {
        it.value().state = PluginState::Loaded;
    }
    
    DEBUG_INFO("PluginManager", QString("Plugin disabled: %1").arg(pluginId));
    return true;
}

bool PluginManager::executePluginAction(const QString& pluginId, const QString& action, const QJsonObject& params) {
    DEBUG_TRACE("PluginManager", QString("Executing plugin action %1 on %2").arg(action).arg(pluginId));
    
    IPlugin* plugin = getPlugin(pluginId);
    if (!plugin) {
        ERROR_WARNING("PluginManager", QString("Plugin not found for action execution: %1").arg(pluginId));
        return false;
    }
    
    if (plugin->state() != PluginState::Active && plugin->state() != PluginState::Loaded) {
        ERROR_WARNING("PluginManager", QString("Plugin %1 not in executable state").arg(pluginId));
        return false;
    }
    
    // For now, this is a placeholder for future action execution system
    // In a real implementation, this would call specific plugin methods
    DEBUG_INFO("PluginManager", QString("Plugin action executed: %1 on %2").arg(action).arg(pluginId));
    return true;
}

void PluginManager::broadcastEvent(const QString& eventName, const QJsonObject& data) {
    DEBUG_TRACE("PluginManager", QString("Broadcasting event: %1").arg(eventName));
    
    // Trigger the event through the event system
    PluginEventSystem::instance().triggerEvent(eventName, data);
    
    // Emit signal for UI components
    emit pluginEvent(eventName, data);
}

void PluginManager::sendMessageToPlugin(const QString& pluginId, const QString& message, const QJsonObject& data) {
    DEBUG_TRACE("PluginManager", QString("Sending message to plugin %1: %2").arg(pluginId).arg(message));
    
    emit pluginMessage(pluginId, message, data);
}

void PluginManager::setPluginDirectory(const QString& directory) {
    QMutexLocker locker(&m_mutex);
    m_pluginDirectory = directory;
    
    // Re-scan the new directory
    scanPluginDirectory();
    
    DEBUG_INFO("PluginManager", QString("Plugin directory changed to: %1").arg(directory));
}

void PluginManager::scanPluginDirectory() {
    QStringList directories = {
        m_pluginDirectory,
        QCoreApplication::applicationDirPath() + "/plugins"
    };
    
    for (const QString& dir : directories) {
        if (QDir(dir).exists()) {
            QStringList plugins = scanDirectoryForPlugins(dir);
            for (const QString& plugin : plugins) {
                if (!m_plugins.contains(plugin)) {
                    // Plugin will be discovered during next scan
                }
            }
        }
    }
}

QStringList PluginManager::scanDirectoryForPlugins(const QString& directory) {
    QStringList plugins;
    QDir dir(directory);
    
    // Look for dynamic libraries
    QStringList filters;
#ifdef Q_OS_WINDOWS
    filters << "*.dll";
#elif defined(Q_OS_MAC)
    filters << "*.dylib" << "*.framework";
#else
    filters << "*.so";
#endif
    
    QStringList files = dir.entryList(filters, QDir::Files);
    
    for (const QString& file : files) {
        QString pluginId = generatePluginId(directory + "/" + file);
        plugins << pluginId;
        
        // Store basic plugin info
        PluginInfo info;
        info.filePath = directory + "/" + file;
        info.name = QFileInfo(file).baseName();
        info.version = "1.0.0"; // Default version
        info.type = PluginType::Utility; // Default type
        info.state = PluginState::Unloaded;
        info.config = QJsonObject();
        
        m_plugins[pluginId] = info;
    }
    
    return plugins;
}

QString PluginManager::generatePluginId(const QString& filePath) const {
    QFileInfo info(filePath);
    return QString("%1_%2").arg(info.baseName()).arg(info.lastModified().toSecsSinceEpoch());
}

// PluginDeveloper implementation
QString PluginDeveloper::generatePluginTemplate(const QString& name, PluginType type) {
    DEBUG_TRACE("PluginDeveloper", QString("Generating template for plugin: %1").arg(name));
    
    QString templateContent = generatePluginHeader(name, type) + "\n\n" + 
                             generatePluginImplementation(name, type);
    
    return templateContent;
}

QString PluginDeveloper::generatePluginHeader(const QString& name, PluginType type) {
    QString className = name + "Plugin";
    
    QString header = QString(
        "#pragma once\n"
        "#include \"PluginManager.h\"\n"
        "#include <QObject>\n"
        "#include <QJsonObject>\n"
        "\n"
        "class %1 : public QObject, public IPlugin {\n"
        "    Q_OBJECT\n"
        "    Q_INTERFACES(IPlugin)\n"
        "\n"
    ).arg(className);
    
    switch (type) {
        case PluginType::Visualizer:
            header += "    Q_INTERFACES(IVisualizerPlugin)\n";
            break;
        case PluginType::AudioEffect:
            header += "    Q_INTERFACES(IAudioEffectPlugin)\n";
            break;
        case PluginType::Output:
            header += "    Q_INTERFACES(IOutputPlugin)\n";
            break;
        case PluginType::Remote:
            header += "    Q_INTERFACES(IRemoteControlPlugin)\n";
            break;
        default:
            break;
    }
    
    header += QString(
        "\n"
        "    Q_PLUGIN_METADATA(IID \"%2\" FILE \"%3.json\")\n"
        "\n"
        "public:\n"
        "    %1();\n"
        "    ~%1() override;\n"
        "\n"
        "    // IPlugin interface\n"
    ).arg(className).arg(className.toLower()).arg(name);
    
    // Add interface methods based on plugin type
    if (type == PluginType::Visualizer) {
        header += QString(
            "    QStringList presetFormats() const override;\n"
            "    bool canLoadPreset(const QString& filePath) const override;\n"
            "    QByteArray processVisualization(const QByteArray& audioData, const QSize& targetSize) override;\n"
        );
    } else if (type == PluginType::AudioEffect) {
        header += QString(
            "    QStringList supportedFormats() const override;\n"
            "    QByteArray processAudio(const QByteArray& audioData, int sampleRate, int channels) override;\n"
        );
    } else if (type == PluginType::Output) {
        header += QString(
            "    QStringList outputFormats() const override;\n"
            "    bool canOutputTo(const QString& format, const QString& destination) const override;\n"
            "    bool startOutput(const QString& format, const QJsonObject& options) override;\n"
            "    bool writeData(const QByteArray& data) override;\n"
            "    bool stopOutput() override;\n"
        );
    } else if (type == PluginType::Remote) {
        header += QString(
            "    quint16 defaultPort() const override;\n"
            "    QStringList supportedCommands() const override;\n"
            "    bool handleCommand(const QString& command, const QJsonObject& params, QJsonObject& response) override;\n"
        );
    }
    
    header += "\nprivate:\n";
    header += "    void initializePlugin();\n";
    header += "};\n";
    
    return header;
}

QString PluginDeveloper::generatePluginImplementation(const QString& name, PluginType type) {
    QString className = name + "Plugin";
    
    QString impl = QString(
        "#include \"%1.h\"\n"
        "\n"
        "%2::%2()\n"
        "{\n"
        "    initializePlugin();\n"
        "}\n"
        "\n"
        "%2::~%2()\n"
        "{\n"
        "    shutdown();\n"
        "}\n"
        "\n"
    ).arg(name).arg(className);
    
    // Add method implementations
    impl += QString(
        "void %2::initializePlugin()\n"
        "{\n"
        "    // Plugin-specific initialization\n"
        "}\n"
        "\n"
        "// IPlugin interface implementations\n"
        "QString %2::name() const\n"
        "{\n"
        "    return \"%1\";\n"
        "}\n"
        "\n"
        "QString %2::version() const\n"
        "{\n"
        "    return \"1.0.0\";\n"
        "}\n"
        "\n"
        "QString %2::description() const\n"
        "{\n"
        "    return \"%3 plugin for VibeSync\";\n"
        "}\n"
        "\n"
        "QString %2::author() const\n"
        "{\n"
        "    return \"Developer\";\n"
        "}\n"
        "\n"
        "PluginType %2::type() const\n"
        "{\n"
        "    return PluginType::%4;\n"
        "}\n"
        "\n"
        "QStringList %2::dependencies() const\n"
        "{\n"
        "    return QStringList();\n"
        "}\n"
        "\n"
        "bool %2::initialize(const QJsonObject& config)\n"
        "{\n"
        "    Q_UNUSED(config);\n"
        "    return true;\n"
        "}\n"
        "\n"
        "void %2::shutdown()\n"
        "{\n"
        "    // Cleanup plugin resources\n"
        "}\n"
        "\n"
        "PluginState %2::state() const\n"
        "{\n"
        "    return PluginState::Loaded;\n"
        "}\n"
        "\n"
        "QString %2::lastError() const\n"
        "{\n"
        "    return QString();\n"
        "}\n"
        "\n"
        "QJsonObject %2::defaultConfig() const\n"
        "{\n"
        "    return QJsonObject();\n"
        "}\n"
        "\n"
        "QJsonObject %2::currentConfig() const\n"
        "{\n"
        "    return QJsonObject();\n"
        "}\n"
        "\n"
        "bool %2::setConfig(const QJsonObject& config)\n"
        "{\n"
        "    Q_UNUSED(config);\n"
        "    return true;\n"
        "}\n"
        "\n"
        "QStringList %2::capabilities() const\n"
        "{\n"
        "    return QStringList();\n"
        "}\n"
        "\n"
        "bool %2::hasCapability(const QString& capability) const\n"
        "{\n"
        "    Q_UNUSED(capability);\n"
        "    return false;\n"
        "}\n"
    ).arg(name).arg(className).arg(name).arg(PluginTypeToString(type));
    
    // Add type-specific implementations
    if (type == PluginType::Visualizer) {
        impl += QString(
            "\n"
            "// IVisualizerPlugin interface implementations\n"
            "QStringList %1::presetFormats() const\n"
            "{\n"
            "    return QStringList() << \"milkdrop\" << \"projectm\";\n"
            "}\n"
            "\n"
            "bool %1::canLoadPreset(const QString& filePath) const\n"
            "{\n"
            "    Q_UNUSED(filePath);\n"
            "    return true;\n"
            "}\n"
            "\n"
            "QByteArray %1::processVisualization(const QByteArray& audioData, const QSize& targetSize)\n"
            "{\n"
            "    Q_UNUSED(audioData);\n"
            "    Q_UNUSED(targetSize);\n"
            "    return QByteArray();\n"
            "}\n"
        ).arg(className);
    } else if (type == PluginType::AudioEffect) {
        impl += QString(
            "\n"
            "// IAudioEffectPlugin interface implementations\n"
            "QStringList %1::supportedFormats() const\n"
            "{\n"
            "    return QStringList() << \"wav\" << \"mp3\" << \"flac\";\n"
            "}\n"
            "\n"
            "QByteArray %1::processAudio(const QByteArray& audioData, int sampleRate, int channels)\n"
            "{\n"
            "    Q_UNUSED(audioData);\n"
            "    Q_UNUSED(sampleRate);\n"
            "    Q_UNUSED(channels);\n"
            "    return QByteArray();\n"
            "}\n"
        ).arg(className);
    } else if (type == PluginType::Output) {
        impl += QString(
            "\n"
            "// IOutputPlugin interface implementations\n"
            "QStringList %1::outputFormats() const\n"
            "{\n"
            "    return QStringList() << \"file\";\n"
            "}\n"
            "\n"
            "bool %1::canOutputTo(const QString& format, const QString& destination) const\n"
            "{\n"
            "    Q_UNUSED(format);\n"
            "    Q_UNUSED(destination);\n"
            "    return true;\n"
            "}\n"
            "\n"
            "bool %1::startOutput(const QString& format, const QJsonObject& options)\n"
            "{\n"
            "    Q_UNUSED(format);\n"
            "    Q_UNUSED(options);\n"
            "    return true;\n"
            "}\n"
            "\n"
            "bool %1::writeData(const QByteArray& data)\n"
            "{\n"
            "    Q_UNUSED(data);\n"
            "    return true;\n"
            "}\n"
            "\n"
            "bool %1::stopOutput()\n"
            "{\n"
            "    return true;\n"
            "}\n"
        ).arg(className);
    } else if (type == PluginType::Remote) {
        impl += QString(
            "\n"
            "// IRemoteControlPlugin interface implementations\n"
            "quint16 %1::defaultPort() const\n"
            "{\n"
            "    return 8080;\n"
            "}\n"
            "\n"
            "QStringList %1::supportedCommands() const\n"
            "{\n"
            "    return QStringList() << \"play\" << \"pause\" << \"stop\";\n"
            "}\n"
            "\n"
            "bool %1::handleCommand(const QString& command, const QJsonObject& params, QJsonObject& response)\n"
            "{\n"
            "    Q_UNUSED(command);\n"
            "    Q_UNUSED(params);\n"
            "    Q_UNUSED(response);\n"
            "    return false;\n"
            "}\n"
        ).arg(className);
    }
    
    return impl;
}

QString PluginDeveloper::PluginTypeToString(PluginType type) {
    switch (type) {
        case PluginType::Visualizer: return "Visualizer";
        case PluginType::AudioEffect: return "AudioEffect";
        case PluginType::Output: return "Output";
        case PluginType::UI: return "UI";
        case PluginType::Utility: return "Utility";
        case PluginType::Remote: return "Remote";
        case PluginType::AI: return "AI";
        case PluginType::Integration: return "Integration";
        default: return "Utility";
    }
}

bool PluginDeveloper::buildPlugin(const QString& sourcePath, const QString& outputPath, const QString& kitPath) {
    DEBUG_TRACE("PluginDeveloper", QString("Building plugin from %1 to %2").arg(sourcePath).arg(outputPath));
    
    Q_UNUSED(kitPath); // Placeholder for future Qt Kit integration
    
    // For now, this is a placeholder
    // In a real implementation, this would use qmake or CMake to build the plugin
    QFileInfo sourceInfo(sourcePath);
    if (!sourceInfo.exists()) {
        ERROR_ERROR("PluginDeveloper", QString("Source file does not exist: %1").arg(sourcePath));
        return false;
    }
    
    // Create a simple build script
    QString buildScript = outputPath + ".build";
    QFile buildFile(buildScript);
    if (buildFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&buildFile);
        out << "#!/bin/bash\n";
        out << "echo 'Building plugin: " << sourcePath << "'\n";
        out << "# Placeholder build commands\n";
        out << "cp " << sourcePath << " " << outputPath << "\n";
        buildFile.close();
    }
    
    DEBUG_INFO("PluginDeveloper", QString("Plugin build script created: %1").arg(buildScript));
    return true;
}

QStringList PluginDeveloper::getBuildFlags(PluginType type) {
    QStringList flags;
    
    flags << "-fPIC" << "-shared";
    
    switch (type) {
        case PluginType::Visualizer:
            flags << "-DVISUALIZER_PLUGIN";
            break;
        case PluginType::AudioEffect:
            flags << "-DAUDIO_EFFECT_PLUGIN";
            break;
        case PluginType::Output:
            flags << "-DOUTPUT_PLUGIN";
            break;
        case PluginType::Remote:
            flags << "-DREMOTE_PLUGIN";
            break;
        default:
            flags << "-DUTILITY_PLUGIN";
            break;
    }
    
    return flags;
}

bool PluginDeveloper::testPlugin(const QString& pluginPath) {
    DEBUG_TRACE("PluginDeveloper", QString("Testing plugin: %1").arg(pluginPath));
    
    QFileInfo info(pluginPath);
    if (!info.exists()) {
        ERROR_ERROR("PluginDeveloper", QString("Plugin file does not exist: %1").arg(pluginPath));
        return false;
    }
    
    // Load the plugin and test basic functionality
    QPluginLoader loader(pluginPath);
    if (!loader.load()) {
        ERROR_ERROR("PluginDeveloper", QString("Failed to load plugin for testing: %1").arg(loader.errorString()));
        return false;
    }
    
    QObject* pluginObject = loader.instance();
    if (!pluginObject) {
        ERROR_ERROR("PluginDeveloper", "Failed to get plugin instance");
        loader.unload();
        return false;
    }
    
    IPlugin* plugin = qobject_cast<IPlugin*>(pluginObject);
    if (!plugin) {
        ERROR_ERROR("PluginDeveloper", "Plugin does not implement IPlugin interface");
        loader.unload();
        return false;
    }
    
    // Test basic plugin methods
    if (plugin->name().isEmpty()) {
        ERROR_ERROR("PluginDeveloper", "Plugin name is empty");
        loader.unload();
        return false;
    }
    
    if (plugin->version().isEmpty()) {
        ERROR_ERROR("PluginDeveloper", "Plugin version is empty");
        loader.unload();
        return false;
    }
    
    // Test initialization
    if (!plugin->initialize()) {
        ERROR_ERROR("PluginDeveloper", QString("Plugin initialization failed: %1").arg(plugin->lastError()));
        loader.unload();
        return false;
    }
    
    // Test configuration
    QJsonObject config = plugin->defaultConfig();
    if (!plugin->setConfig(config)) {
        ERROR_WARNING("PluginDeveloper", "Failed to set default configuration");
    }
    
    plugin->shutdown();
    loader.unload();
    
    DEBUG_INFO("PluginDeveloper", QString("Plugin test passed: %1").arg(plugin->name()));
    return true;
}

QJsonObject PluginDeveloper::validatePluginManifest(const QJsonObject& manifest) {
    QJsonObject result;
    QStringList errors;
    QStringList warnings;
    
    // Validate required fields
    QStringList required = {"name", "version", "author", "type"};
    for (const QString& field : required) {
        if (!manifest.contains(field)) {
            errors << QString("Missing required field: %1").arg(field);
        }
    }
    
    // Validate type
    if (manifest.contains("type")) {
        QString type = manifest["type"].toString();
        QStringList validTypes = {"Visualizer", "AudioEffect", "Output", "UI", "Utility", "Remote", "AI", "Integration"};
        if (!validTypes.contains(type)) {
            errors << QString("Invalid plugin type: %1").arg(type);
        }
    }
    
    // Validate version format
    if (manifest.contains("version")) {
        QString version = manifest["version"].toString();
        if (!QRegExp("\\d+\\.\\d+\\.\\d+").exactMatch(version)) {
            warnings << "Version should follow semantic versioning (x.y.z)";
        }
    }
    
    result["valid"] = errors.isEmpty();
    result["errors"] = QJsonArray::fromStringList(errors);
    result["warnings"] = QJsonArray::fromStringList(warnings);
    
    return result;
}

QString PluginDeveloper::generatePluginDocumentation(const QString& pluginPath) {
    QFileInfo info(pluginPath);
    if (!info.exists()) {
        return QString("Error: Plugin file does not exist: %1").arg(pluginPath);
    }
    
    QString doc;
    doc += QString("# Plugin Documentation: %1\n\n").arg(info.baseName());
    doc += "## Overview\n";
    doc += "This plugin was generated automatically by VibeSync Plugin Developer.\n\n";
    doc += "## Configuration\n";
    doc += "Configure this plugin through the VibeSync settings interface.\n\n";
    doc += "## Usage\n";
    doc += "1. Load the plugin through the Plugin Manager\n";
    doc += "2. Configure plugin settings as needed\n";
    doc += "3. Enable the plugin to start using its functionality\n\n";
    doc += "## Development\n";
    doc += "This plugin can be modified and rebuilt using the Plugin Developer tools.\n";
    
    return doc;
}

// PluginRegistry implementation
void PluginRegistry::registerBuiltInPlugins() {
    DEBUG_INFO("PluginRegistry", "Registering built-in plugins");
    
    registerBasicVisualizer();
    registerWaveformVisualizer();
    registerSpectrumAnalyzer();
    
    registerBasicEQ();
    registerReverbEffect();
    registerBassBoost();
    
    registerFileOutput();
    registerStreamOutput();
    
    registerHTTPAPI();
    registerWebSocketAPI();
    registerMIDIInput();
}

void PluginRegistry::registerBasicVisualizer() {
    DEBUG_INFO("PluginRegistry", "Registering basic visualizer plugin");
    // Placeholder for built-in visualizer
}

void PluginRegistry::registerWaveformVisualizer() {
    DEBUG_INFO("PluginRegistry", "Registering waveform visualizer plugin");
    // Placeholder for waveform visualizer
}

void PluginRegistry::registerSpectrumAnalyzer() {
    DEBUG_INFO("PluginRegistry", "Registering spectrum analyzer plugin");
    // Placeholder for spectrum analyzer
}

void PluginRegistry::registerBasicEQ() {
    DEBUG_INFO("PluginRegistry", "Registering basic EQ plugin");
    // Placeholder for basic EQ
}

void PluginRegistry::registerReverbEffect() {
    DEBUG_INFO("PluginRegistry", "Registering reverb effect plugin");
    // Placeholder for reverb effect
}

void PluginRegistry::registerBassBoost() {
    DEBUG_INFO("PluginRegistry", "Registering bass boost plugin");
    // Placeholder for bass boost
}

void PluginRegistry::registerFileOutput() {
    DEBUG_INFO("PluginRegistry", "Registering file output plugin");
    // Placeholder for file output
}

void PluginRegistry::registerStreamOutput() {
    DEBUG_INFO("PluginRegistry", "Registering stream output plugin");
    // Placeholder for stream output
}

void PluginRegistry::registerHTTPAPI() {
    DEBUG_INFO("PluginRegistry", "Registering HTTP API plugin");
    // Placeholder for HTTP API
}

void PluginRegistry::registerWebSocketAPI() {
    DEBUG_INFO("PluginRegistry", "Registering WebSocket API plugin");
    // Placeholder for WebSocket API
}

void PluginRegistry::registerMIDIInput() {
    DEBUG_INFO("PluginRegistry", "Registering MIDI input plugin");
    // Placeholder for MIDI input
}

// PluginEventSystem implementation
PluginEventSystem& PluginEventSystem::instance() {
    static PluginEventSystem instance;
    return instance;
}

void PluginEventSystem::registerEventHandler(const QString& eventName, std::function<void(const QJsonObject&)> handler) {
    QMutexLocker locker(&m_mutex);
    
    if (!m_handlers.contains(eventName)) {
        m_handlers[eventName] = QList<std::function<void(const QJsonObject&)>>();
    }
    
    m_handlers[eventName].append(handler);
    DEBUG_TRACE("PluginEventSystem", QString("Registered event handler for: %1").arg(eventName));
}

void PluginEventSystem::unregisterEventHandler(const QString& eventName, std::function<void(const QJsonObject&)> handler) {
    QMutexLocker locker(&m_mutex);
    
    auto it = m_handlers.find(eventName);
    if (it != m_handlers.end()) {
        it->removeAll(handler);
        if (it->isEmpty()) {
            m_handlers.remove(eventName);
        }
        DEBUG_TRACE("PluginEventSystem", QString("Unregistered event handler for: %1").arg(eventName));
    }
}

void PluginEventSystem::triggerEvent(const QString& eventName, const QJsonObject& data) {
    DEBUG_TRACE("PluginEventSystem", QString("Triggering event: %1").arg(eventName));
    
    emit eventTriggered(eventName, data);
    
    QMutexLocker locker(&m_mutex);
    auto it = m_handlers.find(eventName);
    if (it != m_handlers.end()) {
        for (const auto& handler : *it) {
            try {
                handler(data);
            } catch (const std::exception& e) {
                ERROR_ERROR("PluginEventSystem", QString("Exception in event handler for %1: %2").arg(eventName).arg(e.what()));
            }
        }
    }
}

void PluginEventSystem::triggerAudioEvent(const QJsonObject& audioData) {
    QJsonObject data = audioData;
    data["eventType"] = "audio";
    triggerEvent("audioData", data);
}

void PluginEventSystem::triggerVisualEvent(const QJsonObject& visualData) {
    QJsonObject data = visualData;
    data["eventType"] = "visualization";
    triggerEvent("visualizationData", data);
}

void PluginEventSystem::triggerUIAction(const QString& action, const QJsonObject& params) {
    QJsonObject data = params;
    data["action"] = action;
    data["eventType"] = "ui";
    triggerEvent("uiAction", data);
}

void PluginEventSystem::triggerAsyncEvent(const QString& eventName, const QJsonObject& data) {
    DEBUG_TRACE("PluginEventSystem", QString("Triggering async event: %1").arg(eventName));
    
    QtConcurrent::run([eventName, data]() {
        triggerEvent(eventName, data);
    });
}