#include "SettingsManager.h"
#include "core/PathUtils.h"

void SettingsManager::setValue(const QString& key, const QVariant& value) {
    m_settings.setValue(key, value);
    emit settingChanged(key, value);
}

QVariant SettingsManager::value(const QString& key, const QVariant& defaultValue) const {
    return m_settings.value(key, defaultValue);
}

void SettingsManager::remove(const QString& key) {
    m_settings.remove(key);
    emit settingChanged(key, QVariant());
}

bool SettingsManager::contains(const QString& key) const {
    return m_settings.contains(key);
}

QString SettingsManager::getPresetPath() const {
    return value("viz/preset_path", PathUtils::getPresetPath()).toString();
}

bool SettingsManager::getShufflePresets() const {
    return value("viz/shuffle", true).toBool();
}

int SettingsManager::getFPS() const {
    return value("viz/fps", 60).toInt();
}

QString SettingsManager::getWatermarkText() const {
    return value("overlay/watermark_text", "vibe-sync").toString();
}

bool SettingsManager::getShowWatermark() const {
    return value("overlay/watermark_visible", false).toBool();
}

float SettingsManager::getGlobalScale() const {
    return value("overlay/global_scale", 1.0f).toFloat();
}

QString SettingsManager::getFFmpegCommand() const {
    QString defaultCmd = "ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt bgra -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset ultrafast -crf 18 {OUTPUT}";
    return value("recording/ffmpeg_cmd", defaultCmd).toString();
}

void SettingsManager::setPresetPath(const QString& path) {
    setValue("viz/preset_path", path);
}

void SettingsManager::setShufflePresets(bool shuffle) {
    setValue("viz/shuffle", shuffle);
}

void SettingsManager::setFPS(int fps) {
    setValue("viz/fps", fps);
}

void SettingsManager::setWatermarkText(const QString& text) {
    setValue("overlay/watermark_text", text);
}

void SettingsManager::setShowWatermark(bool show) {
    setValue("overlay/watermark_visible", show);
}

void SettingsManager::setGlobalScale(float scale) {
    setValue("overlay/global_scale", scale);
}

void SettingsManager::setFFmpegCommand(const QString& cmd) {
    setValue("recording/ffmpeg_cmd", cmd);
}