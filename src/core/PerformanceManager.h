#pragma once
#include <QObject>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <QMutex>
#include <QThread>
#include <QElapsedTimer>
#include <atomic>
#include <functional>

// Performance monitoring types
struct PerformanceSnapshot {
    QString component;
    double cpuUsagePercent = 0.0;
    size_t memoryUsageBytes = 0;
    int activeThreads = 0;
    int audioBufferSize = 0;
    int videoFrameRate = 0;
    int presetLoadTimeMs = 0;
    QDateTime timestamp;
};

struct OptimizationRecommendation {
    QString id;
    QString title;
    QString description;
    QString category;
    double expectedImprovement = 0.0; // percentage
    bool isSafe = true;
    bool requiresRestart = false;
    std::function<void()> applyFunction = nullptr;
};

class PerformanceMonitor : public QObject {
    Q_OBJECT
public:
    static PerformanceMonitor& instance();
    
    // Monitoring control
    void startMonitoring(int intervalMs = 1000);
    void stopMonitoring();
    bool isMonitoring() const { return m_isMonitoring; }
    
    // Component performance tracking
    void recordComponentMetrics(const QString& component, double cpu, size_t memory, int threads = 1);
    void recordAudioMetrics(int bufferSize, int sampleRate, int channels);
    void recordVideoMetrics(int frameRate, int resolution, double renderTimeMs);
    void recordPresetMetrics(const QString& presetPath, int loadTimeMs);
    
    // Alert system
    void setAlertThresholds(double highCpuPercent = 80.0, size_t highMemoryMB = 512, 
                           int lowFrameRate = 30);
    void enableAlerts(bool enable);
    
    // Performance snapshots and history
    PerformanceSnapshot getCurrentSnapshot() const;
    QVector<PerformanceSnapshot> getHistory(int minutes = 10) const;
    void clearHistory();
    
    // Optimization recommendations
    QVector<OptimizationRecommendation> generateRecommendations() const;
    void applyRecommendation(const QString& recommendationId);
    
    // Resource management
    void optimizeMemory();
    void optimizeCpuUsage();
    void cleanupUnusedResources();
    
    // Statistics
    struct PerformanceStats {
        double avgCpuUsage = 0.0;
        double peakCpuUsage = 0.0;
        size_t avgMemoryUsage = 0;
        size_t peakMemoryUsage = 0;
        int frameRate = 0;
        double audioLatencyMs = 0.0;
        int activeComponents = 0;
    };
    
    PerformanceStats getStatistics() const;
    
signals:
    void performanceSnapshot(const PerformanceSnapshot& snapshot);
    void performanceAlert(const QString& alertType, const QString& message, double value);
    void optimizationRecommended(const OptimizationRecommendation& recommendation);

private:
    PerformanceMonitor();
    
    void collectSystemMetrics();
    void checkPerformanceAlerts();
    void analyzePatterns();
    void emitRecommendation(const OptimizationRecommendation& rec);
    
    mutable QMutex m_mutex;
    QTimer* m_monitoringTimer = nullptr;
    bool m_isMonitoring = false;
    
    // Alert thresholds
    double m_highCpuThreshold = 80.0;
    size_t m_highMemoryThreshold = 512 * 1024 * 1024; // 512MB
    int m_lowFrameRateThreshold = 30;
    bool m_alertsEnabled = true;
    
    // Performance data
    QVector<PerformanceSnapshot> m_history;
    QMap<QString, QVector<double>> m_cpuHistory;
    QMap<QString, QVector<size_t>> m_memoryHistory;
    
    // Current metrics
    PerformanceSnapshot m_currentSnapshot;
    PerformanceStats m_stats;
    
    // System information
    int m_cpuCores = 0;
    size_t m_totalMemory = 0;
};

// Advanced resource manager for memory and CPU optimization
class ResourceManager : public QObject {
    Q_OBJECT
public:
    static ResourceManager& instance();
    
    // Memory management
    void setMemoryLimits(size_t maxBytes);
    void registerMemorySource(const QString& name, std::function<size_t()> getUsage);
    void enableMemoryCompression(bool enable);
    void enableMemoryPooling(bool enable);
    
    // CPU management
    void setCpuPriority(QThread* thread, int priority); // -20 to 19
    void limitThreadCount(int maxThreads);
    void enableCpuAffinity(int coreId);
    
    // Resource pooling
    template<typename T>
    std::shared_ptr<T> acquireResource(const QString& poolName, std::function<std::shared_ptr<T>()> createFunc);
    template<typename T>
    void releaseResource(const QString& poolName, std::shared_ptr<T> resource);
    
    // Cleanup and optimization
    void scheduleCleanup();
    void forceCleanup();
    void optimizeForGaming();
    void optimizeForRecording();
    
signals:
    void memoryPressure(size_t currentBytes, size_t limitBytes);
    void cpuThrottling(int throttlingPercent);

private:
    ResourceManager();
    
    void checkMemoryPressure();
    void performCleanup();
    
    size_t m_maxMemoryBytes = 1024 * 1024 * 1024; // 1GB default
    bool m_memoryCompressionEnabled = false;
    bool m_memoryPoolingEnabled = false;
    
    QMap<QString, std::function<size_t()>> m_memorySources;
    QMap<QString, QList<std::weak_ptr<void>>> m_resourcePools;
    QTimer* m_cleanupTimer = nullptr;
    QTimer* m_pressureTimer = nullptr;
    
    mutable QMutex m_mutex;
};

// Performance profiling for detailed analysis
class PerformanceProfiler : public QObject {
    Q_OBJECT
public:
    static PerformanceProfiler& instance();
    
    // Function profiling
    QString beginProfiling(const QString& functionName);
    void endProfiling(const QString& profileId, const QString& category = "General");
    
    // Scope-based profiling
    class ScopedProfiler {
    public:
        ScopedProfiler(const QString& name, PerformanceProfiler* profiler) 
            : m_name(name), m_profiler(profiler) {
            m_id = m_profiler->beginProfiling(m_name);
        }
        
        ~ScopedProfiler() {
            m_profiler->endProfiling(m_id);
        }
        
    private:
        QString m_name;
        QString m_id;
        PerformanceProfiler* m_profiler;
    };
    
    // Report generation
    QByteArray generateReport() const;
    void exportReport(const QString& filePath) const;
    void clearProfileData();
    
    // Analysis
    QMap<QString, double> getAverageExecutionTimes() const;
    QMap<QString, int> getExecutionCounts() const;
    QString getSlowestFunction() const;

signals:
    void profilingComplete(const QString& profileId, double executionTime);

private:
    PerformanceProfiler();
    
    struct ProfileData {
        QString functionName;
        QString category;
        qint64 startTime;
        int callCount = 0;
        double totalTime = 0.0;
        double avgTime = 0.0;
        double minTime = std::numeric_limits<double>::max();
        double maxTime = 0.0;
    };
    
    QMap<QString, ProfileData> m_profileData;
    mutable QMutex m_mutex;
    QTimer* m_reportTimer = nullptr;
};

// Convenience macros for performance profiling
#define PROFILE_FUNCTION() \
    PerformanceProfiler::ScopedProfiler _profiler(__FUNCTION__, &PerformanceProfiler::instance())

#define PROFILE_SCOPE(name) \
    PerformanceProfiler::ScopedProfiler _profiler(name, &PerformanceProfiler::instance())

#define PROFILE_CUSTOM(name, category) \
    PerformanceProfiler::instance().beginProfiling(name); \
    auto _cleanup_profile = qScopeGuard([&]() { \
        PerformanceProfiler::instance().endProfiling(name, category); \
    })

// Template implementations for ResourceManager
template<typename T>
std::shared_ptr<T> ResourceManager::acquireResource(const QString& poolName, std::function<std::shared_ptr<T>()> createFunc) {
    QMutexLocker locker(&m_mutex);
    
    auto& pool = m_resourcePools[poolName];
    
    // Try to find an available resource
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        if (auto resource = std::dynamic_pointer_cast<T>(it->lock())) {
            pool.erase(it);
            return resource;
        }
    }
    
    // Create new resource
    return createFunc();
}

template<typename T>
void ResourceManager::releaseResource(const QString& poolName, std::shared_ptr<T> resource) {
    QMutexLocker locker(&m_mutex);
    m_resourcePools[poolName].append(std::weak_ptr<void>(resource));
    
    // Limit pool size to prevent memory leaks
    auto& pool = m_resourcePools[poolName];
    if (pool.size() > 10) {
        pool.removeFirst();
    }
}