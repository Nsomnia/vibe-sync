#include "PresetManager.h"
#include <QSettings>
#include <QDebug>

PresetManager::PresetManager(QObject* parent) : QObject(parent) {
    loadLists();
}

void PresetManager::setPresetDirectory(const QString& path) {
    m_presetDirectory = path;
    scanPresets();
    validatePresetList();
    emit presetListChanged();
}

QString PresetManager::currentPreset() const {
    if (m_currentIndex >= 0 && m_currentIndex < m_allPresets.size()) {
        return m_allPresets[m_currentIndex];
    }
    return QString();
}

QString PresetManager::nextPreset() {
    if (m_allPresets.isEmpty()) return QString();
    
    int nextIndex = m_currentIndex + 1;
    if (nextIndex >= m_allPresets.size()) nextIndex = 0;
    
    m_currentIndex = nextIndex;
    QString preset = currentPreset();
    emit currentPresetChanged(preset);
    return preset;
}

QString PresetManager::previousPreset() {
    if (m_allPresets.isEmpty()) return QString();
    
    int prevIndex = m_currentIndex - 1;
    if (prevIndex < 0) prevIndex = m_allPresets.size() - 1;
    
    m_currentIndex = prevIndex;
    QString preset = currentPreset();
    emit currentPresetChanged(preset);
    return preset;
}

QStringList PresetManager::getAllPresets() const {
    return m_allPresets;
}

void PresetManager::toggleFavorite(const QString& presetPath) {
    if (m_favorites.contains(presetPath)) {
        m_favorites.removeOne(presetPath);
    } else {
        m_favorites.append(presetPath);
    }
    saveLists();
}

void PresetManager::toggleBlacklist(const QString& presetPath) {
    if (m_blacklist.contains(presetPath)) {
        m_blacklist.removeOne(presetPath);
    } else {
        m_blacklist.append(presetPath);
    }
    saveLists();
}

bool PresetManager::isFavorite(const QString& presetPath) const {
    return m_favorites.contains(presetPath);
}

bool PresetManager::isBlacklisted(const QString& presetPath) const {
    return m_blacklist.contains(presetPath);
}

void PresetManager::quarantineCurrentPreset() {
    QString current = currentPreset();
    if (!current.isEmpty() && !m_quarantine.contains(current)) {
        m_quarantine.append(current);
        saveLists();
        qDebug() << "🗑️ Quarantined preset:" << getPresetName(current);
    }
}

QStringList PresetManager::getQuarantinedPresets() const {
    return m_quarantine;
}

QString PresetManager::getPresetName(const QString& presetPath) const {
    QFileInfo fi(presetPath);
    return fi.fileName();
}

void PresetManager::loadLists() {
    QSettings settings("NeonCorp", "NeonVisualizer");
    m_favorites = settings.value("presets/favorites").toStringList();
    m_blacklist = settings.value("presets/blacklist").toStringList();
    m_quarantine = settings.value("presets/quarantine").toStringList();
}

void PresetManager::saveLists() {
    QSettings settings("NeonCorp", "NeonVisualizer");
    settings.setValue("presets/favorites", m_favorites);
    settings.setValue("presets/blacklist", m_blacklist);
    settings.setValue("presets/quarantine", m_quarantine);
}

void PresetManager::scanPresets() {
    m_allPresets.clear();
    
    if (m_presetDirectory.isEmpty()) return;
    
    QDir dir(m_presetDirectory);
    QStringList filters = {"*.milk", "*.prjm", "*.fx"};
    QStringList files = dir.entryList(filters, QDir::Files);
    
    for(const QString& file : files) {
        QString fullPath = dir.absoluteFilePath(file);
        if (!m_blacklist.contains(fullPath) && !m_quarantine.contains(fullPath)) {
            m_allPresets.append(fullPath);
        }
    }
    
    m_currentIndex = 0;
}

void PresetManager::validatePresetList() {
    // Remove any presets that no longer exist
    m_allPresets.erase(
        std::remove_if(m_allPresets.begin(), m_allPresets.end(),
                      [](const QString& preset) { return !QFile::exists(preset); }),
        m_allPresets.end()
    );
    
    // Ensure current index is valid
    if (m_currentIndex >= m_allPresets.size()) {
        m_currentIndex = 0;
    }
}