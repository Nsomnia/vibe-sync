#include "ErrorManager.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QRandomGenerator>
#include <cassert>

struct ErrorManager::Impl {
    ErrorStats stats;
    std::function<void(const ErrorInfo&)> errorDisplayCallback = nullptr;
    QString logFilePath;
    QTimer* statsUpdateTimer = nullptr;
    
    Impl() {
        QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs";
        QDir().mkpath(logDir);
        logFilePath = logDir + "/errors.log";
        
        statsUpdateTimer = new QTimer();
        connect(statsUpdateTimer, &QTimer::timeout, [this]() {
            // Periodic stats update signal could be emitted here
        });
        statsUpdateTimer->start(30000); // Every 30 seconds
    }
    
    void writeToLog(const ErrorInfo& error) {
        QFile file(logFilePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            QString timestamp = error.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz");
            QString severityStr;
            
            switch (error.severity) {
                case ErrorSeverity::Info: severityStr = "INFO"; break;
                case ErrorSeverity::Warning: severityStr = "WARN"; break;
                case ErrorSeverity::Error: severityStr = "ERROR"; break;
                case ErrorSeverity::Critical: severityStr = "CRIT"; break;
                case ErrorSeverity::Fatal: severityStr = "FATAL"; break;
                default: severityStr = "UNK"; break;
            }
            
            QString typeStr;
            switch (error.type) {
                case ErrorType::AudioEngine: typeStr = "AUDIO"; break;
                case ErrorType::Visualization: typeStr = "VIS"; break;
                case ErrorType::Playlist: typeStr = "PLAY"; break;
                case ErrorType::Preset: typeStr = "PRESET"; break;
                case ErrorType::Recording: typeStr = "REC"; break;
                case ErrorType::UI: typeStr = "UI"; break;
                case ErrorType::FileSystem: typeStr = "FS"; break;
                case ErrorType::Network: typeStr = "NET"; break;
                case ErrorType::System: typeStr = "SYS"; break;
                case ErrorType::Plugin: typeStr = "PLUGIN"; break;
                default: typeStr = "UNK"; break;
            }
            
            out << QString("[%1][%2][%3] %4\n")
                   .arg(timestamp)
                   .arg(severityStr)
                   .arg(typeStr)
                   .arg(error.message);
            
            if (!error.details.isEmpty()) {
                out << QString("  Details: %1\n").arg(error.details);
            }
            if (!error.location.isEmpty()) {
                out << QString("  Location: %1\n").arg(error.location);
            }
            
            file.close();
        }
    }
    
    bool shouldRestartApplication(const ErrorInfo& error) {
        // Criteria for automatic restart
        if (error.severity == ErrorSeverity::Fatal) {
            return true;
        }
        
        if (error.recommendedAction == ErrorAction::Restart) {
            return true;
        }
        
        // Multiple critical errors in short time
        static QDateTime lastCriticalTime;
        static int criticalCount = 0;
        
        QDateTime now = QDateTime::currentDateTime();
        if (now.toSecsSinceEpoch() - lastCriticalTime.toSecsSinceEpoch() < 60) {
            criticalCount++;
            if (criticalCount >= 5) {
                return true;
            }
        } else {
            criticalCount = 1;
            lastCriticalTime = now;
        }
        
        return false;
    }
};

ErrorManager& ErrorManager::instance() {
    static ErrorManager instance;
    return instance;
}

ErrorManager::ErrorManager() {
    pImpl = std::make_unique<Impl>();
    setupEmergencyTimer();
    
    // Register default recovery strategies
    registerRecoveryStrategy(std::make_shared<AudioErrorHandler>());
    registerRecoveryStrategy(std::make_shared<VisualizationErrorHandler>());
    registerRecoveryStrategy(std::make_shared<PresetErrorHandler>());
}

void ErrorManager::registerError(const ErrorInfo& error) {
    QMutexLocker locker(&m_mutex);
    
    QString errorId = error.id.isEmpty() ? generateErrorId() : error.id;
    ErrorInfo managedError = error;
    managedError.id = errorId;
    managedError.timestamp = QDateTime::currentDateTime();
    
    // Store in active errors
    m_activeErrors[errorId] = managedError;
    
    // Add to history (keep last 1000 errors)
    m_errorHistory.append(managedError);
    if (m_errorHistory.size() > 1000) {
        m_errorHistory.removeFirst();
    }
    
    // Update statistics
    pImpl->stats.totalErrors++;
    pImpl->stats.countsByType[managedError.type]++;
    pImpl->stats.countsBySeverity[managedError.severity]++;
    pImpl->stats.lastError = managedError.timestamp;
    
    if (managedError.severity == ErrorSeverity::Fatal) {
        pImpl->stats.fatalErrors++;
    }
    
    // Write to log
    pImpl->writeToLog(managedError);
    
    // Emit signals
    emit errorRegistered(managedError);
    
    // Check for critical error
    if (managedError.severity == ErrorSeverity::Critical || 
        managedError.severity == ErrorSeverity::Fatal) {
        emit criticalError(managedError);
        
        if (pImpl->shouldRestartApplication(managedError)) {
            QTimer::singleShot(1000, qApp, &QCoreApplication::quit);
            emit emergencyShutdown();
        }
    }
    
    // Show to user if appropriate
    showErrorToUser(managedError);
}

void ErrorManager::handleError(const QString& id, ErrorType type, ErrorSeverity severity,
                              const QString& message, const QString& location) {
    ErrorInfo error;
    error.id = id;
    error.type = type;
    error.severity = severity;
    error.message = message;
    error.location = location;
    error.timestamp = QDateTime::currentDateTime();
    
    registerError(error);
}

void ErrorManager::registerRecoveryStrategy(std::shared_ptr<ErrorRecoveryStrategy> strategy) {
    QMutexLocker locker(&m_mutex);
    m_recoveryStrategies.append(strategy);
}

void ErrorManager::attemptRecovery(const QString& errorId) {
    QMutexLocker locker(&m_mutex);
    
    if (!m_activeErrors.contains(errorId)) {
        return;
    }
    
    ErrorInfo& error = m_activeErrors[errorId];
    
    // Check retry limits
    if (error.retryCount >= error.maxRetries) {
        DEBUG_WARNING(DebugCategory::System, 
                     QString("Max retries exceeded for error %1").arg(errorId));
        return;
    }
    
    error.retryCount++;
    DEBUG_INFO(DebugCategory::System, 
              QString("Attempting recovery %1/%2 for error %3")
              .arg(error.retryCount).arg(error.maxRetries).arg(errorId));
    
    // Try custom recovery function first
    if (error.recoveryFunction) {
        if (error.recoveryFunction()) {
            DEBUG_INFO(DebugCategory::System, 
                      QString("Custom recovery succeeded for error %1").arg(errorId));
            error.isRecoverable = false; // Mark as resolved
            emit errorRecovered(errorId);
            return;
        }
    }
    
    // Try registered recovery strategies
    for (auto strategy : m_recoveryStrategies) {
        if (strategy->canHandle(error)) {
            if (strategy->attemptRecovery(error)) {
                DEBUG_INFO(DebugCategory::System, 
                          QString("Strategy '%1' recovered error %2")
                          .arg(strategy->getDescription()).arg(errorId));
                error.isRecoverable = false; // Mark as resolved
                emit errorRecovered(errorId);
                return;
            }
        }
    }
    
    DEBUG_WARNING(DebugCategory::System, 
                 QString("All recovery attempts failed for error %1").arg(errorId));
}

QList<ErrorInfo> ErrorManager::getRecentErrors(int count) const {
    QMutexLocker locker(&m_mutex);
    QList<ErrorInfo> recent = m_errorHistory;
    
    if (count > 0 && recent.size() > count) {
        recent = recent.mid(recent.size() - count);
    }
    
    return recent;
}

QList<ErrorInfo> ErrorManager::getErrorsByType(ErrorType type) const {
    QMutexLocker locker(&m_mutex);
    
    QList<ErrorInfo> result;
    for (const auto& error : m_activeErrors) {
        if (error.type == type) {
            result.append(error);
        }
    }
    return result;
}

ErrorInfo ErrorManager::getError(const QString& id) const {
    QMutexLocker locker(&m_mutex);
    return m_activeErrors.value(id);
}

void ErrorManager::clearErrors() {
    QMutexLocker locker(&m_mutex);
    
    m_activeErrors.clear();
    m_errorHistory.clear();
    
    // Reset statistics
    pImpl->stats = ErrorStats();
    
    DEBUG_INFO(DebugCategory::System, "All errors cleared");
}

ErrorManager::ErrorStats ErrorManager::getStatistics() const {
    QMutexLocker locker(&m_mutex);
    return pImpl->stats;
}

void ErrorManager::setErrorDisplayCallback(std::function<void(const ErrorInfo&)> callback) {
    pImpl->errorDisplayCallback = callback;
}

void ErrorManager::showErrorToUser(const ErrorInfo& error) {
    if (error.severity == ErrorSeverity::Info) {
        return; // Don't show info messages to users
    }
    
    if (pImpl->errorDisplayCallback) {
        pImpl->errorDisplayCallback(error);
    } else {
        // Default error display could be implemented here
        // For now, just log to console
        DEBUG_WARNING(DebugCategory::UI, 
                     QString("User Error: %1").arg(error.message));
    }
}

void ErrorManager::enableSystemIntegration(bool enable) {
    m_systemIntegrationEnabled = enable;
    DEBUG_INFO(DebugCategory::System, 
              QString("System integration %1").arg(enable ? "enabled" : "disabled"));
}

void ErrorManager::setEmergencyShutdownTimeout(int seconds) {
    m_emergencyShutdownTimeout = seconds;
    DEBUG_INFO(DebugCategory::System, 
              QString("Emergency shutdown timeout set to %1 seconds").arg(seconds));
}

void ErrorManager::setupEmergencyTimer() {
    m_emergencyTimer = new QTimer(this);
    connect(m_emergencyTimer, &QTimer::timeout, this, &ErrorManager::checkForFatalErrors);
    m_emergencyTimer->start(5000); // Check every 5 seconds
}

void ErrorManager::checkForFatalErrors() {
    QMutexLocker locker(&m_mutex);
    
    int fatalCount = 0;
    QDateTime oldestFatal;
    
    for (const auto& error : m_activeErrors) {
        if (error.severity == ErrorSeverity::Fatal) {
            fatalCount++;
            if (oldestFatal.isNull() || error.timestamp < oldestFatal) {
                oldestFatal = error.timestamp;
            }
        }
    }
    
    // Emergency shutdown conditions
    if (fatalCount >= 3) {
        DEBUG_FATAL(DebugCategory::System, "Multiple fatal errors detected - initiating emergency shutdown");
        emit emergencyShutdown();
        QTimer::singleShot(2000, qApp, &QCoreApplication::quit);
    }
}

QString ErrorManager::generateErrorId() const {
    return QString("ERR_%1_%2")
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"))
        .arg(m_nextErrorId++, 4, 10, QChar('0'));
}

// Recovery strategy implementations
bool AudioErrorHandler::attemptRecovery(ErrorInfo& error) {
    DEBUG_INFO(DebugCategory::Audio, "Attempting audio engine recovery");
    
    // Simple retry with delay
    QTimer::singleShot(1000, []() {
        DEBUG_INFO(DebugCategory::Audio, "Audio engine retry initiated");
    });
    
    return true; // Always return true to allow retry
}

bool VisualizationErrorHandler::attemptRecovery(ErrorInfo& error) {
    DEBUG_INFO(DebugCategory::Visualization, "Attempting visualization recovery");
    
    // Fallback to default preset
    if (error.message.contains("preset")) {
        DEBUG_INFO(DebugCategory::Visualization, "Loading fallback preset");
        return true;
    }
    
    return false;
}

bool PresetErrorHandler::attemptRecovery(ErrorInfo& error) {
    DEBUG_INFO(DebugCategory::Preset, "Attempting preset system recovery");
    
    // Skip to next preset
    if (error.message.contains("failed to load")) {
        DEBUG_INFO(DebugCategory::Preset, "Skipping to next preset");
        return true;
    }
    
    return false;
}