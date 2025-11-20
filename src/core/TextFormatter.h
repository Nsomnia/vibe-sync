#pragma once
#include <QString>
#include <QFileInfo>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include "StringUtils.h"

class TextFormatter {
public:
    struct TrackInfo {
        QString artist;
        QString title;
        QString displayString;
    };

    static TrackInfo parse(const QString& filePath) {
        TrackInfo info;
        
        // 1. TagLib
        TagLib::FileRef f(filePath.toStdString().c_str());
        if (!f.isNull() && f.tag()) {
            info.artist = QString::fromStdString(f.tag()->artist().to8Bit(true));
            info.title = QString::fromStdString(f.tag()->title().to8Bit(true));
        }

        // 2. Fallback & Cleaning
        if (info.title.isEmpty()) {
            QFileInfo fi(filePath);
            QString base = fi.completeBaseName();
            // USE UTILITY
            base = StringUtils::cleanTrackTitle(base);
            
            // Simple split logic
            QStringList parts = base.split("-");
            if (parts.size() >= 2) {
                info.artist = parts[0].trimmed();
                info.title = parts[1].trimmed();
            } else {
                info.title = base.trimmed();
                info.artist = "Unknown Artist";
            }
        }

        // 3. Formatting
        // USE UTILITY for Word Wrap
        info.title = StringUtils::wordWrap(info.title, 25);
        info.displayString = info.artist + "\n" + info.title;

        return info;
    }
};