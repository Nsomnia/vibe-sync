#pragma once
#include <QObject>
#include <projectM-4/projectM.h>

class VizEngine : public QObject {
    Q_OBJECT
public:
    VizEngine(QObject* parent = nullptr);
    ~VizEngine();
    
    bool initialize(const QString& presetPath, int meshX = 32, int meshY = 24, int fps = 60);
    void cleanup();
    
    void loadPreset(const QString& presetPath);
    QString currentPreset() const { return m_currentPreset; }
    
    void setMeshSize(int x, int y);
    void setFPS(int fps);
    void setBeatSensitivity(float sensitivity);
    void setSmoothDuration(float duration);
    
    bool isInitialized() const { return m_handle != nullptr; }

signals:
    void presetLoaded(const QString& presetPath);
    void initializationFailed(const QString& error);

private:
    projectm_handle m_handle = nullptr;
    QString m_presetPath;
    QString m_currentPreset;
    
    bool setupProjectM(const QString& presetPath, int meshX, int meshY, int fps);
    void cleanupProjectM();
};