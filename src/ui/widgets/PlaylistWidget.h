#pragma once
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "../../engine/PlaylistManager.h"

class PlaylistWidget : public QListWidget {
    Q_OBJECT
public:
    PlaylistWidget(PlaylistManager* manager, QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);
    void onPlaylistChanged();
    void onCurrentTrackChanged(const QString& filePath);

private:
    PlaylistManager* m_manager;
    void refreshPlaylist();
    void addFilesFromUrls(const QList<QUrl>& urls);
};