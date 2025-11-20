#pragma once
#include <QMenuBar>
#include <QAction>

class AppMenuBar : public QMenuBar {
    Q_OBJECT
public:
    explicit AppMenuBar(QWidget* parent = nullptr);

signals:
    void openFilesRequested();
    void openFolderRequested();
    void showSettingsRequested();
    void quitRequested();

private slots:
    void onOpenFiles();
    void onOpenFolder();
    void onShowSettings();
    void onQuit();

private:
    QAction* m_openFilesAction = nullptr;
    QAction* m_openFolderAction = nullptr;
    QAction* m_settingsAction = nullptr;
    QAction* m_quitAction = nullptr;
};