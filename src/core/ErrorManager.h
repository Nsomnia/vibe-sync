#pragma once
#include <QObject>
#include <QException>
#include <QTimer>
#include <QMap>
#include <QList>
#include <functional>
#include <atomic>

// Error types and categories
enum class ErrorType {
    AudioEngine,
    Visualization,
    Playlist,
    Preset,
    Recording,
    UI,
    FileSystem,
    Network,
    System,
    Plugin
};

enum class ErrorSeverity {
    Info,
    Warning,
    Error,
    Critical,
    Fatal
};

enum class ErrorAction {
    None,
    Retry,
    Fallback,
    Restart,
    Shutdown,
    Ignore
};

// Error information structure
struct ErrorInfo {
    QString id;
    ErrorType type;
    ErrorSeverity severity;
    QString message;
    QString details;
    QString location; // Function/file/line
    QDateTime timestamp;
    int retryCount = 0;
    int maxRetries = 3;
    bool isRecoverable = true;
    ErrorAction recommendedAction = ErrorAction::Retry;
    
    // Context information
    QMap<QString, QVariant> context;
    
    // Recovery data
    std::function<bool()> recoveryFunction = nullptr;
};

// Error recovery strategies
class ErrorRecoveryStrategy {
public:
    virtual ~ErrorRecoveryStrategy() = default;
    virtual bool canHandle(const ErrorInfo& error) const = 0;
    virtual bool attemptRecovery(ErrorInfo& error) = 0;
    virtual QString getDescription() const = 0;
};

// Global error handler that manages recovery
class ErrorManager : public QObject {
    Q_OBJECT
public:
    static ErrorManager& instance();
    
    // Error registration and handling
    void registerError(const ErrorInfo& error);
    void handleError(const QString& id, ErrorType type, ErrorSeverity severity, 
                    const QString& message, const QString& location = QString());
    
    // Recovery strategies
    void registerRecoveryStrategy(std::shared_ptr<ErrorRecoveryStrategy> strategy);
    void attemptRecovery(const QString& errorId);
    
    // Error querying and management
    QList<ErrorInfo> getRecentErrors(int count = 50) const;
    QList<ErrorInfo> getErrorsByType(ErrorType type) const;
    ErrorInfo getError(const QString& id) const;
    void clearErrors();
    
    // Statistics
    struct ErrorStats {
        QMap<ErrorType, int> countsByType;
        QMap<ErrorSeverity, int> countsBySeverity;
        int totalErrors = 0;
        int recoveredErrors = 0;
        int fatalErrors = 0;
        QDateTime lastError;
    };
    
    ErrorStats getStatistics() const;
    
    // User interface integration
    void setErrorDisplayCallback(std::function<void(const ErrorInfo&)> callback);
    void showErrorToUser(const ErrorInfo& error);
    
    // System integration
    void enableSystemIntegration(bool enable);
    void setEmergencyShutdownTimeout(int seconds);
    
signals:
    void errorRegistered(const ErrorInfo& error);
    void errorRecovered(const QString& errorId);
    void criticalError(const ErrorInfo& error);
    void emergencyShutdown();
    void errorStatsChanged(const ErrorStats& stats);

private:
    ErrorManager();
    
    void setupEmergencyTimer();
    void checkForFatalErrors();
    QString generateErrorId() const;
    
    struct Impl;
    std::unique_ptr<Impl> pImpl;
    
    QMap<QString, ErrorInfo> m_activeErrors;
    QList<ErrorInfo> m_errorHistory;
    QList<std::shared_ptr<ErrorRecoveryStrategy>> m_recoveryStrategies;
    
    mutable QMutex m_mutex;
    std::atomic<int> m_nextErrorId{0};
    
    QTimer* m_emergencyTimer = nullptr;
    int m_emergencyShutdownTimeout = 30; // seconds
    bool m_systemIntegrationEnabled = false;
};

// Convenience macros for error handling
#define HANDLE_ERROR(type, severity, message) \
    ErrorManager::instance().handleError( \
        QString(), type, severity, message, \
        QString("%1:%2").arg(__FILE__).arg(__LINE__) \
    )

#define HANDLE_ERROR_WITH_CONTEXT(type, severity, message, context) \
    { \
        ErrorInfo error; \
        error.id = QString(); \
        error.type = type; \
        error.severity = severity; \
        error.message = message; \
        error.location = QString("%1:%2").arg(__FILE__).arg(__LINE__); \
        error.context = context; \
        ErrorManager::instance().registerError(error); \
    }

#define HANDLE_RECOVERABLE_ERROR(type, message, recoveryFunc) \
    { \
        ErrorInfo error; \
        error.type = type; \
        error.severity = ErrorSeverity::Error; \
        error.message = message; \
        error.location = QString("%1:%2").arg(__FILE__).arg(__LINE__); \
        error.recoveryFunction = recoveryFunc; \
        error.isRecoverable = true; \
        ErrorManager::instance().registerError(error); \
    }

// Specific error handling classes
class AudioErrorHandler : public ErrorRecoveryStrategy {
public:
    bool canHandle(const ErrorInfo& error) const override {
        return error.type == ErrorType::AudioEngine;
    }
    
    bool attemptRecovery(ErrorInfo& error) override;
    QString getDescription() const override { return "Audio System Recovery"; }
};

class VisualizationErrorHandler : public ErrorRecoveryStrategy {
public:
    bool canHandle(const ErrorInfo& error) const override {
        return error.type == ErrorType::Visualization;
    }
    
    bool attemptRecovery(ErrorInfo& error) override;
    QString getDescription() const override { return "Visualization Recovery"; }
};

class PresetErrorHandler : public ErrorRecoveryStrategy {
public:
    bool canHandle(const ErrorInfo& error) const override {
        return error.type == ErrorType::Preset;
    }
    
    bool attemptRecovery(ErrorInfo& error) override;
    QString getDescription() const override { return "Preset System Recovery"; }
};

// Runtime assertions for debug builds
#ifdef QT_DEBUG
#define RUNTIME_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            HANDLE_ERROR(ErrorType::System, ErrorSeverity::Fatal, \
                        QString("Assertion failed: %1").arg(message)); \
            Q_ASSERT(false); \
        } \
    } while (0)

#define RUNTIME_CHECK_PTR(ptr, message) \
    RUNTIME_ASSERT(ptr != nullptr, QString("%1 - Pointer is null").arg(message))
#else
#define RUNTIME_ASSERT(condition, message) do {} while (0)
#define RUNTIME_CHECK_PTR(ptr, message) do {} while (0)
#endif