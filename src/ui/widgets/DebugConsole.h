#pragma once
#include <QTextEdit>
#include <QScrollBar>
#include "../../core/Logger.h"

class DebugConsole : public QTextEdit {
    Q_OBJECT
public:
    DebugConsole(QWidget* parent = nullptr) : QTextEdit(parent) {
        setReadOnly(true);
        setStyleSheet("background-color: #1e1e1e; font-family: Monospace; font-size: 11px;");
        
        // Connect to the Singleton Logger
        connect(&Logger::instance(), &Logger::newLogMessage, this, &DebugConsole::appendLog);
    }

public slots:
    void appendLog(const QString& msg, int level) {
        QString color = "#cccccc"; // Gray
        if (level == 1) color = "#aaffaa"; // Green (Info)
        if (level == 2) color = "#ffaa00"; // Orange (Warn)
        if (level == 3) color = "#ff5555"; // Red (Critical)

        QString html = QString("<span style='color:%1'>%2</span>").arg(color, msg);
        append(html);
        
        // Auto-scroll
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    }
};