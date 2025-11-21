# Quick Start Guide

Welcome to VibeSync! This guide will help you get started with the debug-production branch, which includes comprehensive debug capabilities and advanced music visualizer features.

## Prerequisites

### System Requirements
- **Operating System**: Linux (Ubuntu 20.04+ recommended, Arch Linux compatible)
- **Graphics**: OpenGL 3.3+ support
- **Audio**: ALSA/PulseAudio/PipeWire support
- **Storage**: 500MB for application, additional space for audio/video files

### Required Dependencies
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install qt6-base-dev libprojectm-dev ffmpeg cmake make g++ \
    libfftw3-dev libfreetype6-dev libfontconfig1-dev

# Arch Linux
sudo pacman -S --needed qt6-base projectm ffmpeg cmake make fftw freetype2 fontconfig

# Dependencies for debug features
sudo apt install libspdlog-dev nlohmann-json3-dev
# OR for Arch:
sudo pacman -S --needed spdlog nlohmann-json
```

### Audio System Setup
For system audio recording, you may need to install:
```bash
# PipeWire (recommended for modern systems)
sudo apt install pipewire pipewire-alsa pipewire-pulse

# OR PulseAudio (for older systems)
sudo apt install pulseaudio pulseaudio-utils
```

## Installation

### Building from Source (Debug-Production Branch)
```bash
# Clone the repository
git clone https://github.com/Nsomnia/vibe-sync.git
cd vibe-sync

# Checkout debug-production branch
git checkout debug-production

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build
make -j$(nproc)

# Install (optional)
sudo make install
```

## Basic Usage

### Starting VibeSync
```bash
# Standard mode with debug logging
./vibe-sync

# With specific audio files
./vibe-sync /path/to/audio1.mp3 /path/to/audio2.flac

# Enable full debug output
./vibe-sync --debug-all

# Start with specific preset for testing
./vibe-sync --default-preset

# Full debug mode with network debugging
./vibe-sync --debug-network --debug-file
```

### Command Line Options
```bash
# Display help
./vibe-sync --help

# Window settings
./vibe-sync --width 1920 --height 1080 --fps 60

# Text overlay customization
./vibe-sync --font-path /usr/share/fonts/TTF/DejaVuSans-Bold.ttf \
    --song-info-font-size 48 --url-font-size 32

# Recording settings
./vibe-sync --record-video --output-directory ~/Videos/VibeSync

# Performance settings
./vibe-sync --max-memory 2GB --thread-pool-size 8
```

### Keyboard Shortcuts
- **Space** - Play/Pause music
- **N** - Next random preset
- **P** - Previous preset
- **R** - Start/Stop recording
- **F** - Toggle fullscreen
- **S** - Open settings window
- **D** - Toggle debug panel
- **Q** - Quit application

## First Time Setup

### 1. Audio Setup
1. Launch VibeSync
2. Go to **Settings** → **Audio**
3. Select your preferred audio source:
   - **System Audio** - Uses PipeWire/PulseAudio
   - **File Input** - Plays local audio files
   - **Suno AI** - Downloads from Suno.com (requires API key)
4. Test audio playback

### 2. Visualizer Configuration
1. **Settings** → **Visualizer**
2. Choose ProjectM preset directory (defaults provided)
3. Adjust sensitivity and beat detection
4. Enable GPU acceleration if available
5. Test with different presets

### 3. Text Overlays
1. **Settings** → **Text & Font**
2. Configure title/artist display
3. Set font files and sizes
4. Customize colors and borders
5. Enable animation effects

### 4. Recording Setup
1. **Settings** → **Recording**
2. Choose video output directory
3. Configure FFmpeg encoding settings
4. Set audio input mode (PipeWire recommended)
5. Test recording functionality

## Music Player Basics

### Loading Audio Files
- **Menu** → **File** → **Add Audio Files**
- Drag and drop files into the playlist
- Create playlists from the **Playlist** menu
- Import from **Suno.com** (if configured)

### Playlist Management
- View current playlist in the sidebar
- Reorder tracks by dragging
- Remove tracks with delete key
- Save/load playlist files (.m3u, .pls)

### Playback Controls
- **Play/Pause** - Space bar or play button
- **Next/Previous** - Arrow keys or skip buttons
- **Shuffle** - Shuffle button
- **Repeat** - Repeat modes (off/one/all)
- **Volume** - Slider or audio keys

### Format Support
- **Audio**: MP3, FLAC, WAV, OGG, M4A, AAC
- **Playlist**: M3U, M3U8, PLS, XSPF
- **Metadata**: ID3v1, ID3v2, Vorbis Comments

## Visualizer Features

### Preset Management
1. **Browse Presets** - Scroll through available .milk files
2. **Favorites** - Press 'F' to mark/unmark favorites
3. **History** - Press 'P' to go back to previous presets
4. **Blacklist** - Broken presets are automatically moved

### Customization
- **Beat Detection** - Adjust sensitivity for different music
- **Smoothness** - Control transition speed between presets
- **Performance** - Monitor FPS and memory usage
- **GPU Settings** - Enable/disable GPU acceleration

### Test Mode
For testing and debugging:
```bash
# Start with default preset only
./vibe-sync --default-preset

# Single preset mode (no cycling)
./vibe-sync --preset-path /path/to/specific.milk
```

## Text Overlays

### Basic Text Display
- **Song Title** - Automatically extracted from metadata
- **Artist Name** - Displayed below title
- **Custom Text** - Additional text overlay
- **URL/Branding** - Static overlay in corners

### Animation Types
- **Fade In/Out** - Smooth appearance and disappearance
- **Bounce** - Text bounces around the screen
- **Slide** - Text slides in from edges
- **Typewriter** - Text appears character by character

### Customization Options
- **Fonts** - TTF/OTF font file support
- **Colors** - Hex color codes for text and borders
- **Sizes** - Independent size control for different text types
- **Positions** - Configurable placement and alignment
- **Timing** - Animation duration and delay settings

## Video Recording

### Basic Recording
1. **Settings** → **Recording** → **Enable Recording**
2. Choose output directory and filename format
3. Select audio input mode (PipeWire recommended)
4. Press 'R' to start/stop recording
5. Videos saved to specified directory

### FFmpeg Configuration
Advanced users can customize encoding:
```bash
# High quality H.264
./vibe-sync --ffmpeg-command "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset slow -crf 18 -c:a aac -b:a 192k {OUTPUT_PATH}"

# HEVC for smaller file sizes
./vibe-sync --ffmpeg-command "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx265 -crf 28 -preset medium -c:a aac -b:a 192k {OUTPUT_PATH}"
```

### Audio Sources
- **System Audio** - Records from speakers/headphones
- **File Audio** - Records from played audio files
- **Mixed Audio** - Combines system and file audio

## Debug Features (Debug-Production Branch)

### Debug Categories
The debug system includes 11 categories:
- **Core** - Main application functionality
- **Audio** - Music player and audio processing
- **Visualization** - ProjectM and visualizer operations
- **UI** - User interface interactions
- **Performance** - Resource usage and optimization
- **Network** - External API calls and integrations
- **Database** - Data persistence and storage
- **Security** - Authentication and permissions
- **Plugin** - Plugin loading and management
- **Integration** - External service integration
- **Custom** - User-defined debug categories

### Debug Levels
- **Trace** - Detailed execution flow
- **Debug** - General debugging information
- **Info** - Important events and status
- **Warning** - Potential issues
- **Error** - Error conditions
- **Fatal** - Critical system failures

### Debug Usage
```bash
# Enable specific category
./vibe-sync --debug-audio --debug-visualization

# Save debug to file
./vibe-sync --debug-file --debug-log-path /tmp/vibesync.log

# Network debug server
./vibe-sync --debug-network --debug-port 12345

# Debug UI panel
./vibe-sync --debug-ui

# Performance monitoring
./vibe-sync --debug-performance
```

### Debug UI Panel
Press 'D' to toggle the debug panel, which shows:
- Current debug level and categories
- Real-time log output
- Performance metrics
- Error statistics
- Plugin status

## Troubleshooting

### Audio Issues
**No audio output:**
- Check system audio settings
- Verify ALSA/PulseAudio/PipeWire status
- Restart audio system: `pulseaudio --kill && pulseaudio --start`

**Recording fails:**
- Check FFmpeg installation: `ffmpeg -version`
- Verify write permissions to output directory
- Try different audio input mode

### Visualizer Issues
**Black screen or crashes:**
- Update graphics drivers
- Disable GPU acceleration: `./vibe-sync --no-gpu`
- Check OpenGL support: `glxinfo | grep OpenGL`

**Poor performance:**
- Reduce resolution: `./vibe-sync --width 1024 --height 768`
- Lower frame rate: `./vibe-sync --fps 30`
- Close other GPU-intensive applications

### Debug System Issues
**Debug output too verbose:**
- Adjust debug levels: `./vibe-sync --debug-level Info`
- Disable specific categories: `./vibe-sync --no-debug-core`

**Debug panel not responding:**
- Restart with default settings: `./vibe-sync --reset-settings`
- Check Qt dependencies are properly installed

## Next Steps

Now that you've completed the basic setup:

1. **Explore Features** - Check out the [Music Player Guide](music-player.md)
2. **Customize Visuals** - Read the [Visualizations Guide](visualizations.md)
3. **Create Videos** - Learn about [Video Recording](video-recording.md)
4. **Advanced Text** - Master [Text Overlays](text-overlays.md)
5. **Preset Management** - Study [Presets Guide](presets.md)

For developers and AI agents:
- [Debug Systems Guide](../developer/debug-systems.md)
- [Plugin Development Guide](../developer/plugin-development.md)
- [API Reference](../developer/api-reference.md)

## Getting Help

- **Documentation**: All documentation in the `/docs` directory
- **GitHub Issues**: Report bugs and request features
- **Community**: Join discussions in the repository
- **Debug Logs**: Always include debug logs when reporting issues

Happy music visualizing! 🎵✨