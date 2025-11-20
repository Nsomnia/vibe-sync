#include "TextEngine.h"
#include <cmath>
#include <QDebug>

TextEngine::TextEngine(QObject* parent) : QObject(parent) {}

void TextEngine::setElement(const QString& id, const TextElement& config) {
    QMutexLocker locker(&m_mutex);
    m_elements[id] = config;
    m_elements[id].id = id;
}

void TextEngine::updateText(const QString& id, const QString& newText) {
    QMutexLocker locker(&m_mutex);
    if (m_elements.contains(id)) {
        m_elements[id].text = newText;
    }
}

void TextEngine::setVisible(const QString& id, bool visible) {
    QMutexLocker locker(&m_mutex);
    if (m_elements.contains(id)) {
        m_elements[id].visible = visible;
    }
}

void TextEngine::render(QPainter* painter, const QSize& viewport) {
    QMutexLocker locker(&m_mutex);

    // Reference Height (1080p)
    const float refHeight = 1080.0f;
    float scaleFactor = (float)viewport.height() / refHeight;
    
    // Apply User Global Scale
    scaleFactor *= m_globalScale;

    static float time = 0.0f;
    time += 0.05f;

    for (auto& el : m_elements) {
        if (!el.visible || el.text.isEmpty()) continue;

        painter->save();

        // 1. Calculate Position
        float x = el.relX * viewport.width();
        float y = el.relY * viewport.height();

        // 2. Calculate Font Size
        float finalSize = el.baseFontSize * scaleFactor;
        QFont font(el.fontFamily);
        font.setPixelSize(std::max(8, (int)finalSize)); // Min size 8px
        painter->setFont(font);

        // 3. Apply Animation Effects
        QColor finalColor = el.color;
        float drawScale = 1.0f;

        if (el.enableBreathing) {
            float breath = (std::sin(time + (x * 0.01f)) + 1.0f) * 0.5f;
            int alpha = 180 + (int)(breath * 75); // 180-255
            finalColor.setAlpha(std::min(255, alpha));
            drawScale = 1.0f + (breath * 0.02f);
        }

        painter->setPen(finalColor);

        // 4. Draw
        painter->translate(x, y);
        painter->scale(drawScale, drawScale);
        
        // Measure text for alignment (Center alignment logic)
        QRect bounds = painter->fontMetrics().boundingRect(QRect(0,0,0,0), Qt::AlignCenter, el.text);
        // We draw centered on the coordinate
        painter->drawText(-bounds.width()/2, -bounds.height()/2, el.text);

        painter->restore();
    }
}