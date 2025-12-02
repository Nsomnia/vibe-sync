#pragma once
#include <QObject>
#include <QPluginLoader>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include <functional>

// Plugin types and interfaces
enum class PluginType {
    Visualizer,     // Custom visualizer effects
    AudioEffect,    // Audio processing effects
    Output,         // Custom output formats/destinations
    UI,            // UI extensions
    Utility,       // Utility plugins
    Remote,        // Remote control integration
    AI,            // AI-powered features
    Integration    // Third-party integrations
};

enum class PluginState {
    Unloaded,
    Loading,
    Loaded,
    Active,
    Error,
    Disabled
};

// Base plugin interface
class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    // Plugin information
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;
    virtual QString author() const = 0;
    virtual PluginType type() const = 0;
    virtual QStringList dependencies() const = 0;
    
    // Lifecycle management
    virtual bool initialize(const QJsonObject& config = QJsonObject()) = 0;
    virtual void shutdown() = 0;
    virtual PluginState state() const = 0;
    virtual QString lastError() const = 0;
    
    // Configuration
    virtual QJsonObject defaultConfig() const = 0;
    virtual QJsonObject currentConfig() const = 0;
    virtual bool setConfig(const QJsonObject& config) = 0;
    
    // Capabilities
    virtual QStringList capabilities() const = 0;
    virtual bool hasCapability(const QString& capability) const = 0;
};

// Specific plugin interfaces
class IVisualizerPlugin : public IPlugin {
public:
    virtual QStringList presetFormats() const = 0;
    virtual bool canLoadPreset(const QString& filePath) const = 0;
    virtual QByteArray processVisualization(const QByteArray& audioData, const QSize& targetSize) = 0;
};

class IAudioEffectPlugin : public IPlugin {
public:
    virtual QStringList supportedFormats() const = 0;
    virtual QByteArray processAudio(const QByteArray& audioData, int sampleRate, int channels) = 0;
};

class IOutputPlugin : public IPlugin {
public:
    virtual QStringList outputFormats() const = 0;
    virtual bool canOutputTo(const QString& format, const QString& destination) const = 0;
    virtual bool startOutput(const QString& format, const QJsonObject& options) = 0;
    virtual bool writeData(const QByteArray& data) = 0;
    virtual bool stopOutput() = 0;
};

class IRemoteControlPlugin : public IPlugin {
public:
    virtual quint16 defaultPort() const = 0;
    virtual QStringList supportedCommands() const = 0;
    virtual bool handleCommand(const QString& command, const QJsonObject& params, QJsonObject& response) = 0;
};

// Plugin manager for loading and managing plugins
class PluginManager : public QObject {
    Q_OBJECT
public:
    static PluginManager& instance();
    
    // Plugin discovery and loading
    QStringList discoverPlugins();
    bool loadPlugin(const QString& pluginId);
    bool unloadPlugin(const QString& pluginId);
    bool reloadPlugin(const QString& pluginId);
    
    // Plugin management
    QStringList loadedPlugins() const;
    QStringList availablePlugins() const;
    QStringList pluginsByType(PluginType type) const;
    IPlugin* getPlugin(const QString& pluginId) const;
    PluginState getPluginState(const QString& pluginId) const;
    
    // Plugin configuration
    QJsonObject getPluginConfig(const QString& pluginId) const;
    bool setPluginConfig(const QString& pluginId, const QJsonObject& config);
    
    // Plugin execution
    bool enablePlugin(const QString& pluginId);
    bool disablePlugin(const QString& pluginId);
    bool executePluginAction(const QString& pluginId, const QString& action, const QJsonObject& params = QJsonObject());
    
    // Plugin communication
    void broadcastEvent(const QString& eventName, const QJsonObject& data = QJsonObject());
    void sendMessageToPlugin(const QString& pluginId, const QString& message, const QJsonObject& data = QJsonObject());
    
    // Settings
    void setPluginDirectory(const QString& directory);
    QString pluginDirectory() const { return m_pluginDirectory; }
    void setAutoLoadEnabled(bool enabled) { m_autoLoadEnabled = enabled; }
    bool isAutoLoadEnabled() const { return m_autoLoadEnabled; }

signals:
    void pluginLoaded(const QString& pluginId);
    void pluginUnloaded(const QString& pluginId);
    void pluginError(const QString& pluginId, const QString& error);
    void pluginMessage(const QString& pluginId, const QString& message, const QJsonObject& data);
    void pluginEvent(const QString& eventName, const QJsonObject& data);

private:
    PluginManager();
    
    void scanPluginDirectory();
    QStringList scanDirectoryForPlugins(const QString& directory);
    QString generatePluginId(const QString& filePath) const;
    
    struct PluginInfo {
        QString filePath;
        QString name;
        QString version;
        PluginType type;
        PluginState state;
        QString error;
        QJsonObject config;
        std::unique_ptr<QPluginLoader> loader;
        IPlugin* interface = nullptr;
    };
    
    QMap<QString, PluginInfo> m_plugins;
    QString m_pluginDirectory;
    bool m_autoLoadEnabled = true;
    
    mutable QMutex m_mutex;
};

// Plugin development utilities
class PluginDeveloper {
public:
    // Plugin template generation
    static QString generatePluginTemplate(const QString& name, PluginType type);
    static QString generatePluginHeader(const QString& name, PluginType type);
    static QString generatePluginImplementation(const QString& name, PluginType type);
    
    // Build utilities
    static bool buildPlugin(const QString& sourcePath, const QString& outputPath, const QString& kitPath = QString());
    static QStringList getBuildFlags(PluginType type);
    
    // Testing utilities
    static bool testPlugin(const QString& pluginPath);
    static QJsonObject validatePluginManifest(const QJsonObject& manifest);
    
    // Documentation
    static QString generatePluginDocumentation(const QString& pluginPath);

private:
    PluginDeveloper() = default;
};

// Plugin registry for built-in plugins
class PluginRegistry {
public:
    static void registerBuiltInPlugins();
    
    // Built-in visualizer plugins
    static void registerBasicVisualizer();
    static void registerWaveformVisualizer();
    static void registerSpectrumAnalyzer();
    
    // Built-in audio effect plugins
    static void registerBasicEQ();
    static void registerReverbEffect();
    static void registerBassBoost();
    
    // Built-in output plugins
    static void registerFileOutput();
    static void registerStreamOutput();
    
    // Built-in remote control plugins
    static void registerHTTPAPI();
    static void registerWebSocketAPI();
    static void registerMIDIInput();

private:
    PluginRegistry() = default;
};

// Plugin event system for communication
class PluginEventSystem {
public:
    static PluginEventSystem& instance();
    
    // Event registration
    void registerEventHandler(const QString& eventName, std::function<void(const QJsonObject&)> handler);
    void unregisterEventHandler(const QString& eventName, std::function<void(const QJsonObject&)> handler);
    
    // Event triggering
    void triggerEvent(const QString& eventName, const QJsonObject& data = QJsonObject());
    void triggerAudioEvent(const QJsonObject& audioData);
    void triggerVisualEvent(const QJsonObject& visualData);
    void triggerUIAction(const QString& action, const QJsonObject& params = QJsonObject());
    
    // Async events
    void triggerAsyncEvent(const QString& eventName, const QJsonObject& data = QJsonObject());

signals:
    void eventTriggered(const QString& eventName, const QJsonObject& data);

private:
    PluginEventSystem() = default;
    
    QMap<QString, QList<std::function<void(const QJsonObject&)>>> m_handlers;
    mutable QMutex m_mutex;
};

// Convenience macros for plugin development
#define DECLARE_PLUGIN(className) \
public: \
    static const char* PluginName; \
    static const char* PluginVersion; \
    static const char* PluginDescription; \
    static const char* PluginAuthor; \
    static PluginType PluginType; \
    \
    QString name() const override { return PluginName; } \
    QString version() const override { return PluginVersion; } \
    QString description() const override { return PluginDescription; } \
    QString author() const override { return PluginAuthor; } \
    PluginType type() const override { return PluginType; }

#define DEFINE_PLUGIN(className, name, version, desc, author, pluginType) \
    const char* className::PluginName = name; \
    const char* className::PluginVersion = version; \
    const char* className::PluginDescription = desc; \
    const char* className::PluginAuthor = author; \
    PluginType className::PluginType = pluginType;

#define PLUGIN_FACTORY(className) \
    extern "C" IPlugin* createPlugin() { \
        return new className(); \
    }