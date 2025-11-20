#pragma once
#include <QString>
#include <QRegularExpression>

class StringUtils {
public:
    // Sanitize for filesystem (VideoRecorder usage)
    static QString safeFilename(const QString& input) {
        QString s = input;
        // Allow only alphanumeric, underscores, and dashes
        return s.replace(QRegularExpression("[^a-zA-Z0-9_\\-]"), "");
    }

    // Clean up song metadata (TextFormatter usage)
    static QString cleanTrackTitle(const QString& input) {
        QString s = input;
        s.replace("_", " ");
        // Remove common junk like "(Official Video)", "[HQ]", etc.
        s.remove(QRegularExpression("\\s*\\(.*?\\)")); 
        s.remove(QRegularExpression("\\s*\\[.*?\\]"));
        return s.trimmed();
    }

    // Intelligent newline insertion for long text
    static QString wordWrap(const QString& input, int maxChars = 20) {
        if (input.length() <= maxChars) return input;
        
        int mid = input.length() / 2;
        int space = input.indexOf(' ', mid - 5); // Look for space near middle
        if (space == -1) space = input.indexOf(' ', mid); // Look forward if not backward

        if (space != -1) {
            return input.left(space) + "\n" + input.mid(space + 1);
        }
        return input;
    }
};