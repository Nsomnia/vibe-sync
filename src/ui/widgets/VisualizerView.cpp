#include "VisualizerView.h"

VisualizerView::VisualizerView(QWidget* parent) : QOpenGLWidget(parent) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&VisualizerView::update));
    m_timer->start(16);
    
    m_textEngine = new TextEngine(this);
    
    // Initialize Default Elements
    TextElement wm;
    wm.id = "watermark";
    wm.text = "vibe-sync"; // Default
    wm.relX = 0.90f; // Bottom Right approx
    wm.relY = 0.95f;
    wm.baseFontSize = 18;
    wm.color = QColor(200, 200, 200, 100); // Semi-transparent
    wm.visible = false; // Hidden by default
    m_textEngine->setElement("watermark", wm);
    
    TextElement meta;
    meta.id = "metadata";
    meta.relX = 0.05f; // Top Left
    meta.relY = 0.10f;
    meta.baseFontSize = 32;
    meta.enableBreathing = true;
    m_textEngine->setElement("metadata", meta);
}

VisualizerView::~VisualizerView() {
    if (m_handle) {
        projectm_destroy(m_handle);
    }
}

void VisualizerView::initializeGL() {
    initializeOpenGLFunctions();
    
    // Note: ProjectM initialization should be done with proper settings
    // This is a placeholder - actual initialization should happen when a preset is loaded
}

void VisualizerView::resizeGL(int w, int h) {
    if (m_handle) {
        projectm_reset_textures(m_handle);
    }
}

void VisualizerView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (m_handle) {
        projectm_opengl_render_frame(m_handle);
    }

    // RENDER TEXT ENGINE
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    
    // Pass the current viewport size for scaling calculations
    m_textEngine->render(&painter, size());
    
    painter.end();

    if (m_recorder && m_recorder->isRecording()) {
        m_recorder->writeFrame(grabFramebuffer());
    }
}

void VisualizerView::loadPreset(const QString& path) {
    if (QFile::exists(path)) {
        // For now, just store the path. Full ProjectM integration would go here
        qDebug() << "📁 Loading preset:" << QFileInfo(path).fileName();
    }
}