# Visualizations Guide

VibeSync uses the industry-standard ProjectM engine for visualizations, providing both classic Milkdrop compatibility and modern AI-powered visual effects. This guide covers all visualizer features from your aurora-projectm-visualizer work.

## ProjectM Engine Integration

### Core Features
- **ProjectM Library** - Industry-standard audio visualization engine
- **Milkdrop Compatibility** - Full support for .milk preset files
- **Real-time Audio Analysis** - Frequency and beat detection
- **GPU Acceleration** - Hardware-accelerated rendering
- **Cross-platform** - OpenGL-based rendering

### Supported Preset Formats
| Format | Extension | Compatibility | Notes |
|--------|-----------|---------------|-------|
| Milkdrop | .milk | Full | Original Milkdrop presets |
| ProjectM | .prjm | Native | Enhanced ProjectM format |
| Custom | .custom | Limited | User-defined presets |

## Preset Management System

### Automatic Preset Discovery
```bash
# Specify preset directory
./vibe-sync --preset-directory /path/to/presets

# Recursive directory search
./vibe-sync --preset-recursive-search

# Filter presets by pattern
./vibe-sync --preset-pattern "electronic*"
```

### Preset Loading and Cycling

#### Random Cycling (Default)
```bash
# Enable random preset cycling
./vibe-sync --shuffle-enabled true

# Set cycle interval
./vibe-sync --preset-cycle-interval 30

# Manual next/previous
./vibe-sync --next-preset-key n --prev-preset-key p
```

#### Sequential Cycling
```bash
# Sequential mode
./vibe-sync --sequential-mode

# Custom sequence file
./vibe-sync --preset-sequence-file custom-sequence.txt
```

### Advanced Preset Features

#### Favorites System
```bash
# Mark preset as favorite
./vibe-sync --favorite-preset-key f

# View favorites only
./vibe-sync --favorites-only

# Favorite preset file location
./vibe-sync --favorites-file ~/.config/vibe-sync/favorites.json
```

**JSON Format for Favorites:**
```json
{
  "favorites": [
    {
      "path": "/path/to/preset1.milk",
      "name": "My Favorite Preset",
      "tags": ["electronic", "fast"],
      "rating": 5,
      "date_added": "2024-01-15T10:30:00Z"
    }
  ]
}
```

#### Broken Preset Management
```bash
# Automatic broken preset detection
./vibe-sync --broken-preset-detection \
    --broken-preset-directory /path/to/broken \
    --mark-broken-preset-key b

# Manual broken preset marking
./vibe-sync --mark-preset-broken /path/to/preset.milk

# Report broken preset
./vibe-sync --report-broken-preset /path/to/preset.milk \
    --error-description "Shader compilation failed"
```

**Broken Preset Detection Process:**
1. **Load preset** - Attempt to load .milk file
2. **Shader compilation** - Compile GLSL shaders
3. **Runtime execution** - Test shader execution
4. **Error detection** - Monitor stderr and stdout
5. **Automatic move** - Move broken presets to separate directory

#### Preset Blacklist System
```bash
# Add preset to blacklist
./vibe-sync --blacklist-preset /path/to/unwanted.milk

# View blacklist
./vibe-sync --view-blacklist

# Remove from blacklist
./vibe-sync --unblacklist-preset /path/to/preset.milk

# Blacklist configuration file
./vibe-sync --blacklist-file ~/.config/vibe-sync/blacklist.json
```

**Blacklist File Format:**
```json
{
  "blacklisted_presets": [
    {
      "path": "/path/to/unwanted.milk",
      "reason": "causes GPU driver crash",
      "date_added": "2024-01-15T10:30:00Z",
      "category": "broken"
    }
  ],
  "auto_blacklist": true,
  "auto_blacklist_on_crash": true
}
```

### Test Mode and Preset Isolation

#### Single Preset Testing
```bash
# Test mode with specific preset
./vibe-sync --test-mode \
    --default-preset /path/to/test.milk

# Disable preset cycling in test mode
./vibe-sync --test-mode --no-preset-cycling

# Debug mode for testing
./vibe-sync --test-mode --debug-preset-loading
```

#### Preset Validation
```bash
# Validate all presets in directory
./vibe-sync --validate-presets /path/to/presets

# Generate preset report
./vibe-sync --preset-report --output-file preset-report.json

# Check preset compatibility
./vibe-sync --check-compatibility
```

## Audio Analysis and Beat Detection

### Real-time Audio Processing
```bash
# Enable audio analysis
./vibe-sync --audio-analysis-enabled

# Configure analysis parameters
./vibe-sync --fft-size 2048 \
    --sample-rate 44100 \
    --hop-size 512

# Frequency band configuration
./vibe-sync --bass-range 20-250Hz \
    --mid-range 250-4000Hz \
    --treble-range 4000-20000Hz
```

### Beat Detection Algorithms
```bash
# Beat detection method
./vibe-sync --beat-detection energy

# Sensitivity settings
./vibe-sync --beat-sensitivity 0.8 \
    --beat-threshold 0.3 \
    --beat-hold-time 0.1

# Adaptive beat detection
./vibe-sync --adaptive-beat-detection \
    --beat-learning-rate 0.1
```

**Beat Detection Methods:**
- **Energy-based** - Detects sudden energy increases
- **Spectral flux** - Analyzes spectral changes
- **Onset detection** - Finds note onsets
- **Machine learning** - AI-powered beat detection

### Audio Reactivity Parameters
```bash
# Global reactivity
./vibe-sync --reactivity-scale 1.0

# Per-band reactivity
./vibe-sync --bass-reactivity 1.2 \
    --mid-reactivity 0.8 \
    --treble-reactivity 1.5

# Smoothing settings
./vibe-sync --react-smoothing 0.9 \
    --react-smoothing-frequency 30
```

## Visualization Modes and Styles

### Built-in Visualizer Modes
```bash
# Classic Milkdrop mode
./vibe-sync --mode milkdrop

# Modern ProjectM mode
./vibe-sync --mode projectm

# Hybrid mode
./vibe-sync --mode hybrid

# AI-enhanced mode
./vibe-sync --mode ai-enhanced
```

### Style Categories
Based on the music playing, visualizations can be categorized:

#### Electronic Music
- **High energy** presets
- **Fast transitions**
- **Neon color schemes**
- **Geometric patterns**

#### Ambient/Chill Music
- **Smooth transitions**
- **Soft color palettes**
- **Organic patterns**
- **Slow evolution**

#### Rock/Metal Music
- **Aggressive patterns**
- **High contrast colors**
- **Dynamic explosions**
- **Strobing effects**

#### Classical Music
- **Elegant patterns**
- **Harmonious colors**
- **Graceful movements**
- **Sophisticated transitions**

### Custom Visualization Profiles
```bash
# Save current visualizer settings as profile
./vibe-sync --save-visualizer-profile "Electronic Night"

# Load profile
./vibe-sync --load-visualizer-profile "Electronic Night"

# List available profiles
./vibe-sync --list-visualizer-profiles

# Auto-profile based on genre
./vibe-sync --auto-genre-profiles
```

**Profile Format:**
```json
{
  "name": "Electronic Night",
  "genre": "electronic",
  "settings": {
    "reactivity_scale": 1.2,
    "beat_sensitivity": 0.9,
    "transition_speed": 1.5,
    "color_saturation": 1.3,
    "brightness": 1.1
  },
  "preset_filters": {
    "categories": ["electronic", "techno", "house"],
    "tags": ["neon", "geometric", "fast"],
    "exclude_tags": ["ambient", "slow"]
  }
}
```

## GPU Acceleration and Performance

### GPU Settings
```bash
# Enable GPU acceleration
./vibe-sync --enable-gpu

# GPU selection (for multi-GPU systems)
./vibe-sync --gpu-device 0

# OpenGL version
./vibe-sync --opengl-version 3.3

# VSync settings
./vibe-sync --vsync-enabled true \
    --vsync-adaptation true
```

### Performance Optimization
```bash
# Frame rate limiting
./vibe-sync --max-fps 60 \
    --min-fps 24

# Resolution scaling
./vibe-sync --render-scale 0.75 \
    --internal-resolution 1280x720

# Quality settings
./vibe-sync --high-quality \
    --anti-aliasing MSAA_4x \
    --anisotropic-filtering 16x

# Performance mode
./vibe-sync --performance-mode \
    --low-memory-mode
```

### Memory Management
```bash
# Texture memory limit
./vibe-sync --texture-memory-limit 512MB

# Frame buffer optimization
./vibe-sync --fbo-optimization \
    --fbo-compression

# Garbage collection
./vibe-sync --auto-gc \
    --gc-interval 30.0
```

## Advanced Features

### AI-Powered Enhancements
```bash
# Enable AI enhancements
./vibe-sync --ai-enhanced \
    --ai-model-path /path/to/model

# AI preset selection
./vibe-sync --ai-preset-selection \
    --ai-learning-mode

# Real-time AI adjustments
./vibe-sync --ai-realtime-adjustments \
    --ai-adjustment-interval 5.0
```

### Machine Learning Integration
```bash
# Train custom AI models
./vibe-sync --train-ai-model \
    --training-data /path/to/dataset \
    --model-output /path/to/model

# Use pre-trained models
./vibe-sync --use-pretrained-model music-genre-v1

# AI model management
./vibe-sync --list-ai-models \
    --download-ai-model genre-classification
```

### Custom Shader Support
```bash
# Load custom shaders
./vibe-sync --custom-shader-path /path/to/shaders

# Shader compilation debugging
./vibe-sync --debug-shader-compilation

# Live shader editing
./vibe-sync --live-shader-editing \
    --shader-hot-reload
```

## Integration with Other Systems

### Music Player Integration
- **Metadata-based selection** - Choose presets based on genre/mood
- **Synchronized transitions** - Change presets with song changes
- **Reactive parameters** - Adjust visualization based on track features
- **Playlist visualization** - Different visualization for each track type

### Text Overlay Integration
- **Color synchronization** - Text colors match visualization
- **Position coordination** - Text placement considers visualization
- **Performance balance** - Text rendering coordinated with visualizer
- **Theme consistency** - Unified visual theme across systems

### Recording Integration
- **High-quality capture** - Preserve visual quality in recordings
- **Real-time processing** - No quality loss during recording
- **Audio sync** - Perfect synchronization with audio
- **Format optimization** - Choose optimal recording formats

### Suno AI Integration
- **Genre-aware presets** - Select appropriate presets for Suno genres
- **Mood detection** - AI-powered mood analysis for preset selection
- **Style consistency** - Maintain visual style across Suno tracks
- **Learning system** - Improve preset selection over time

## Debugging and Troubleshooting

### Visualizer Debug System
```bash
# Enable visualizer debugging
./vibe-sync --debug-visualization \
    --debug-preset-loading \
    --debug-shader-compilation \
    --debug-audio-analysis

# Performance monitoring
./vibe-sync --debug-performance \
    --performance-interval 1.0

# Memory tracking
./vibe-sync --debug-memory \
    --memory-tracking-interval 5.0
```

### Common Issues and Solutions

#### Black Screen
```bash
# Diagnose graphics issues
./vibe-sync --diagnose-graphics

# Fallback to software rendering
./vibe-sync --software-rendering

# Check OpenGL support
./vibe-sync --check-opengl
```

#### Poor Performance
```bash
# Reduce quality settings
./vibe-sync --low-quality \
    --render-scale 0.5 \
    --max-fps 30

# Disable GPU features
./vibe-sync --disable-gpu \
    --disable-vsync

# Monitor performance
./vibe-sync --performance-monitor
```

#### Preset Loading Failures
```bash
# Validate preset files
./vibe-sync --validate-presets

# Reset preset cache
./vibe-sync --reset-preset-cache

# Verbose error reporting
./vibe-sync --verbose-errors
```

### Performance Metrics
Monitor these key metrics:
- **FPS** - Frames per second
- **GPU Usage** - Graphics card utilization
- **Memory Usage** - RAM consumption
- **Audio Latency** - Delay between audio and visuals
- **Shader Compilation Time** - Time to compile presets
- **Preset Load Time** - Time to switch presets

## Advanced Configuration

### TOML Configuration
Create `~/.config/vibe-sync/visualizer.toml`:

```toml
# Visualizer Configuration
[visualizer]
enabled = true
mode = "projectm"
gpu_acceleration = true

[visualizer.presets]
directory = "/usr/share/vibe-sync/presets"
recursive_search = true
shuffle_enabled = true
cycle_interval = 30
favorites_enabled = true
broken_detection = true

[visualizer.audio]
analysis_enabled = true
fft_size = 2048
beat_detection = "energy"
reactivity_scale = 1.0

[visualizer.performance]
max_fps = 60
render_scale = 1.0
quality = "high"
vsync = true

[visualizer.ai]
enhanced_mode = false
preset_selection = false
learning_mode = false

[visualizer.debug]
enabled = false
performance_monitoring = false
memory_tracking = false
shader_debugging = false
```

### Environment Variables
```bash
# Override default settings via environment
export VIBESYNC_PRESET_DIR="/custom/presets"
export VIBESYNC_GPU_ENABLED="true"
export VIBESYNC_DEBUG_LEVEL="Info"
export VIBESYNC_CACHE_DIR="/tmp/vibe-sync-cache"
```

## Keyboard Shortcuts

| Key | Function |
|-----|----------|
| N | Next random preset |
| P | Previous preset |
| F | Toggle favorite |
| B | Mark preset as broken |
| R | Reset to default preset |
| 1-9 | Direct preset selection |
| G | Toggle GPU acceleration |
| Q | Cycle quality settings |
| Space | Pause/resume animation |
| + | Increase reactivity |
| - | Decrease reactivity |
| [ | Previous visualization mode |
| ] | Next visualization mode |

## Future Enhancements

Planned visualization features:

1. **VR/AR Support** - Immersive visualization experiences
2. **Multi-user Sync** - Shared visualization sessions
3. **Cloud Presets** - Online preset sharing and discovery
4. **Advanced AI** - Real-time preset generation
5. **Custom Shaders** - User-defined GLSL effects
6. **3D Visualization** - Three-dimensional visual effects
7. **Interactive Controls** - Live parameter adjustment
8. **Visual Analytics** - Visualization performance metrics

For more technical details, see the [Developer Guide](../developer/debug-systems.md) and [API Reference](../developer/api-reference.md).