#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QString>

class AudioEngine : public QObject {
    Q_OBJECT
public:
    static AudioEngine& instance() {
        static AudioEngine s;
        return s;
    }

    bool loadFile(const QString& filePath);
    void play();
    void pause();
    void stop();
    
    bool isPlaying() const;
    qint64 position() const;
    qint64 duration() const;
    
    void setPosition(qint64 position);
    void setVolume(int volume);

signals:
    void playbackStarted();
    void playbackPaused();
    void playbackStopped();
    void playbackFinished();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);

private:
    AudioEngine();
    
    QMediaPlayer* m_player = nullptr;
    QAudioOutput* m_audioOutput = nullptr;
    QTimer* m_positionTimer = nullptr;
    
    void setupConnections();
};