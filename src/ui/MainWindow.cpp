#include "MainWindow.h"
#include "menus/AppMenuBar.h"
#include "../core/PathUtils.h"
#include "../core/TextFormatter.h"
#include "widgets/VisualizerView.h"
#include "widgets/PlaylistWidget.h"
#include "widgets/DebugConsole.h"
#include "dialogs/SettingsDialog.h"
#include "../engine/AudioEngine.h"
#include "../engine/PresetManager.h"
#include "../engine/PlaylistManager.h"
#include "../engine/VideoRecorder.h"
#include "../data/SettingsManager.h"

#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    resize(1280, 800);
    setWindowTitle("vibe-sync [DEV]");

    // Initialize core components
    m_playlistMgr = new PlaylistManager(this);
    m_presetMgr = new PresetManager(this);
    m_viz = new VisualizerView(this);
    m_recorder = new VideoRecorder(this);
    
    // Load settings
    SettingsManager& settings = SettingsManager::instance();
    QString presetPath = settings.getPresetPath();
    if(!presetPath.isEmpty()) {
        m_presetMgr->setPresetDirectory(presetPath);
    }
    
    m_viz->setRecorder(m_recorder);

    setupUI();
    setupConnections();

    qDebug() << "🎵 vibe-sync initialized successfully";
}

void MainWindow::setupUI() {
    // Central Visualizer
    setCentralWidget(m_viz);

    // Left Dock: Playlist
    QDockWidget* dockPlaylist = new QDockWidget("Playlist", this);
    PlaylistWidget* playlistWidget = new PlaylistWidget(m_playlistMgr, dockPlaylist);
    dockPlaylist->setWidget(playlistWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockPlaylist);

    // Right Dock: Controls
    QDockWidget* dockControls = new QDockWidget("Visualizer Controls", this);
    QWidget* controlPanel = new QWidget;
    QVBoxLayout* ctrlLayout = new QVBoxLayout(controlPanel);
    
    m_lblPreset = new QLabel("Current Preset", this);
    m_lblPreset->setWordWrap(true);
    m_lblPreset->setStyleSheet("font-weight: bold; color: cyan;");
    
    m_btnNextPreset = new QPushButton("Next Preset", this);
    m_btnPrevPreset = new QPushButton("Prev Preset", this);
    m_btnFav = new QPushButton("❤️ Favorite", this);
    m_btnBlack = new QPushButton("🚫 Blacklist", this);
    m_btnQuarantine = new QPushButton("☣️ QUARANTINE", this);
    m_btnQuarantine->setStyleSheet("background-color: #550000; color: #ffaaaa; border: 1px solid red;");
    m_chkLock = new QCheckBox("Lock Preset", this);

    m_btnRecord = new QPushButton("Start Recording", this);
    m_btnRecord->setStyleSheet("background-color: #004400; color: #aaffaa;");

    ctrlLayout->addWidget(new QLabel("<b>Visualizer</b>"));
    ctrlLayout->addWidget(m_lblPreset);
    ctrlLayout->addWidget(m_btnNextPreset);
    ctrlLayout->addWidget(m_btnPrevPreset);
    ctrlLayout->addSpacing(10);
    ctrlLayout->addWidget(m_btnFav);
    ctrlLayout->addWidget(m_btnBlack);
    ctrlLayout->addWidget(m_btnQuarantine);
    ctrlLayout->addWidget(m_chkLock);
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(m_btnRecord);

    dockControls->setWidget(controlPanel);
    addDockWidget(Qt::RightDockWidgetArea, dockControls);
    
    // Bottom Debug Console
    QDockWidget* dockConsole = new QDockWidget("Debug Console", this);
    DebugConsole* debugConsole = new DebugConsole(dockConsole);
    dockConsole->setWidget(debugConsole);
    addDockWidget(Qt::BottomDockWidgetArea, dockConsole);

    // Menu Bar
    m_menu = new AppMenuBar(this);
    setMenuBar(m_menu);
}

void MainWindow::setupConnections() {
    // Menu connections
    connect(m_menu, &AppMenuBar::showSettingsRequested, this, &MainWindow::onShowSettings);
    connect(m_menu, &AppMenuBar::openFilesRequested, this, &MainWindow::onOpenFiles);
    connect(m_menu, &AppMenuBar::openFolderRequested, this, &MainWindow::onOpenFolder);

    // Playlist connections
    connect(m_playlistMgr, &PlaylistManager::currentTrackChanged, this, &MainWindow::onCurrentTrackChanged);
    connect(m_playlistMgr, &PlaylistManager::playlistChanged, this, &MainWindow::onPlaylistChanged);

    // Preset connections
    connect(m_presetMgr, &PresetManager::currentPresetChanged, this, [this](const QString& presetPath) {
        m_viz->loadPreset(presetPath);
        QFileInfo fi(presetPath);
        m_lblPreset->setText(fi.fileName());
        
        if(m_presetMgr->isFavorite(presetPath)) {
            m_btnFav->setText("❤️ Un-Favorite");
        } else {
            m_btnFav->setText("🤍 Favorite");
        }
        
        qDebug() << "👁️ Loaded Preset:" << fi.fileName();
    });

    // Control connections
    connect(m_btnNextPreset, &QPushButton::clicked, this, &MainWindow::onNextPreset);
    connect(m_btnPrevPreset, &QPushButton::clicked, this, &MainWindow::onPrevPreset);
    connect(m_btnFav, &QPushButton::clicked, this, &MainWindow::onToggleFavorite);
    connect(m_btnBlack, &QPushButton::clicked, this, &MainWindow::onToggleBlacklist);
    connect(m_btnQuarantine, &QPushButton::clicked, this, &MainWindow::onQuarantinePreset);
    connect(m_btnRecord, &QPushButton::clicked, this, &MainWindow::onRecordToggle);

    // Auto-advance presets every 15 seconds (if not locked)
    QTimer* presetTimer = new QTimer(this);
    connect(presetTimer, &QTimer::timeout, [this]() {
        if(!m_chkLock->isChecked()) {
            onNextPreset();
        }
    });
    presetTimer->start(15000);

    // Load initial preset
    QTimer::singleShot(1000, this, [this]() {
        onNextPreset();
    });
}

void MainWindow::onShowSettings() {
    SettingsDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        SettingsManager& settings = SettingsManager::instance();
        m_presetMgr->setPresetDirectory(settings.getPresetPath());
        m_viz->textEngine()->setGlobalScale(settings.getGlobalScale());
        m_viz->textEngine()->setVisible("watermark", settings.getShowWatermark());
        m_viz->textEngine()->updateText("watermark", settings.getWatermarkText());
        
        qDebug() << "⚙️ Settings applied. Some changes may require restart.";
    }
}

void MainWindow::onOpenFiles() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Add Music", QDir::homePath(), 
        "Audio Files (*.mp3 *.wav *.flac *.ogg *.m4a *.aac)");
    if(!files.isEmpty()) {
        m_playlistMgr->addFiles(files);
        if(m_playlistMgr->currentIndex() == -1) {
            m_playlistMgr->playAtIndex(0);
        }
    }
}

void MainWindow::onOpenFolder() {
    QString dir = QFileDialog::getExistingDirectory(this, "Add Music Folder");
    if(!dir.isEmpty()) {
        QDir d(dir);
        QStringList files = d.entryList({"*.mp3", "*.wav", "*.flac", "*.ogg", "*.m4a", "*.aac"}, QDir::Files);
        QStringList fullPaths;
        for(const auto& f : files) {
            fullPaths << d.absoluteFilePath(f);
        }
        m_playlistMgr->addFiles(fullPaths);
    }
}

void MainWindow::onNextPreset() {
    QString preset = m_presetMgr->nextPreset();
    if(!preset.isEmpty()) {
        m_viz->loadPreset(preset);
        QFileInfo fi(preset);
        m_lblPreset->setText(fi.fileName());
    }
}

void MainWindow::onPrevPreset() {
    QString preset = m_presetMgr->previousPreset();
    if(!preset.isEmpty()) {
        m_viz->loadPreset(preset);
        QFileInfo fi(preset);
        m_lblPreset->setText(fi.fileName());
    }
}

void MainWindow::onToggleFavorite() {
    QString current = m_presetMgr->currentPreset();
    if(!current.isEmpty()) {
        m_presetMgr->toggleFavorite(current);
        onNextPreset(); // Refresh the button state
    }
}

void MainWindow::onToggleBlacklist() {
    QString current = m_presetMgr->currentPreset();
    if(!current.isEmpty()) {
        m_presetMgr->toggleBlacklist(current);
        onNextPreset(); // Move to next preset
    }
}

void MainWindow::onQuarantinePreset() {
    m_presetMgr->quarantineCurrentPreset();
    onNextPreset(); // Move to next preset
}

void MainWindow::onRecordToggle() {
    if (!m_recorder->isRecording()) {
        QString title = m_playlistMgr->currentFile();
        if (title.isEmpty()) title = "VisualizerCapture";
        else title = QFileInfo(title).completeBaseName();
        
        if (m_recorder->start(title)) {
            m_btnRecord->setText("⏹️ Stop Recording");
            m_btnRecord->setStyleSheet("background-color: #aa0000; color: white;");
        }
    } else {
        m_recorder->stop();
        m_btnRecord->setText("Start Recording");
        m_btnRecord->setStyleSheet("background-color: #004400; color: #aaffaa;");
    }
}

void MainWindow::onCurrentTrackChanged(const QString& filePath) {
    if(AudioEngine::instance().loadFile(filePath)) {
        AudioEngine::instance().play();
    }
    
    auto info = TextFormatter::parse(filePath);
    
    // Update the "metadata" element in TextEngine
    m_viz->textEngine()->updateText("metadata", info.displayString);
    
    qDebug() << "🎵 Now Playing:" << info.artist << "-" << info.title;
}

void MainWindow::onPlaylistChanged() {
    qDebug() << "📋 Playlist updated with" << m_playlistMgr->count() << "tracks";
}