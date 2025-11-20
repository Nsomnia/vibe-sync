#include "AppMenuBar.h"

AppMenuBar::AppMenuBar(QWidget* parent) : QMenuBar(parent) {
    // File Menu
    QMenu* fileMenu = addMenu("File");
    
    m_openFilesAction = fileMenu->addAction("Add Files...");
    connect(m_openFilesAction, &QAction::triggered, this, &AppMenuBar::onOpenFiles);
    
    m_openFolderAction = fileMenu->addAction("Add Folder...");
    connect(m_openFolderAction, &QAction::triggered, this, &AppMenuBar::onOpenFolder);
    
    fileMenu->addSeparator();
    
    m_settingsAction = fileMenu->addAction("Settings...");
    connect(m_settingsAction, &QAction::triggered, this, &AppMenuBar::onShowSettings);
    
    fileMenu->addSeparator();
    
    m_quitAction = fileMenu->addAction("Quit");
    connect(m_quitAction, &QAction::triggered, this, &AppMenuBar::onQuit);

    // View Menu (optional)
    QMenu* viewMenu = addMenu("View");
    // Add view-related actions here

    // Help Menu
    QMenu* helpMenu = addMenu("Help");
    QAction* aboutAction = helpMenu->addAction("About");
    // Connect about action to show about dialog
}

void AppMenuBar::onOpenFiles() {
    emit openFilesRequested();
}

void AppMenuBar::onOpenFolder() {
    emit openFolderRequested();
}

void AppMenuBar::onShowSettings() {
    emit showSettingsRequested();
}

void AppMenuBar::onQuit() {
    emit quitRequested();
}