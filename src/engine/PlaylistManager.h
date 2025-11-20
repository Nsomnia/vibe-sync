#pragma once
#include <QObject>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QMediaPlayer>

class PlaylistManager : public QObject {
    Q_OBJECT
public:
    explicit PlaylistManager(QObject* parent = nullptr);
    
    void addFiles(const QStringList& filePaths);
    void addFile(const QString& filePath);
    void removeFile(int index);
    void clear();
    
    int currentIndex() const;
    QString currentFile() const;
    
    void playAtIndex(int index);
    void next();
    void previous();
    bool shuffle() const;
    void setShuffle(bool enable);
    
    int count() const { return m_playlist.count(); }
    QStringList getPlaylist() const { return m_playlist; }

signals:
    void currentTrackChanged(const QString& filePath);
    void playlistChanged();
    void playbackStarted(const QString& filePath);
    void playbackFinished();

private:
    QStringList m_playlist;
    int m_currentIndex = -1;
    bool m_shuffle = false;
    QRandomGenerator m_random;
    
    bool isValidAudioFile(const QString& filePath) const;
};