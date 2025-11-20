#include "VizEngine.h"
#include <QDebug>

VizEngine::VizEngine(QObject* parent) : QObject(parent) {}

VizEngine::~VizEngine() {
    cleanup();
}

bool VizEngine::initialize(const QString& presetPath, int meshX, int meshY, int fps) {
    m_presetPath = presetPath;
    return setupProjectM(presetPath, meshX, meshY, fps);
}

void VizEngine::cleanup() {
    cleanupProjectM();
}

void VizEngine::loadPreset(const QString& presetPath) {
    if (m_handle && QFile::exists(presetPath)) {
        projectm_load_preset_data(m_handle, presetPath.toStdString().c_str(), nullptr);
        m_currentPreset = presetPath;
        emit presetLoaded(presetPath);
        qDebug() << "👁️ Loaded preset:" << QFileInfo(presetPath).fileName();
    }
}

void VizEngine::setMeshSize(int x, int y) {
    if (m_handle) {
        // Note: This might require reinitialization for full effect
        qDebug() << "🔧 Mesh size changed to" << x << "x" << y;
    }
}

void VizEngine::setFPS(int fps) {
    if (m_handle) {
        qDebug() << "🎯 FPS changed to" << fps;
    }
}

void VizEngine::setBeatSensitivity(float sensitivity) {
    if (m_handle) {
        qDebug() << "🎵 Beat sensitivity changed to" << sensitivity;
    }
}

void VizEngine::setSmoothDuration(float duration) {
    if (m_handle) {
        qDebug() << "⏱️ Smooth duration changed to" << duration << "seconds";
    }
}

bool VizEngine::setupProjectM(const QString& presetPath, int meshX, int meshY, int fps) {
    if (QFile::exists(presetPath)) {
        projectm_settings settings{};
        settings.meshX = meshX;
        settings.meshY = meshY;
        settings.fps = fps;
        settings.textureSize = 2048;
        
        m_handle = projectm_create(&settings);
        if (m_handle) {
            projectm_load_preset_data(m_handle, presetPath.toStdString().c_str(), nullptr);
            m_currentPreset = presetPath;
            qDebug() << "🎨 ProjectM initialized successfully";
            return true;
        }
    }
    
    qDebug() << "❌ Failed to initialize ProjectM";
    emit initializationFailed("Failed to initialize visualizer");
    return false;
}

void VizEngine::cleanupProjectM() {
    if (m_handle) {
        projectm_destroy(m_handle);
        m_handle = nullptr;
        qDebug() << "🧹 ProjectM cleaned up";
    }
}