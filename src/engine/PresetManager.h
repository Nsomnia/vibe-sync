#pragma once
#include <QObject>
#include <QStringList>
#include <QDir>
#include <QRandomGenerator>

class PresetManager : public QObject {
    Q_OBJECT
public:
    PresetManager(QObject* parent = nullptr);
    
    void setPresetDirectory(const QString& path);
    QString currentPreset() const;
    QString nextPreset();
    QString previousPreset();
    QStringList getAllPresets() const;
    
    // Favorites/Blacklist management
    void toggleFavorite(const QString& presetPath);
    void toggleBlacklist(const QString& presetPath);
    bool isFavorite(const QString& presetPath) const;
    bool isBlacklisted(const QString& presetPath) const;
    
    // Quarantine system (for problematic presets)
    void quarantineCurrentPreset();
    QStringList getQuarantinedPresets() const;
    
    // Utility
    QString getPresetName(const QString& presetPath) const;

signals:
    void currentPresetChanged(const QString& presetPath);
    void presetListChanged();

private:
    QString m_presetDirectory;
    QStringList m_allPresets;
    QStringList m_favorites;
    QStringList m_blacklist;
    QStringList m_quarantine;
    int m_currentIndex = 0;
    
    void loadLists();
    void saveLists();
    void scanPresets();
    void validatePresetList();
};