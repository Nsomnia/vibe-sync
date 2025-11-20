#pragma once
#include <QObject>
#include <QProcess>
#include <QImage>
#include <QDateTime>
#include <QDir>

class VideoRecorder : public QObject {
    Q_OBJECT
public:
    explicit VideoRecorder(QObject* parent = nullptr);
    bool start(const QString& songTitle);
    void stop();
    void writeFrame(const QImage& img);
    bool isRecording() const { return m_isRecording; }
    
    void setCommandTemplate(const QString& cmd) { m_cmdTemplate = cmd; }
    QString getCommandTemplate() const { return m_cmdTemplate; }

private:
    QProcess* m_ffmpeg = nullptr;
    bool m_isRecording = false;
    QString m_cmdTemplate;
};