#include "Logger.h"
#include <QDateTime>
#include <iostream>

void Logger::log(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    QMutexLocker locker(&m_mutex);

    QString time = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    QString level;
    int colorCode = 0; // 0=Debug, 1=Info, 2=Warning, 3=Critical

    switch (type) {
    case QtDebugMsg:    level = "[DBG]"; colorCode = 0; break;
    case QtInfoMsg:     level = "[INF]"; colorCode = 1; break;
    case QtWarningMsg:  level = "[WRN]"; colorCode = 2; break;
    case QtCriticalMsg: level = "[CRT]"; colorCode = 3; break;
    case QtFatalMsg:    level = "[FTL]"; colorCode = 3; break;
    }

    // Format: 12:00:00.000 [DBG] Message (File:Line)
    QString formatted = QString("%1 %2 %3").arg(time, level, msg);
    
    // Also dump to stdout for terminal users
    std::cout << formatted.toStdString() << std::endl;

    emit newLogMessage(formatted, colorCode);
}