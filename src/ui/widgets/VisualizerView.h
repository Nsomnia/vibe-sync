#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <projectM-4/projectM.h>
#include "../../engine/TextEngine.h"
#include "../../engine/VideoRecorder.h"

class VisualizerView : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit VisualizerView(QWidget* parent = nullptr);
    ~VisualizerView();

    void loadPreset(const QString& path);
    
    // New Text API
    TextEngine* textEngine() { return m_textEngine; }
    
    void setRecorder(VideoRecorder* rec) { m_recorder = rec; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    projectm_handle m_handle = nullptr;
    QTimer* m_timer;
    TextEngine* m_textEngine;
    VideoRecorder* m_recorder = nullptr;
};