#include "AudioEngine.h"
#include <QDebug>

AudioEngine::AudioEngine() {
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
    
    m_positionTimer = new QTimer(this);
    connect(m_positionTimer, &QTimer::timeout, this, [this]() {
        emit positionChanged(m_player->position());
    });
    
    setupConnections();
}

void AudioEngine::setupConnections() {
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        switch (state) {
        case QMediaPlayer::PlayingState:
            m_positionTimer->start(100);
            emit playbackStarted();
            break;
        case QMediaPlayer::PausedState:
            m_positionTimer->stop();
            emit playbackPaused();
            break;
        case QMediaPlayer::StoppedState:
            m_positionTimer->stop();
            emit playbackStopped();
            break;
        }
    });
    
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            emit playbackFinished();
        }
    });
    
    connect(m_player, &QMediaPlayer::durationChanged, this, &AudioEngine::durationChanged);
}

bool AudioEngine::loadFile(const QString& filePath) {
    m_player->setSource(QUrl::fromLocalFile(filePath));
    return m_player->error() == QMediaPlayer::NoError;
}

void AudioEngine::play() {
    m_player->play();
}

void AudioEngine::pause() {
    m_player->pause();
}

void AudioEngine::stop() {
    m_player->stop();
}

bool AudioEngine::isPlaying() const {
    return m_player->playbackState() == QMediaPlayer::PlayingState;
}

qint64 AudioEngine::position() const {
    return m_player->position();
}

qint64 AudioEngine::duration() const {
    return m_player->duration();
}

void AudioEngine::setPosition(qint64 position) {
    m_player->setPosition(position);
}

void AudioEngine::setVolume(int volume) {
    m_audioOutput->setVolume(volume / 100.0f);
}