#pragma once
#include <QString>
#include <QStandardPaths>

class PathUtils {
public:
    static QString getPresetPath() {
        QStringList candidates = {
            "/usr/share/projectM/presets",
            "/usr/local/share/projectM/presets", 
            QDir::homePath() + "/.config/projectM/presets",
            QDir::homePath() + "/projectM/presets"
        };
        
        for(const QString& path : candidates) {
            if(QDir(path).exists()) return path;
        }
        
        return QString(); // No presets found
    }
    
    static QString getConfigPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    }
    
    static QString getDataPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
};