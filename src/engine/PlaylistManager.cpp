#include "PlaylistManager.h"
#include <QDebug>

PlaylistManager::PlaylistManager(QObject* parent) : QObject(parent) {
    m_random = QRandomGenerator::securelySeeded();
}

void PlaylistManager::addFiles(const QStringList& filePaths) {
    int addedCount = 0;
    for(const QString& filePath : filePaths) {
        if (addFile(filePath)) {
            addedCount++;
        }
    }
    
    if (addedCount > 0) {
        emit playlistChanged();
        qDebug() << "📁 Added" << addedCount << "files to playlist";
    }
}

bool PlaylistManager::addFile(const QString& filePath) {
    if (!isValidAudioFile(filePath)) return false;
    
    // Avoid duplicates
    if (!m_playlist.contains(filePath)) {
        m_playlist.append(filePath);
        return true;
    }
    return false;
}

void PlaylistManager::removeFile(int index) {
    if (index >= 0 && index < m_playlist.count()) {
        bool wasCurrent = (index == m_currentIndex);
        m_playlist.removeAt(index);
        
        // Adjust current index if necessary
        if (wasCurrent) {
            m_currentIndex = -1;
        } else if (index < m_currentIndex) {
            m_currentIndex--;
        }
        
        emit playlistChanged();
    }
}

void PlaylistManager::clear() {
    m_playlist.clear();
    m_currentIndex = -1;
    emit playlistChanged();
}

int PlaylistManager::currentIndex() const {
    return m_currentIndex;
}

QString PlaylistManager::currentFile() const {
    if (m_currentIndex >= 0 && m_currentIndex < m_playlist.count()) {
        return m_playlist[m_currentIndex];
    }
    return QString();
}

void PlaylistManager::playAtIndex(int index) {
    if (index >= 0 && index < m_playlist.count()) {
        m_currentIndex = index;
        QString file = currentFile();
        emit currentTrackChanged(file);
        emit playbackStarted(file);
        qDebug() << "🎵 Playing:" << QFileInfo(file).fileName();
    }
}

void PlaylistManager::next() {
    if (m_playlist.isEmpty()) return;
    
    int nextIndex;
    if (m_shuffle) {
        nextIndex = m_random.bounded(0, m_playlist.count());
    } else {
        nextIndex = m_currentIndex + 1;
        if (nextIndex >= m_playlist.count()) {
            nextIndex = 0;
        }
    }
    
    playAtIndex(nextIndex);
}

void PlaylistManager::previous() {
    if (m_playlist.isEmpty()) return;
    
    int prevIndex = m_currentIndex - 1;
    if (prevIndex < 0) {
        prevIndex = m_playlist.count() - 1;
    }
    
    playAtIndex(prevIndex);
}

bool PlaylistManager::shuffle() const {
    return m_shuffle;
}

void PlaylistManager::setShuffle(bool enable) {
    m_shuffle = enable;
}

bool PlaylistManager::isValidAudioFile(const QString& filePath) const {
    QString extension = QFileInfo(filePath).suffix().toLower();
    return QStringList({"mp3", "wav", "flac", "ogg", "m4a", "aac"}).contains(extension);
}