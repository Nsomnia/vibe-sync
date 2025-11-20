#pragma once
#include <QMainWindow>
#include <QDockWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QTimer>
#include <QFileInfo>

class PlaylistManager;
class PresetManager;
class VisualizerView;
class VideoRecorder;
class AppMenuBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onShowSettings();
    void onOpenFiles();
    void onOpenFolder();
    void onNextPreset();
    void onPrevPreset();
    void onToggleFavorite();
    void onToggleBlacklist();
    void onQuarantinePreset();
    void onRecordToggle();
    void onCurrentTrackChanged(const QString& filePath);
    void onPlaylistChanged();

private:
    void setupUI();
    void setupConnections();
    
    // Core components
    PlaylistManager* m_playlistMgr = nullptr;
    PresetManager* m_presetMgr = nullptr;
    VisualizerView* m_viz = nullptr;
    VideoRecorder* m_recorder = nullptr;
    AppMenuBar* m_menu = nullptr;
    
    // UI elements
    QLabel* m_lblPreset = nullptr;
    QPushButton* m_btnNextPreset = nullptr;
    QPushButton* m_btnPrevPreset = nullptr;
    QPushButton* m_btnFav = nullptr;
    QPushButton* m_btnBlack = nullptr;
    QPushButton* m_btnQuarantine = nullptr;
    QPushButton* m_btnRecord = nullptr;
    QCheckBox* m_chkLock = nullptr;
};