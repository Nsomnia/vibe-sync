#pragma once
#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QMutex>
#include <memory>
#include <unordered_map>

// Debug categories for organized logging
enum class DebugCategory {
    Core,
    Audio,
    Visualization,
    UI,
    Preset,
    Playlist,
    Recording,
    Performance,
    Network,
    Plugin,
    System
};

// Debug levels with proper categorization
enum class DebugLevel {
    Trace = 0,   // Detailed execution flow
    Debug = 1,   // Debug information
    Info = 2,    // General information
    Warning = 3, // Warnings
    Error = 4,   // Errors
    Fatal = 5    // Fatal errors
};

// Performance metrics structure
struct PerformanceMetrics {
    QString name;
    qint64 startTime;
    qint64 endTime;
    QString details;
    double durationMs() const { return (endTime - startTime) / 1000000.0; }
};

// Memory usage tracking
struct MemoryStats {
    size_t currentBytes = 0;
    size_t peakBytes = 0;
    size_t allocationCount = 0;
    size_t deallocationCount = 0;
    QMap<QString, size_t> categoryUsage;
};

// Debug configuration
struct DebugConfig {
    bool enablePerformanceMetrics = true;
    bool enableMemoryTracking = true;
    bool enableFileLogging = false;
    bool enableNetworkDebugging = false;
    bool enableRemoteDebugging = false;
    QString logFilePath;
    QString networkDebugHost = "127.0.0.1";
    quint16 networkDebugPort = 4242;
    QMap<DebugCategory, DebugLevel> categoryLevels = {
        {DebugCategory::Core, DebugLevel::Info},
        {DebugCategory::Performance, DebugLevel::Debug},
        {DebugCategory::System, DebugLevel::Warning}
    };
};

class DebugManager : public QObject {
    Q_OBJECT
public:
    static DebugManager& instance();
    
    // Configuration
    void setConfig(const DebugConfig& config);
    DebugConfig getConfig() const { return m_config; }
    
    // Logging with categories
    void log(DebugCategory category, DebugLevel level, const QString& message, 
             const QString& function = QString(), const QString& file = QString(), int line = -1);
    
    // Performance tracking
    QString startTimer(const QString& name);
    void endTimer(const QString& timerId, const QString& details = QString());
    void measureFunction(const QString& functionName, std::function<void()> func);
    
    // Memory tracking
    void trackAllocation(size_t bytes, const QString& category = "Unknown");
    void trackDeallocation(size_t bytes, const QString& category = "Unknown");
    MemoryStats getMemoryStats() const;
    
    // Network debugging (for future remote features)
    void sendDebugPacket(const QByteArray& data);
    void enableNetworkDebugging(const QString& host, quint16 port);
    
    // Export and reporting
    QByteArray exportDebugReport() const;
    void saveDebugReport(const QString& filePath) const;
    void clearMetrics();

signals:
    void debugMessage(DebugCategory category, DebugLevel level, const QString& message);
    void performanceAlert(const QString& metricName, double durationMs);
    void memoryAlert(size_t currentBytes, size_t peakBytes);

private:
    DebugManager() {
        m_config = DebugConfig();
        setupTimers();
    }
    
    void setupTimers();
    void checkPerformanceAlerts();
    void checkMemoryAlerts();
    QString formatMessage(DebugCategory category, DebugLevel level, const QString& message,
                         const QString& function, const QString& file, int line);
    
    struct Impl;
    std::unique_ptr<Impl> pImpl;
    
    DebugConfig m_config;
    mutable QMutex m_mutex;
    
    // Active timers
    QMap<QString, PerformanceMetrics> m_activeTimers;
    QVector<PerformanceMetrics> m_completedTimers;
    
    // Memory tracking
    MemoryStats m_memoryStats;
    
    // Performance thresholds
    static constexpr double SLOW_FUNCTION_THRESHOLD_MS = 100.0;
    static constexpr size_t HIGH_MEMORY_THRESHOLD_BYTES = 100 * 1024 * 1024; // 100MB
};

// Convenience macros for different debug levels
#define DEBUG_TRACE(category, message) \
    DebugManager::instance().log(category, DebugLevel::Trace, message, __FUNCTION__, __FILE__, __LINE__)

#define DEBUG_DEBUG(category, message) \
    DebugManager::instance().log(category, DebugLevel::Debug, message, __FUNCTION__, __FILE__, __LINE__)

#define DEBUG_INFO(category, message) \
    DebugManager::instance().log(category, DebugLevel::Info, message, __FUNCTION__, __FILE__, __LINE__)

#define DEBUG_WARNING(category, message) \
    DebugManager::instance().log(category, DebugLevel::Warning, message, __FUNCTION__, __FILE__, __LINE__)

#define DEBUG_ERROR(category, message) \
    DebugManager::instance().log(category, DebugLevel::Error, message, __FUNCTION__, __FILE__, __LINE__)

#define DEBUG_FATAL(category, message) \
    DebugManager::instance().log(category, DebugLevel::Fatal, message, __FUNCTION__, __FILE__, __LINE__)

// Performance measurement macros
#define MEASURE_FUNCTION(funcName) \
    DebugManager::instance().measureFunction(funcName, [&]() { funcName(); })

#define MEASURE_SCOPE(varName) \
    auto varName = DebugManager::instance().startTimer(QObject::tr("%1 at %2:%3").arg(__FUNCTION__).arg(__FILE__).arg(__LINE__)); \
    struct ScopedTimer { \
        QString timerId; \
        DebugManager* manager; \
        ~ScopedTimer() { manager->endTimer(timerId); } \
    } varName##_scoped{varName, &DebugManager::instance()}