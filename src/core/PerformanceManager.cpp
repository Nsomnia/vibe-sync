#include "PerformanceManager.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <algorithm>
#include <numeric>

// ==================== PerformanceMonitor Implementation ====================

PerformanceMonitor& PerformanceMonitor::instance() {
    static PerformanceMonitor instance;
    return instance;
}

PerformanceMonitor::PerformanceMonitor()
    : m_currentSnapshot{}, m_stats{} {
    
    // Initialize system information
    m_cpuCores = QThread::idealThreadCount();
    m_totalMemory = 8ULL * 1024 * 1024 * 1024; // Placeholder - should query system
    
    m_monitoringTimer = new QTimer(this);
    connect(m_monitoringTimer, &QTimer::timeout, this, &PerformanceMonitor::collectSystemMetrics);
    
    m_stats.avgCpuUsage = 0.0;
    m_stats.peakCpuUsage = 0.0;
    m_stats.avgMemoryUsage = 0;
    m_stats.peakMemoryUsage = 0;
}

void PerformanceMonitor::startMonitoring(int intervalMs) {
    QMutexLocker locker(&m_mutex);
    
    if (!m_isMonitoring) {
        m_isMonitoring = true;
        m_monitoringTimer->start(intervalMs);
        DEBUG_INFO(DebugCategory::Performance, QString("Performance monitoring started (interval: %1ms)").arg(intervalMs));
    }
}

void PerformanceMonitor::stopMonitoring() {
    QMutexLocker locker(&m_mutex);
    
    if (m_isMonitoring) {
        m_isMonitoring = false;
        m_monitoringTimer->stop();
        DEBUG_INFO(DebugCategory::Performance, "Performance monitoring stopped");
    }
}

void PerformanceMonitor::recordComponentMetrics(const QString& component, double cpu, size_t memory, int threads) {
    QMutexLocker locker(&m_mutex);
    
    m_currentSnapshot.component = component;
    m_currentSnapshot.cpuUsagePercent = cpu;
    m_currentSnapshot.memoryUsageBytes = memory;
    m_currentSnapshot.activeThreads = threads;
    m_currentSnapshot.timestamp = QDateTime::currentDateTime();
    
    // Update history
    m_cpuHistory[component].append(cpu);
    m_memoryHistory[component].append(memory);
    
    // Keep history manageable
    if (m_cpuHistory[component].size() > 3600) { // 1 hour at 1-second intervals
        m_cpuHistory[component].removeFirst();
        m_memoryHistory[component].removeFirst();
    }
}

void PerformanceMonitor::recordAudioMetrics(int bufferSize, int sampleRate, int channels) {
    QMutexLocker locker(&m_mutex);
    
    m_currentSnapshot.audioBufferSize = bufferSize;
    
    // Calculate latency (simplified)
    m_stats.audioLatencyMs = (bufferSize * 1000.0) / sampleRate;
}

void PerformanceMonitor::recordVideoMetrics(int frameRate, int resolution, double renderTimeMs) {
    QMutexLocker locker(&m_mutex);
    
    m_currentSnapshot.videoFrameRate = frameRate;
    m_stats.frameRate = frameRate;
    
    // Check for frame rate alerts
    if (m_alertsEnabled && frameRate < m_lowFrameRateThreshold) {
        emit performanceAlert("Low Frame Rate", 
                             QString("Frame rate dropped to %1 FPS").arg(frameRate), 
                             frameRate);
    }
}

void PerformanceMonitor::recordPresetMetrics(const QString& presetPath, int loadTimeMs) {
    QMutexLocker locker(&m_mutex);
    
    m_currentSnapshot.presetLoadTimeMs = loadTimeMs;
    
    // Check for slow preset loading
    if (m_alertsEnabled && loadTimeMs > 5000) { // 5 seconds threshold
        emit performanceAlert("Slow Preset Load", 
                             QString("Preset '%1' loaded in %2ms").arg(presetPath).arg(loadTimeMs), 
                             loadTimeMs);
    }
}

void PerformanceMonitor::setAlertThresholds(double highCpuPercent, size_t highMemoryMB, int lowFrameRate) {
    QMutexLocker locker(&m_mutex);
    
    m_highCpuThreshold = highCpuPercent;
    m_highMemoryThreshold = highMemoryMB * 1024 * 1024; // Convert to bytes
    m_lowFrameRateThreshold = lowFrameRate;
    
    DEBUG_INFO(DebugCategory::Performance, 
              QString("Alert thresholds updated - CPU: %1%%, Memory: %2MB, FPS: %3")
              .arg(highCpuPercent).arg(highMemoryMB).arg(lowFrameRate));
}

void PerformanceMonitor::enableAlerts(bool enable) {
    m_alertsEnabled = enable;
    DEBUG_INFO(DebugCategory::Performance, 
              QString("Performance alerts %1").arg(enable ? "enabled" : "disabled"));
}

PerformanceSnapshot PerformanceMonitor::getCurrentSnapshot() const {
    QMutexLocker locker(&m_mutex);
    return m_currentSnapshot;
}

QVector<PerformanceSnapshot> PerformanceMonitor::getHistory(int minutes) const {
    QMutexLocker locker(&m_mutex);
    
    if (minutes <= 0) return m_history;
    
    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-minutes * 60);
    QVector<PerformanceSnapshot> filtered;
    
    for (const auto& snapshot : m_history) {
        if (snapshot.timestamp >= cutoff) {
            filtered.append(snapshot);
        }
    }
    
    return filtered;
}

void PerformanceMonitor::clearHistory() {
    QMutexLocker locker(&m_mutex);
    
    m_history.clear();
    m_cpuHistory.clear();
    m_memoryHistory.clear();
    
    DEBUG_INFO(DebugCategory::Performance, "Performance history cleared");
}

QVector<OptimizationRecommendation> PerformanceMonitor::generateRecommendations() const {
    QMutexLocker locker(&m_mutex);
    
    QVector<OptimizationRecommendation> recommendations;
    
    // CPU optimization recommendations
    if (m_stats.avgCpuUsage > 70.0) {
        OptimizationRecommendation rec;
        rec.id = "reduce_cpu_usage";
        rec.title = "Reduce CPU Usage";
        rec.description = "Current average CPU usage is high. Consider reducing preset complexity or frame rate.";
        rec.category = "CPU";
        rec.expectedImprovement = 25.0;
        rec.isSafe = true;
        rec.requiresRestart = false;
        recommendations.append(rec);
    }
    
    // Memory optimization recommendations
    if (m_stats.avgMemoryUsage > m_highMemoryThreshold * 0.8) {
        OptimizationRecommendation rec;
        rec.id = "optimize_memory";
        rec.title = "Optimize Memory Usage";
        rec.description = "Memory usage is approaching limits. Enable memory compression or reduce buffer sizes.";
        rec.category = "Memory";
        rec.expectedImprovement = 30.0;
        rec.isSafe = true;
        rec.requiresRestart = false;
        rec.applyFunction = []() {
            ResourceManager::instance().enableMemoryCompression(true);
        };
        recommendations.append(rec);
    }
    
    // Frame rate optimization
    if (m_stats.frameRate < 45) {
        OptimizationRecommendation rec;
        rec.id = "optimize_frame_rate";
        rec.title = "Optimize Frame Rate";
        rec.description = "Frame rate is below optimal. Reduce video quality or enable performance mode.";
        rec.category = "Video";
        rec.expectedImprovement = 20.0;
        rec.isSafe = true;
        rec.requiresRestart = false;
        recommendations.append(rec);
    }
    
    return recommendations;
}

void PerformanceMonitor::applyRecommendation(const QString& recommendationId) {
    QMutexLocker locker(&m_mutex);
    
    auto recommendations = generateRecommendations();
    for (const auto& rec : recommendations) {
        if (rec.id == recommendationId && rec.applyFunction) {
            DEBUG_INFO(DebugCategory::Performance, 
                      QString("Applying recommendation: %1").arg(rec.title));
            rec.applyFunction();
            return;
        }
    }
    
    DEBUG_WARNING(DebugCategory::Performance, 
                 QString("Unknown recommendation ID: %1").arg(recommendationId));
}

void PerformanceMonitor::optimizeMemory() {
    DEBUG_INFO(DebugCategory::Performance, "Performing memory optimization");
    
    ResourceManager::instance().scheduleCleanup();
    ResourceManager::instance().enableMemoryCompression(true);
    
    emit performanceAlert("Memory Optimized", "Memory optimization completed", 0.0);
}

void PerformanceMonitor::optimizeCpuUsage() {
    DEBUG_INFO(DebugCategory::Performance, "Performing CPU optimization");
    
    // Reduce frame rate if too high
    if (m_stats.frameRate > 60) {
        // This would be implemented in the actual video engine
        DEBUG_INFO(DebugCategory::Performance, "Reducing frame rate for CPU optimization");
    }
    
    // Enable thread pooling
    ResourceManager::instance().enableMemoryPooling(true);
    
    emit performanceAlert("CPU Optimized", "CPU optimization completed", 0.0);
}

void PerformanceMonitor::cleanupUnusedResources() {
    DEBUG_INFO(DebugCategory::Performance, "Cleaning up unused resources");
    
    ResourceManager::instance().forceCleanup();
    
    emit performanceAlert("Cleanup Complete", "Unused resources cleaned up", 0.0);
}

PerformanceMonitor::PerformanceStats PerformanceMonitor::getStatistics() const {
    QMutexLocker locker(&m_mutex);
    return m_stats;
}

void PerformanceMonitor::collectSystemMetrics() {
    QMutexLocker locker(&m_mutex);
    
    // Simulate system metrics collection (in a real implementation, this would query OS)
    m_currentSnapshot.timestamp = QDateTime::currentDateTime();
    m_currentSnapshot.activeThreads = QThread::idealThreadCount();
    
    // Calculate averages
    double totalCpu = 0.0;
    size_t totalMemory = 0;
    
    for (auto it = m_cpuHistory.begin(); it != m_cpuHistory.end(); ++it) {
        if (!it->isEmpty()) {
            totalCpu += it->last();
        }
    }
    
    for (auto it = m_memoryHistory.begin(); it != m_memoryHistory.end(); ++it) {
        if (!it->isEmpty()) {
            totalMemory += it->last();
        }
    }
    
    m_stats.avgCpuUsage = totalCpu / std::max(1, m_cpuHistory.size());
    m_stats.avgMemoryUsage = totalMemory;
    
    // Update peak values
    m_stats.peakCpuUsage = std::max(m_stats.peakCpuUsage, m_stats.avgCpuUsage);
    m_stats.peakMemoryUsage = std::max(m_stats.peakMemoryUsage, m_stats.avgMemoryUsage);
    
    // Add to history
    m_history.append(m_currentSnapshot);
    if (m_history.size() > 3600) { // Keep 1 hour of data
        m_history.removeFirst();
    }
    
    // Emit snapshot
    emit performanceSnapshot(m_currentSnapshot);
    
    // Check alerts
    checkPerformanceAlerts();
    
    // Analyze patterns periodically
    static int tickCount = 0;
    if (++tickCount % 60 == 0) { // Every minute
        analyzePatterns();
    }
}

void PerformanceMonitor::checkPerformanceAlerts() {
    if (!m_alertsEnabled) return;
    
    // CPU alert
    if (m_currentSnapshot.cpuUsagePercent > m_highCpuThreshold) {
        emit performanceAlert("High CPU Usage", 
                             QString("CPU usage at %1%").arg(m_currentSnapshot.cpuUsagePercent, 0, 'f', 1),
                             m_currentSnapshot.cpuUsagePercent);
    }
    
    // Memory alert
    if (m_currentSnapshot.memoryUsageBytes > m_highMemoryThreshold) {
        emit performanceAlert("High Memory Usage", 
                             QString("Memory usage at %1MB").arg(m_currentSnapshot.memoryUsageBytes / (1024.0 * 1024.0), 0, 'f', 1),
                             m_currentSnapshot.memoryUsageBytes / (1024.0 * 1024.0));
    }
    
    // Frame rate alert
    if (m_currentSnapshot.videoFrameRate > 0 && m_currentSnapshot.videoFrameRate < m_lowFrameRateThreshold) {
        emit performanceAlert("Low Frame Rate", 
                             QString("Frame rate dropped to %1 FPS").arg(m_currentSnapshot.videoFrameRate),
                             m_currentSnapshot.videoFrameRate);
    }
}

void PerformanceMonitor::analyzePatterns() {
    auto recommendations = generateRecommendations();
    for (const auto& rec : recommendations) {
        emit optimizationRecommended(rec);
    }
}

void PerformanceMonitor::emitRecommendation(const OptimizationRecommendation& rec) {
    emit optimizationRecommended(rec);
}

// ==================== ResourceManager Implementation ====================

ResourceManager& ResourceManager::instance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager()
    : m_cleanupTimer(new QTimer(this)), m_pressureTimer(new QTimer(this)) {
    
    connect(m_cleanupTimer, &QTimer::timeout, this, &ResourceManager::performCleanup);
    connect(m_pressureTimer, &QTimer::timeout, this, &ResourceManager::checkMemoryPressure);
    
    m_cleanupTimer->start(60000); // Cleanup every minute
    m_pressureTimer->start(5000); // Check pressure every 5 seconds
}

void ResourceManager::setMemoryLimits(size_t maxBytes) {
    m_maxMemoryBytes = maxBytes;
    DEBUG_INFO(DebugCategory::Performance, 
              QString("Memory limit set to %1MB").arg(maxBytes / (1024.0 * 1024.0), 0, 'f', 1));
}

void ResourceManager::registerMemorySource(const QString& name, std::function<size_t()> getUsage) {
    QMutexLocker locker(&m_mutex);
    m_memorySources[name] = getUsage;
    
    DEBUG_DEBUG(DebugCategory::Performance, 
               QString("Registered memory source: %1").arg(name));
}

void ResourceManager::enableMemoryCompression(bool enable) {
    m_memoryCompressionEnabled = enable;
    DEBUG_INFO(DebugCategory::Performance, 
              QString("Memory compression %1").arg(enable ? "enabled" : "disabled"));
}

void ResourceManager::enableMemoryPooling(bool enable) {
    m_memoryPoolingEnabled = enable;
    DEBUG_INFO(DebugCategory::Performance, 
              QString("Memory pooling %1").arg(enable ? "enabled" : "disabled"));
}

void ResourceManager::setCpuPriority(QThread* thread, int priority) {
    if (thread && thread->isRunning()) {
        // In a real implementation, this would set actual thread priority
        DEBUG_DEBUG(DebugCategory::Performance, 
                   QString("Thread priority set to %1").arg(priority));
    }
}

void ResourceManager::limitThreadCount(int maxThreads) {
    // This would limit the number of worker threads in a real implementation
    DEBUG_INFO(DebugCategory::Performance, 
              QString("Thread count limited to %1").arg(maxThreads));
}

void ResourceManager::enableCpuAffinity(int coreId) {
    // This would bind threads to specific CPU cores
    DEBUG_INFO(DebugCategory::Performance, 
              QString("CPU affinity enabled for core %1").arg(coreId));
}

void ResourceManager::scheduleCleanup() {
    QTimer::singleShot(100, this, &ResourceManager::performCleanup);
}

void ResourceManager::forceCleanup() {
    DEBUG_INFO(DebugCategory::Performance, "Forcing resource cleanup");
    performCleanup();
}

void ResourceManager::optimizeForGaming() {
    DEBUG_INFO(DebugCategory::Performance, "Optimizing for gaming performance");
    
    setCpuPriority(QThread::currentThread(), -5); // Higher priority
    enableMemoryCompression(true);
    limitThreadCount(4);
    
    emit performanceAlert("Optimization", "Gaming optimization applied", 0.0);
}

void ResourceManager::optimizeForRecording() {
    DEBUG_INFO(DebugCategory::Performance, "Optimizing for recording");
    
    setCpuPriority(QThread::currentThread(), -10); // Highest priority
    enableMemoryPooling(true);
    
    emit performanceAlert("Optimization", "Recording optimization applied", 0.0);
}

void ResourceManager::checkMemoryPressure() {
    QMutexLocker locker(&m_mutex);
    
    size_t totalUsage = 0;
    for (const auto& source : m_memorySources) {
        totalUsage += source();
    }
    
    if (totalUsage > m_maxMemoryBytes * 0.9) { // 90% of limit
        emit memoryPressure(totalUsage, m_maxMemoryBytes);
        
        if (totalUsage > m_maxMemoryBytes) {
            // Trigger emergency cleanup
            performCleanup();
        }
    }
}

void ResourceManager::performCleanup() {
    QMutexLocker locker(&m_mutex);
    
    // Clean up expired resource pools
    for (auto it = m_resourcePools.begin(); it != m_resourcePools.end(); ++it) {
        it->removeIf([](const std::weak_ptr<void>& weakRef) {
            return weakRef.expired();
        });
    }
    
    DEBUG_DEBUG(DebugCategory::Performance, "Resource cleanup completed");
}

// ==================== PerformanceProfiler Implementation ====================

PerformanceProfiler& PerformanceProfiler::instance() {
    static PerformanceProfiler instance;
    return instance;
}

PerformanceProfiler::PerformanceProfiler() {
    m_reportTimer = new QTimer(this);
    connect(m_reportTimer, &QTimer::timeout, this, [this]() {
        // Could emit periodic reports here
    });
    m_reportTimer->start(300000); // Every 5 minutes
}

QString PerformanceProfiler::beginProfiling(const QString& functionName) {
    QMutexLocker locker(&m_mutex);
    
    QString profileId = QString("%1_%2").arg(functionName).arg(QDateTime::currentMSecsSinceEpoch());
    
    if (!m_profileData.contains(functionName)) {
        ProfileData data;
        data.functionName = functionName;
        m_profileData[functionName] = data;
    }
    
    m_profileData[functionName].startTime = QElapsedTimer::msecsSinceReference();
    return profileId;
}

void PerformanceProfiler::endProfiling(const QString& profileId, const QString& category) {
    QMutexLocker locker(&m_mutex);
    
    qint64 endTime = QElapsedTimer::msecsSinceReference();
    
    // Extract function name from profile ID
    QString functionName = profileId.split('_').first();
    
    if (!m_profileData.contains(functionName)) {
        return;
    }
    
    ProfileData& data = m_profileData[functionName];
    qint64 executionTime = endTime - data.startTime;
    
    data.category = category;
    data.callCount++;
    data.totalTime += executionTime;
    data.avgTime = data.totalTime / data.callCount;
    data.minTime = std::min(data.minTime, static_cast<double>(executionTime));
    data.maxTime = std::max(data.maxTime, static_cast<double>(executionTime));
    
    emit profilingComplete(profileId, executionTime);
}

QByteArray PerformanceProfiler::generateReport() const {
    QMutexLocker locker(&m_mutex);
    
    QJsonObject report;
    report["generatedAt"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    report["totalProfiledFunctions"] = m_profileData.size();
    
    QJsonObject functions;
    for (const auto& data : m_profileData) {
        QJsonObject funcData;
        funcData["category"] = data.category;
        funcData["callCount"] = data.callCount;
        funcData["totalTimeMs"] = data.totalTime;
        funcData["averageTimeMs"] = data.avgTime;
        funcData["minTimeMs"] = data.minTime;
        funcData["maxTimeMs"] = data.maxTime;
        functions[data.functionName] = funcData;
    }
    report["functions"] = functions;
    
    QJsonDocument doc(report);
    return doc.toJson(QJsonDocument::Indented);
}

void PerformanceProfiler::exportReport(const QString& filePath) const {
    QByteArray data = generateReport();
    QFile file(filePath);
    
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        DEBUG_INFO(DebugCategory::Performance, 
                  QString("Performance report exported to: %1").arg(filePath));
    } else {
        DEBUG_ERROR(DebugCategory::Performance, 
                   QString("Failed to export performance report to: %1").arg(filePath));
    }
}

void PerformanceProfiler::clearProfileData() {
    QMutexLocker locker(&m_mutex);
    m_profileData.clear();
    DEBUG_INFO(DebugCategory::Performance, "Performance profile data cleared");
}

QMap<QString, double> PerformanceProfiler::getAverageExecutionTimes() const {
    QMutexLocker locker(&m_mutex);
    
    QMap<QString, double> result;
    for (const auto& data : m_profileData) {
        result[data.functionName] = data.avgTime;
    }
    return result;
}

QMap<QString, int> PerformanceProfiler::getExecutionCounts() const {
    QMutexLocker locker(&m_mutex);
    
    QMap<QString, int> result;
    for (const auto& data : m_profileData) {
        result[data.functionName] = data.callCount;
    }
    return result;
}

QString PerformanceProfiler::getSlowestFunction() const {
    QMutexLocker locker(&m_mutex);
    
    QString slowestFunction;
    double maxAvgTime = 0.0;
    
    for (const auto& data : m_profileData) {
        if (data.avgTime > maxAvgTime) {
            maxAvgTime = data.avgTime;
            slowestFunction = data.functionName;
        }
    }
    
    return slowestFunction;
}