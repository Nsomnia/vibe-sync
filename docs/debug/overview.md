# Debug Features Overview

The debug-production branch includes a comprehensive debugging and monitoring system designed for end users, developers, and AI agents. This system provides extensive logging, performance monitoring, error handling, and diagnostic capabilities.

## Debug System Architecture

### Core Components
```
┌─────────────────────────────────────────────────────────────┐
│                     Debug Manager                           │
├─────────────────────────────────────────────────────────────┤
│ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────┐ │
│ │ Debug Log   │ │ Performance │ │ Error       │ │ Plugin  │ │
│ │ System      │ │ Monitor     │ │ Handler     │ │ Manager │ │
│ └─────────────┘ └─────────────┘ └─────────────┘ └─────────┘ │
├─────────────────────────────────────────────────────────────┤
│ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────┐ │
│ │ Audio       │ │ Visualizer  │ │ Recording   │ │ Text    │ │
│ │ Debug       │ │ Debug       │ │ Debug       │ │ Debug   │ │
│ └─────────────┘ └─────────────┘ └─────────────┘ └─────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### Debug Categories (11 Total)
1. **Core** - Main application functionality
2. **Audio** - Music player and audio processing
3. **Visualization** - ProjectM and visualizer operations
4. **UI** - User interface interactions
5. **Performance** - Resource usage and optimization
6. **Network** - External API calls and integrations
7. **Database** - Data persistence and storage
8. **Security** - Authentication and permissions
9. **Plugin** - Plugin loading and management
10. **Integration** - External service integration
11. **Custom** - User-defined debug categories

### Debug Levels (6 Total)
1. **Trace** - Detailed execution flow (most verbose)
2. **Debug** - General debugging information
3. **Info** - Important events and status
4. **Warning** - Potential issues
5. **Error** - Error conditions
6. **Fatal** - Critical system failures (least verbose)

## Quick Start Guide

### Enable Basic Debugging
```bash
# Enable all categories at Info level
./vibe-sync --debug-all

# Enable specific categories
./vibe-sync --debug-core --debug-audio --debug-visualization

# Set specific debug level
./vibe-sync --debug-level Info

# Save debug to file
./vibe-sync --debug-file --debug-log-path /tmp/vibesync.log

# Network debug server (for remote monitoring)
./vibe-sync --debug-network --debug-port 12345
```

### Debug UI Panel
Press **'D'** to toggle the debug panel, which provides:
- Real-time log output
- Performance metrics
- Error statistics
- Plugin status
- System resource usage

## Core Debug Features

### 1. Comprehensive Logging System

#### Log Categories and Examples
```bash
# Core Application Logging
DEBUG_INFO(Core, "Application starting with args: %s", args);
DEBUG_DEBUG(Core, "Configuration loaded from: %s", config_path);
DEBUG_WARNING(Core, "Deprecated config option: %s", option);

# Audio System Logging
DEBUG_INFO(Audio, "Playing track: %s", track_title);
DEBUG_DEBUG(Audio, "Buffer underrun detected, adjusting latency");
DEBUG_ERROR(Audio, "Failed to load audio file: %s", file_path);

# Visualizer Logging
DEBUG_INFO(Visualization, "Loaded preset: %s", preset_name);
DEBUG_DEBUG(Visualization, "Shader compilation time: %ums", compile_time);
DEBUG_ERROR(Visualization, "GPU driver crash in preset: %s", preset_path);

# Performance Logging
DEBUG_INFO(Performance, "Memory usage: %lu MB", memory_usage);
DEBUG_DEBUG(Performance, "FPS: %d, GPU usage: %d%%", fps, gpu_usage);
DEBUG_WARNING(Performance, "High CPU usage detected: %d%%", cpu_usage);

# Plugin System Logging
DEBUG_INFO(Plugin, "Loaded plugin: %s", plugin_name);
DEBUG_DEBUG(Plugin, "Plugin API version: %s", api_version);
DEBUG_ERROR(Plugin, "Plugin initialization failed: %s", plugin_path);
```

#### Advanced Logging Features
```bash
# Thread-aware logging
DEBUG_THREAD_INFO("Main", "Audio processing started");

# Memory tracking
DEBUG_MEMORY_INFO("AudioBuffer", "Allocated 1024 bytes");
DEBUG_MEMORY_DEBUG("Texture", "Texture pool full, allocating new");

# Performance profiling
DEBUG_PROFILE_START("PresetLoad");
DEBUG_PROFILE_END("PresetLoad", "Loaded in %lums");

# Conditional logging
DEBUG_IF(audio_buffer_underruns > 10) {
    DEBUG_WARNING(Audio, "Frequent buffer underruns detected");
}
```

### 2. Performance Monitoring System

#### Real-time Metrics
```bash
# System resource monitoring
./vibe-sync --debug-performance

# Metrics displayed:
# - CPU Usage (total and per-thread)
# - Memory Usage (RAM and GPU memory)
# - Frame Rate (current and average)
# - Audio Latency
# - Disk I/O
# - Network I/O
```

#### Performance Alerts
```bash
# Configure performance thresholds
./vibe-sync --performance-alert-cpu 80
./vibe-sync --performance-alert-memory 85
./vibe-sync --performance-alert-fps 24
./vibe-sync --performance-alert-latency 100

# Alert actions
./vibe-sync --alert-action log    # Log only
./vibe-sync --alert-action notify # User notification
./vibe-sync --alert-action reduce # Auto quality reduction
```

#### Performance Profiling
```bash
# Function-level profiling
PROFILE_FUNCTION("AudioProcessing");
PROFILE_SCOPE("VisualizerUpdate");

// Code block profiling
{
    DEBUG_PROFILE_START("HeavyCalculation");
    // ... expensive operation
    DEBUG_PROFILE_END("HeavyCalculation");
}

# Profiling macros for common operations
PROFILE_AUDIO_DECODE("MP3 Decode");
PROFILE_SHADER_COMPILE("GLSL Compile");
PROFILE_TEXTURE_UPLOAD("GPU Upload");
```

### 3. Error Handling and Recovery System

#### Error Categories and Recovery
```bash
# Audio Errors
./vibe-sync --audio-error-recovery auto-retry

# Visualizer Errors
./vibe-sync --visualizer-error-recovery skip-preset

# Plugin Errors
./vibe-sync --plugin-error-recovery disable-plugin

# Memory Errors
./vibe-sync --memory-error-recovery garbage-collect
```

#### Error Recovery Strategies
1. **Ignore** - Continue operation, log error
2. **Retry** - Automatic retry with exponential backoff
3. **Fallback** - Use alternative implementation
4. **UserPrompt** - Ask user for action
5. **AutoRecover** - Automatic recovery attempt
6. **CriticalShutdown** - Graceful application exit

#### Crash Reporting
```bash
# Enable crash reporting
./vibe-sync --crash-reporting

# Automatic crash report generation
./vibe-sync --auto-crash-report \
    --include-logs \
    --include-system-info \
    --upload-reports

# Manual crash report
DEBUG_FATAL("Manual crash trigger for testing");
```

### 4. Plugin Debug System

#### Plugin Lifecycle Debugging
```bash
# Enable plugin debugging
./vibe-sync --debug-plugin

# Plugin loading debug
DEBUG_INFO(Plugin, "Scanning directory: %s", plugin_dir);
DEBUG_DEBUG(Plugin, "Found plugin: %s", plugin_name);
DEBUG_INFO(Plugin, "Loading plugin: %s", plugin_path);
DEBUG_DEBUG(Plugin, "Plugin API version: %s", api_version);
DEBUG_INFO(Plugin, "Plugin initialized successfully");
```

#### Plugin Testing Framework
```bash
# Plugin unit testing
./vibe-sync --test-plugins \
    --plugin-directory /path/to/plugins

# Plugin validation
./vibe-sync --validate-plugins \
    --check-api-compliance \
    --check-memory-leaks

# Plugin performance testing
./vibe-sync --benchmark-plugin /path/to/plugin.so
```

## Advanced Debug Features

### 5. Network Debugging

#### API Call Debugging
```bash
# Enable network debugging
./vibe-sync --debug-network

# Suno AI integration debugging
./vibe-sync --debug-suno-api \
    --log-requests \
    --log-responses \
    --log-timing

# Web service debugging
DEBUG_INFO(Network, "API Request: %s", request_url);
DEBUG_DEBUG(Network, "Request Headers: %s", headers);
DEBUG_DEBUG(Network, "Response Time: %lums", response_time);
DEBUG_WARNING(Network, "Rate limit approaching: %d/%d", used, limit);
```

### 6. Database and Storage Debugging

#### Data Persistence Debugging
```bash
# Enable database debugging
./vibe-sync --debug-database

# Configuration management
DEBUG_INFO(Database, "Loading config from: %s", config_path);
DEBUG_DEBUG(Database, "Config checksum: %s", checksum);
DEBUG_INFO(Database, "Saving config to: %s", config_path);

# Playlist management
DEBUG_INFO(Database, "Playlist loaded: %d tracks", track_count);
DEBUG_DEBUG(Database, "Playlist size: %lu bytes", playlist_size);
```

### 7. Security and Permission Debugging

#### Authentication Debugging
```bash
# Enable security debugging
./vibe-sync --debug-security

# Permission checking
DEBUG_INFO(Security, "Checking file permissions: %s", file_path);
DEBUG_DEBUG(Security, "Read access: %s", has_read ? "granted" : "denied");
DEBUG_DEBUG(Security, "Write access: %s", has_write ? "granted" : "denied");
DEBUG_WARNING(Security, "Insufficient permissions for: %s", file_path);
```

### 8. Custom Debug Categories

#### User-Defined Debugging
```bash
# Define custom category
./vibe-sync --add-debug-category MyCustom

# Use in code
DEBUG_INFO(MyCustom, "Custom operation started");
DEBUG_DEBUG(MyCustom, "Operation result: %s", result);

# Custom debug level
./vibe-sync --set-debug-level MyCustom Debug
```

## Debug Output Destinations

### Console Output
```bash
# Color-coded console output
./vibe-sync --debug-console \
    --debug-colors \
    --debug-timestamps

# Real-time console output
./vibe-sync --debug-realtime
```

### File Output
```bash
# Log to file
./vibe-sync --debug-file \
    --debug-log-path /tmp/vibesync.log \
    --debug-log-rotate \
    --debug-log-max-size 100MB

# Separate log files per category
./vibe-sync --debug-separate-files
```

### Network Output
```bash
# Network debug server
./vibe-sync --debug-network \
    --debug-port 12345 \
    --debug-server-auth token123

# Remote log forwarding
./vibe-sync --debug-forward \
    --debug-forward-url http://log-server.com/api/logs
```

### Database Output
```bash
# Store logs in database
./vibe-sync --debug-database-storage \
    --debug-db-path ~/.config/vibe-sync/debug.db

# Log retention policy
./vibe-sync --debug-retention \
    --debug-keep-days 30
```

## Debug Configuration

### Command Line Configuration
```bash
# Quick debug enable
./vibe-sync --debug-all --debug-level Debug

# Selective debugging
./vibe-sync --debug-core Info \
    --debug-audio Debug \
    --debug-visualization Trace

# Performance monitoring
./vibe-sync --debug-performance \
    --performance-interval 1.0 \
    --performance-metrics all
```

### Configuration File
Create `~/.config/vibe-sync/debug.toml`:

```toml
# Debug System Configuration
[debug]
enabled = true
console_output = true
file_output = true
network_output = false

[debug.levels]
default = "Info"
Core = "Debug"
Audio = "Debug"
Visualization = "Info"
Performance = "Debug"
UI = "Warning"

[debug.output]
console_colors = true
console_timestamps = true
file_path = "/tmp/vibesync.log"
file_rotate = true
file_max_size = "100MB"

[debug.performance]
enabled = true
interval = 1.0
metrics = ["cpu", "memory", "fps", "latency"]
alerts_enabled = true

[debug.performance.thresholds]
cpu_warning = 80
memory_warning = 85
fps_critical = 24
latency_warning = 100

[debug.network]
enabled = false
port = 12345
auth_token = "debug123"

[debug.plugins]
enabled = true
validate_on_load = true
test_on_load = true
```

### Environment Variables
```bash
# Environment-based configuration
export VIBESYNC_DEBUG_ENABLED=1
export VIBESYNC_DEBUG_LEVEL=Debug
export VIBESYNC_DEBUG_LOG=/tmp/vibesync.log
export VIBESYNC_DEBUG_PERFORMANCE=1
```

## Integration with Other Systems

### Music Player Integration
```bash
# Audio-specific debugging
./vibe-sync --debug-audio \
    --audio-buffer-debug \
    --audio-codec-debug \
    --audio-latency-debug

# Playlist debugging
./vibe-sync --debug-playlist \
    --debug-metadata \
    --debug-file-access
```

### Visualizer Integration
```bash
# ProjectM debugging
./vibe-sync --debug-projectm \
    --debug-shader-compilation \
    --debug-preset-loading \
    --debug-audio-analysis

# GPU debugging
./vibe-sync --debug-gpu \
    --gpu-debug-messages \
    --gpu-performance-counter
```

### Recording Integration
```bash
# Recording debugging
./vibe-sync --debug-recording \
    --debug-ffmpeg \
    --debug-audio-sync \
    --debug-video-quality

# FFmpeg debugging
./vibe-sync --debug-ffmpeg \
    --ffmpeg-verbose \
    --ffmpeg-log-level debug
```

### Text System Integration
```bash
# Text overlay debugging
./vibe-sync --debug-text \
    --debug-font-loading \
    --debug-text-rendering \
    --debug-animation-timings
```

## Debug for Different User Types

### End Users
- **Simple Debug Enable**: `./vibe-sync --debug-all`
- **Performance Monitoring**: `./vibe-sync --debug-performance`
- **Error Reporting**: Automatic crash reports
- **Helpful Debug Panel**: Press 'D' for visual debug interface

### Developers
- **Detailed Logging**: All categories at appropriate levels
- **Performance Profiling**: Function-level profiling
- **Memory Tracking**: Memory usage and leak detection
- **Plugin Debugging**: Comprehensive plugin system debugging

### AI Agents
- **Systematic Debugging**: Configurable debug categories
- **Automated Analysis**: Performance trend analysis
- **Error Pattern Detection**: Machine learning error detection
- **Resource Monitoring**: Real-time resource usage tracking

## Troubleshooting with Debug Tools

### Common Issue Diagnostics

#### Audio Problems
```bash
./vibe-sync --debug-audio --debug-performance
# Look for:
# - Buffer underruns
# - Codec errors
# - Device access issues
# - Latency spikes
```

#### Visualizer Issues
```bash
./vibe-sync --debug-visualization --debug-gpu
# Look for:
# - Shader compilation errors
# - GPU driver crashes
# - Performance degradation
# - Memory leaks
```

#### Plugin Problems
```bash
./vibe-sync --debug-plugin --debug-security
# Look for:
# - Plugin loading failures
# - API version mismatches
# - Permission issues
# - Memory corruption
```

#### Performance Issues
```bash
./vibe-sync --debug-performance --debug-memory
# Monitor:
# - CPU usage patterns
# - Memory consumption
# - GPU utilization
# - Disk I/O bottlenecks
```

## Advanced Debug Features

### 1. Automated Problem Detection
```bash
# AI-powered issue detection
./vibe-sync --ai-problem-detection \
    --learn-normal-behavior \
    --detect-anomalies \
    --predict-issues

# Pattern recognition
./vibe-sync --debug-pattern-recognition \
    --error-patterns \
    --performance-patterns \
    --user-behavior-patterns
```

### 2. Debug Data Analysis
```bash
# Generate debug report
./vibe-sync --debug-report \
    --include-logs \
    --include-performance \
    --include-system-info \
    --output-format json

# Debug data analytics
./vibe-sync --debug-analytics \
    --analyze-trends \
    --find-bottlenecks \
    --performance-optimization
```

### 3. Remote Debugging
```bash
# Remote debug session
./vibe-sync --remote-debug \
    --connect-to 192.168.1.100:12345 \
    --secure-connection

# Debug session recording
./vibe-sync --record-debug-session \
    --output-file debug-session.log \
    --include-screenshots
```

## Future Debug Enhancements

Planned debug features:

1. **AI Debug Assistant** - Intelligent problem diagnosis
2. **Visual Debug Interface** - Graphical debug dashboard
3. **Collaborative Debugging** - Multi-user debug sessions
4. **Cloud Debug Storage** - Centralized log analysis
5. **Predictive Analytics** - Proactive issue prevention
6. **Integration Debugging** - Cross-system debugging
7. **Real-time Collaboration** - Live debugging sessions
8. **Automated Testing Integration** - Debug-guided testing

## Debug Shortcuts Reference

| Key | Function |
|-----|----------|
| D | Toggle debug panel |
| P | Toggle performance monitor |
| L | Clear debug logs |
| S | Save debug report |
| R | Reset debug settings |
| 1-9 | Adjust debug level |
| F | Filter debug output |
| C | Copy debug to clipboard |

For detailed information about specific debug categories, see:
- [Debug Categories](categories.md)
- [Performance Monitoring](performance.md)
- [Error Handling](error-handling.md)
- [Plugin System](plugin-system.md)

This comprehensive debug system provides the foundation for reliable operation, performance optimization, and issue resolution across all VibeSync features.