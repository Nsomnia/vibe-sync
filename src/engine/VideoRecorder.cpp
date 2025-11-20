#include "VideoRecorder.h"
#include "../core/StringUtils.h"
#include <QStandardPaths>
#include <QDebug>

VideoRecorder::VideoRecorder(QObject* parent) : QObject(parent) {
    m_ffmpeg = new QProcess(this);
    m_cmdTemplate = "ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt bgra -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset ultrafast -crf 18 {OUTPUT}";
}

bool VideoRecorder::start(const QString& songTitle) {
    if (m_isRecording) return false;

    // USE UTILITY
    QString safeTitle = StringUtils::safeFilename(songTitle); 
    if (safeTitle.isEmpty()) safeTitle = "UnknownTrack";
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString filename = QString("%1/%2_%3.mp4").arg(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation), safeTitle, timestamp);

    // Setup FFmpeg with template
    QStringList args;
    // Parse template for placeholders
    QString cmd = m_cmdTemplate;
    cmd.replace("{OUTPUT}", filename);
    cmd.replace("{WIDTH}", "1920");
    cmd.replace("{HEIGHT}", "1080");
    cmd.replace("{FPS}", "60");
    
    // Split command into arguments
    QStringList argParts = cmd.split(" ", Qt::SkipEmptyParts);
    if (!argParts.isEmpty()) {
        QString program = argParts.takeFirst();
        m_ffmpeg->start(program, argParts);
        m_isRecording = m_ffmpeg->waitForStarted();
        return m_isRecording;
    }
    
    return false;
}

void VideoRecorder::stop() {
    if (!m_isRecording) return;
    m_ffmpeg->closeWriteChannel(); // Tells ffmpeg EOF
    m_ffmpeg->waitForFinished();
    m_isRecording = false;
}

void VideoRecorder::writeFrame(const QImage& img) {
    if (!m_isRecording) return;
    // Ensure size matches
    QImage scaled = img.scaled(1920, 1080, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // Convert to format expected by ffmpeg
    QImage raw = scaled.convertToFormat(QImage::Format_ARGB32);
    m_ffmpeg->write((const char*)raw.bits(), raw.sizeInBytes());
}