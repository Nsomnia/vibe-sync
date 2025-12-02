# Video Recording Guide

VibeSync includes comprehensive video recording capabilities powered by FFmpeg, allowing you to create high-quality music videos with synchronized visualizations and text overlays.

## Core Recording Features

### Recording Architecture
```
┌─────────────────────────────────────────────────────────────┐
│                   Final Video Output                       │
├─────────────────────────────────────────────────────────────┤
│ ┌─────────────────────────────────────────────────────────┐ │
│ │              Composite Layer                            │ │
│ │ ┌─────────────────┐ ┌─────────────┐ ┌─────────────────┐ │ │
│ │ │ ProjectM Layer  │ │ Text Layer  │ │ UI Overlay      │ │ │
│ │ │ (Background)    │ │ (Overlays)  │ │ (Controls)      │ │ │
│ │ └─────────────────┘ └─────────────┘ └─────────────────┘ │ │
│ └─────────────────────────────────────────────────────────┘ │
├─────────────────────────────────────────────────────────────┤
│ ┌─────────────────────────────────────────────────────────┐ │
│ │                Audio Layer                               │ │
│ │ │ • System Audio    │ • File Audio    │ • Mixed Audio  │ │
│ │ │ • PipeWire        │ • Direct File   │ • Combined     │ │
│ │ │ • PulseAudio      │ • Lossless      │ • Synchronized │ │
│ │ └─────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### Supported Output Formats
| Format | Extension | Quality | Size | Best For |
|--------|-----------|---------|------|----------|
| H.264 | .mp4 | High | Medium | General purpose |
| HEVC | .mp4 | Very High | Small | High quality |
| VP9 | .webm | High | Small | Web streaming |
| ProRes | .mov | Lossless | Large | Professional |
| DNxHD | .mov | Broadcast | Large | Broadcast TV |

## Basic Recording Setup

### Enable Recording
```bash
# Enable video recording
./vibe-sync --record-video

# Specify output directory
./vibe-sync --record-video --output-directory ~/Videos/VibeSync

# Custom filename format
./vibe-sync --record-video --filename-format "{artist}-{title}-{date}"
```

### Audio Input Configuration

#### System Audio (Recommended)
```bash
# Use system audio (PipeWire)
./vibe-sync --record-video --audio-input-mode PipeWire

# Use PulseAudio
./vibe-sync --record-video --audio-input-mode PulseAudio

# System default
./vibe-sync --record-video --audio-input-mode SystemDefault
```

**PipeWire Setup (Recommended):**
```bash
# Create virtual sink
./vibe-sync --record-video --pipewire-sink-name VibeSyncSink

# Verify sink creation
pw-cli list objects | grep VibeSyncSink

# Route audio to virtual sink
pactl load-module module-loopback source=alsa_output.pci-0000_00_1f.3.analog-stereo sink=VibeSyncSink
```

#### File Audio
```bash
# Record from played audio files
./vibe-sync --record-video --audio-input-mode File \
    --audio-file /path/to/song.mp3

# Automatic file selection
./vibe-sync --record-video --audio-input-mode File \
    --auto-audio-file
```

#### Mixed Audio
```bash
# Combine system and file audio
./vibe-sync --record-video --audio-input-mode Mixed \
    --system-audio-level 0.7 \
    --file-audio-level 0.3
```

### Recording Controls

#### Start/Stop Recording
- **GUI**: Record button in toolbar
- **Keyboard**: 'R' key
- **Command Line**: Automatic when recording enabled

#### Recording Status Indicators
- **Red dot** - Recording in progress
- **Timer** - Recording duration
- **File size** - Current output size
- **Audio levels** - Input audio monitoring

## FFmpeg Integration

### Custom FFmpeg Commands
VibeSync provides full control over FFmpeg encoding:

#### High Quality H.264
```bash
./vibe-sync --record-video \
    --ffmpeg-command "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset slow -crf 18 -c:a aac -b:a 192k -movflags +faststart {OUTPUT_PATH}"
```

#### High Efficiency H.265
```bash
./vibe-sync --record-video \
    --ffmpeg-command "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx265 -crf 28 -preset medium -c:a aac -b:a 192k {OUTPUT_PATH}"
```

#### Professional Quality
```bash
./vibe-sync --record-video \
    --ffmpeg-command "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -profile:v high -level:v 4.2 -crf 16 -c:a pcm_s16le -ar 48000 -ac 2 {OUTPUT_PATH}"
```

#### Web-Optimized
```bash
./vibe-sync --record-video \
    --ffmpeg-command "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libvpx-vp9 -crf 30 -b:v 0 -c:a libopus -b:a 128k {OUTPUT_PATH}"
```

### Command Placeholders
Available placeholders in FFmpeg commands:
- **{WIDTH}** - Video width in pixels
- **{HEIGHT}** - Video height in pixels
- **{FPS}** - Frames per second
- **{FRAMERATE}** - Video framerate
- **{OUTPUT_PATH}** - Full output file path
- **{TEMP_PATH}** - Temporary file path

### Preset Commands

#### Ultra High Quality
```bash
./vibe-sync --record-video --ffmpeg-preset ultra-hq
```

**Equivalent command:**
```bash
ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx265 -crf 15 -preset slow -tune film -c:a pcm_s24le -ar 48000 {OUTPUT_PATH}
```

#### Balanced Quality
```bash
./vibe-sync --record-video --ffmpeg-preset balanced
```

**Equivalent command:**
```bash
ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset medium -crf 23 -c:a aac -b:a 192k {OUTPUT_PATH}
```

#### Streaming Optimized
```bash
./vibe-sync --record-video --ffmpeg-preset streaming
```

**Equivalent command:**
```bash
ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset fast -crf 28 -profile:v baseline -level 3.1 -c:a aac -b:a 128k -movflags +faststart {OUTPUT_PATH}
```

## Recording Settings

### Resolution and Frame Rate
```bash
# Set video resolution
./vibe-sync --record-video --video-width 1920 --video-height 1080

# Set frame rate
./vibe-sync --record-video --video-framerate 60

# Resolution presets
./vibe-sync --record-video --resolution 4K    # 3840x2160
./vibe-sync --record-video --resolution 1080p # 1920x1080
./vibe-sync --record-video --resolution 720p  # 1280x720
```

### Quality Controls
```bash
# Constant Rate Factor (CRF) - 0 (lossless) to 51 (worst)
./vibe-sync --record-video --crf 23

# Bitrate control
./vibe-sync --record-video --bitrate 5000k

# Two-pass encoding
./vibe-sync --record-video --two-pass

# Constant quality mode
./vibe-sync --record-video --constant-quality
```

### Performance Optimization
```bash
# Recording buffer size
./vibe-sync --record-video --buffer-size 32768

# Thread count
./vibe-sync --record-video --threads auto

# Hardware acceleration
./vibe-sync --record-video --hardware-accel h264_nvenc  # NVIDIA
./vibe-sync --record-video --hardware-accel h264_vaapi  # Intel
./vibe-sync --record-video --hardware-accel h264_v4l2m2m # Generic
```

## Advanced Recording Features

### Timeline and Editing

#### Timeline Markers
```bash
# Add timeline markers
./vibe-sync --record-video --add-marker 0:30 "Intro complete"
./vibe-sync --record-video --add-marker 2:45 "Drop happens"
./vibe-sync --record-video --add-marker 4:20 "Outro begins"
```

#### Chapter Creation
```bash
# Generate chapters automatically
./vibe-sync --record-video --generate-chapters \
    --chapter-detection threshold \
    --chapter-threshold 0.8

# Manual chapter points
./vibe-sync --record-video --add-chapter 0:00 "Song Start"
./vibe-sync --record-video --add-chapter 3:30 "Main Verse"
./vibe-sync --record-video --add-chapter 6:45 "Bridge"
```

### Batch Processing
```bash
# Process multiple audio files
./vibe-sync --batch-process \
    --input-directory /path/to/songs \
    --output-directory /path/to/videos \
    --output-template "{artist}_{title}"

# Queue processing
./vibe-sync --queue-process \
    --add-to-queue song1.mp3 \
    --add-to-queue song2.flac \
    --add-to-queue song3.ogg
```

### Scheduled Recording
```bash
# Schedule recording
./vibe-sync --schedule-recording \
    --start-time "2024-01-15T20:00:00" \
    --duration "00:15:30" \
    --audio-file "/path/to/song.mp3"

# Recurring recordings
./vibe-sync --recurring-recording \
    --frequency daily \
    --time "22:00" \
    --duration "00:10:00"
```

## Audio-Video Synchronization

### Sync Methods
```bash
# Audio clock sync (recommended)
./vibe-sync --sync-method audio

# Video clock sync
./vibe-sync --sync-method video

# External clock sync
./vibe-sync --sync-method external \
    --external-clock-url "http://ntp-server.com/time"
```

### Sync Correction
```bash
# Enable automatic sync correction
./vibe-sync --auto-sync-correction \
    --max-sync-drift 100ms \
    --correction-interval 5s

# Manual sync adjustment
./vibe-sync --sync-offset +50ms  # Delay audio by 50ms
./vibe-sync --sync-offset -75ms  # Advance audio by 75ms
```

### Audio Latency Management
```bash
# Measure audio latency
./vibe-sync --measure-latency

# Latency compensation
./vibe-sync --latency-compensation 120ms
```

## Live Recording Features

### Real-time Monitoring
```bash
# Enable preview during recording
./vibe-sync --record-video --preview-enabled

# Audio level monitoring
./vibe-sync --record-video --audio-monitoring \
    --monitor-device default

# Video quality monitoring
./vibe-sync --record-video --quality-monitor
```

### Dynamic Recording
```bash
# Real-time quality adjustment
./vibe-sync --record-video --adaptive-quality \
    --min-quality 720p \
    --max-quality 4K

# Dynamic resolution scaling
./vibe-sync --record-video --dynamic-resolution \
    --scale-threshold 80%cpu
```

## Storage and File Management

### Output Configuration
```bash
# Output directory structure
./vibe-sync --record-video \
    --output-structure artist_album \
    --create-subdirectories

# Filename templates
./vibe-sync --record-video \
    --filename-template "{artist}-{album}-{title}-{date}-{quality}"

# Automatic organization
./vibe-sync --record-video \
    --auto-organize \
    --organization-pattern "{genre}/{year}/{artist}"
```

### File Size Management
```bash
# Maximum file size
./vibe-sync --record-video --max-file-size 2GB

# Automatic file splitting
./vibe-sync --record-video --split-files \
    --split-interval 30m

# Compression efficiency
./vibe-sync --record-video --optimize-compression \
    --compression-ratio 0.8
```

### Temporary Files
```bash
# Temporary directory
./vibe-sync --record-video --temp-dir /tmp/vibe-sync-recordings

# Cleanup settings
./vibe-sync --record-video --cleanup-temp-files \
    --cleanup-delay 300s
```

## Quality Assurance

### Recording Validation
```bash
# Validate output files
./vibe-sync --validate-recordings \
    --check-audio-sync \
    --check-video-quality \
    --check-file-integrity

# Generate quality report
./vibe-sync --record-video --quality-report \
    --report-file recording-quality.json
```

### Performance Monitoring
```bash
# Real-time performance metrics
./vibe-sync --record-video --performance-monitor \
    --metrics-interval 1.0

# Disk I/O monitoring
./vibe-sync --record-video --io-monitor

# Network monitoring (for streaming)
./vibe-sync --record-video --network-monitor
```

### Error Detection
```bash
# Automatic error detection
./vibe-sync --record-video --auto-error-detection \
    --check-dropped-frames \
    --check-audio-gaps \
    --check-encoding-errors

# Error reporting
./vibe-sync --record-video --error-report \
    --report-level detailed
```

## Integration with Other Features

### Music Player Integration
- **Automatic track selection** - Record current playlist
- **Playlist recording** - Record entire playlists
- **Metadata embedding** - Include track info in video metadata
- **Cover art inclusion** - Embed album art in video

### Text Overlay Integration
- **Include text in recording** - Text overlays captured in video
- **Text-only recording mode** - Record just text animations
- **Custom text for recording** - Different text settings for recording

### Visualization Integration
- **Preset synchronization** - Visualizer presets included in recording
- **Quality preservation** - Maintain visual quality during recording
- **Real-time visualization** - No delays in visualization during recording

### Suno AI Integration
- **Auto-record Suno tracks** - Automatically record downloaded Suno songs
- **Metadata enhancement** - Include enhanced metadata in recordings
- **Playlist creation** - Create video playlists from Suno collections

## Troubleshooting

### Common Recording Issues

#### No Audio in Recording
1. **Check audio source**:
   ```bash
   ./vibe-sync --record-video --audio-debug
   ```
2. **Verify permissions** - Ensure access to audio devices
3. **Test different audio modes** - Try PipeWire, PulseAudio, File
4. **Check audio levels** - Ensure audio is not muted

#### Video Quality Issues
1. **Lower resolution** - Reduce video dimensions:
   ```bash
   ./vibe-sync --record-video --video-width 1280 --video-height 720
   ```
2. **Reduce frame rate** - Lower FPS:
   ```bash
   ./vibe-sync --record-video --video-framerate 30
   ```
3. **Check hardware** - Ensure sufficient GPU/CPU resources

#### Sync Problems
1. **Adjust sync offset**:
   ```bash
   ./vibe-sync --sync-offset +100ms
   ```
2. **Change sync method**:
   ```bash
   ./vibe-sync --sync-method video
   ```
3. **Measure latency**:
   ```bash
   ./vibe-sync --measure-audio-latency
   ```

#### Recording Stops Unexpectedly
1. **Check disk space** - Ensure sufficient storage
2. **Monitor performance** - Check CPU/GPU usage:
   ```bash
   ./vibe-sync --record-video --performance-monitor
   ```
3. **Increase buffer size**:
   ```bash
   ./vibe-sync --record-video --buffer-size 65536
   ```

### Debug Commands
```bash
# Enable recording debug logging
./vibe-sync --record-video --debug-recording \
    --debug-ffmpeg \
    --debug-audio-sync \
    --verbose-output

# Test recording pipeline
./vibe-sync --test-recording-pipeline

# Generate diagnostic report
./vibe-sync --record-video --diagnostic-report \
    --output-file recording-diagnostics.json
```

## Configuration Files

### Recording Configuration
Create `~/.config/vibe-sync/recording.toml`:

```toml
# Recording Configuration
[recording]
enabled = true
output_directory = "~/Videos/VibeSync"
filename_format = "{artist}-{title}-{date}"

[recording.audio]
input_mode = "PipeWire"
pipewire_sink_name = "VibeSyncSink"
system_audio_level = 1.0
file_audio_level = 1.0

[recording.video]
width = 1920
height = 1080
framerate = 60
quality = "high"
crf = 23

[recording.ffmpeg]
preset = "balanced"
hardware_accel = "auto"
threads = "auto"

[recording.quality]
auto_quality = true
min_quality = "720p"
max_quality = "4K"
performance_threshold = 80

[recording.storage]
max_file_size = "2GB"
split_files = false
auto_cleanup = true
cleanup_delay = 300
```

### FFmpeg Preset Files
Create `~/.config/vibe-sync/ffmpeg-presets.toml`:

```toml
[presets.ultra_hq]
command = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx265 -crf 15 -preset slow -tune film -c:a pcm_s24le -ar 48000 {OUTPUT_PATH}"
description = "Ultra high quality for professional use"

[presets.balanced]
command = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset medium -crf 23 -c:a aac -b:a 192k {OUTPUT_PATH}"
description = "Balanced quality and file size"

[presets.streaming]
command = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset fast -crf 28 -profile:v baseline -level 3.1 -c:a aac -b:a 128k -movflags +faststart {OUTPUT_PATH}"
description = "Optimized for web streaming"
```

## Keyboard Shortcuts

| Key | Function |
|-----|----------|
| R | Start/Stop recording |
| P | Pause/Resume recording |
| S | Toggle recording settings |
| V | Change video quality |
| A | Cycle audio input mode |
| T | Add timeline marker |
| [ | Previous recording preset |
| ] | Next recording preset |
| 1-9 | Select quality preset |
| O | Open output directory |
| D | Recording diagnostic |

## Future Enhctions

Planned recording features:

1. **Multi-camera Support** - Multiple angle recordings
2. **Green Screen Integration** - Chroma key effects
3. **Real-time Effects** - Live video effects during recording
4. **Cloud Upload** - Direct upload to streaming platforms
5. **Live Streaming** - Real-time streaming to platforms
6. **Advanced Editing** - Built-in video editing tools
7. **AI Enhancement** - AI-powered video quality enhancement
8. **Collaborative Recording** - Multi-user recording sessions

For more technical details, see the [Developer Guide](../developer/debug-systems.md) and [API Reference](../developer/api-reference.md).