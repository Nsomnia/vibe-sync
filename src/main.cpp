#include <QApplication>
#include "ui/MainWindow.h"
#include "core/Logger.h"
#include <QPalette>
#include <QStyleFactory>
#include <QDir>

// The Global Hook Function
void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    Logger::instance().log(type, context, msg);
}

int main(int argc, char *argv[]) {
    // Install the interceptor immediately
    qInstallMessageHandler(messageHandler);

    QApplication app(argc, argv);
    
    // --- Dark Theme (The "Chad" Palette) ---
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette p;
    p.setColor(QPalette::Window, QColor(53, 53, 53));
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Base, QColor(25, 25, 25));
    p.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    p.setColor(QPalette::ToolTipBase, Qt::white);
    p.setColor(QPalette::ToolTipText, Qt::white);
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Button, QColor(53, 53, 53));
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::BrightText, Qt::red);
    p.setColor(QPalette::Link, QColor(42, 130, 218));
    p.setColor(QPalette::Highlight, QColor(42, 130, 218));
    p.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(p);

    MainWindow window;
    window.show();

    qDebug() << "🚀 NeonVisualizer Initialized. Debug interceptor active.";
    qDebug() << "   System: " << QSysInfo::prettyProductName();
    qDebug() << "   Qt Version: " << qVersion();

    return app.exec();
}