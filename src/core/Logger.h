#pragma once
#include <QObject>
#include <QString>
#include <QMutex>

class Logger : public QObject {
    Q_OBJECT
public:
    static Logger& instance() {
        static Logger s;
        return s;
    }

    // Called by the global Qt message handler
    void log(QtMsgType type, const QMessageLogContext& context, const QString& msg);

signals:
    void newLogMessage(const QString& formattedMsg, int logLevel);

private:
    Logger() {}
    QMutex m_mutex;
};