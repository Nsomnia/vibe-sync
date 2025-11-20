#pragma once
#include <QObject>
#include <QPainter>
#include <QMap>
#include <QMutex>

struct TextElement {
    QString id;           // Unique ID (e.g., "watermark", "artist")
    QString text;         // The content
    
    // Positioning (0.0 to 1.0 relative to screen size)
    // e.g., x=0.5, y=0.5 is dead center.
    float relX = 0.0f; 
    float relY = 0.0f; 
    
    // Appearance
    int baseFontSize = 24;// Size at 1080p
    QColor color = Qt::white;
    QString fontFamily = "Futura";
    bool visible = true;

    // Animation State
    bool enableBreathing = false;
    bool enableSlide = false;
    float animPhase = 0.0f;
};

class TextEngine : public QObject {
    Q_OBJECT
public:
    explicit TextEngine(QObject* parent = nullptr);

    // Add or Update an element
    void setElement(const QString& id, const TextElement& config);
    void updateText(const QString& id, const QString& newText);
    void setVisible(const QString& id, bool visible);

    // The Main Draw Call
    void render(QPainter* painter, const QSize& viewport);

    // Global Settings
    void setGlobalScale(float scale) { m_globalScale = scale; }
    void setDpiAwareness(bool enable) { m_dpiAware = enable; }

private:
    QMap<QString, TextElement> m_elements;
    float m_globalScale = 1.0f;
    bool m_dpiAware = true;
    QMutex m_mutex;
};