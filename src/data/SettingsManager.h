#pragma once
#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QString>

class SettingsManager : public QObject {
    Q_OBJECT
public:
    static SettingsManager& instance() {
        static SettingsManager s;
        return s;
    }

    // Generic setters/getters
    void setValue(const QString& key, const QVariant& value);
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void remove(const QString& key);
    bool contains(const QString& key) const;

    // Specialized getters for common settings
    QString getPresetPath() const;
    bool getShufflePresets() const;
    int getFPS() const;
    QString getWatermarkText() const;
    bool getShowWatermark() const;
    float getGlobalScale() const;
    QString getFFmpegCommand() const;

    // Specialized setters
    void setPresetPath(const QString& path);
    void setShufflePresets(bool shuffle);
    void setFPS(int fps);
    void setWatermarkText(const QString& text);
    void setShowWatermark(bool show);
    void setGlobalScale(float scale);
    void setFFmpegCommand(const QString& cmd);

signals:
    void settingChanged(const QString& key, const QVariant& value);

private:
    SettingsManager() {}
    QSettings m_settings;
};