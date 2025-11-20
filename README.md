# vibe-sync

AI-powered music video creator using projectM Linux audio visualizer as the base. Generate stunning synchronized music visualizations with AI intelligence.

## Overview

vibe-sync is a sophisticated C++/Qt music player that transforms your audio experience into mesmerizing visual content. Built on top of the legendary projectM visualizer, it provides professional-grade video recording capabilities with advanced text overlay systems.

## Features

### 🎵 Core Functionality
- **ProjectM Integration**: Full projectM visualizer support with custom preset management
- **Smart Playlist**: Drag-and-drop music loading with metadata extraction
- **High-Quality Recording**: FFmpeg-powered video capture with customizable encoding
- **Advanced Text Engine**: Resolution-independent text overlays with animation effects

### 🎨 Visual Customization
- **Resolution Independent**: Text scales perfectly from 720p to 4K
- **Animation Effects**: Breathing text, sliding effects, and custom positioning
- **Watermark System**: Persistent branding with customizable text and position
- **Preset Management**: Favorites, blacklist, and quarantine system for presets

### 🛠️ Developer Features
- **Debug Console**: Real-time logging with color-coded output
- **Address Sanitizer**: Memory leak detection and debugging support
- **Settings System**: Persistent configuration with QSettings
- **Modular Architecture**: Clean separation of concerns

## Installation

### Prerequisites (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install qt6-base-dev libprojectm-dev libtag1-dev ffmpeg build-essential cmake
```

### Build Instructions
```bash
# Clone and build
git clone <repository-url>
cd vibe-sync
mkdir build && cd build

# Configure with sanitizers (optional)
cmake .. -DENABLE_SANITIZERS=ON

# Build
make -j$(nproc)

# Run
./NeonVisualizer
```

### Dependencies
- **Qt6**: Modern GUI framework
- **projectM**: Audio visualization engine
- **TagLib**: Audio metadata extraction
- **FFmpeg**: Video encoding and processing

## Usage

### Basic Workflow
1. **Load Music**: Use File > Add Files or drag audio files into the playlist
2. **Select Preset**: Browse through visualizer presets using the control panel
3. **Customize Text**: Configure watermarks and text overlays in Settings
4. **Record**: Click "Start Recording" to capture your music video

### Controls
- **Playlist Dock**: Left panel with drag-and-drop music loading
- **Visualizer Controls**: Right panel with preset management and recording
- **Debug Console**: Bottom panel with real-time system logging

### Text Overlay System
The resolution-independent text engine supports:
- **Relative Positioning**: Use 0.0-1.0 coordinates for screen-independent placement
- **Animation Effects**: Breathing, sliding, and custom timing
- **Global Scaling**: Adjust all text sizes uniformly
- **Multiple Elements**: Simultaneous artist info and watermarks

## Project Structure

```
src/
├── core/           # Core utilities
│   ├── Logger.h/cpp       # Debug logging system
│   ├── StringUtils.h      # String manipulation utilities
│   ├── PathUtils.h        # Path handling utilities
│   └── TextFormatter.h    # Audio metadata parsing
├── engine/         # Core engines
│   ├── TextEngine.h/cpp   # Text rendering system
│   ├── AudioEngine.h/cpp  # Audio playback
│   ├── VideoRecorder.h/cpp # Video capture
│   ├── PresetManager.h/cpp # Preset management
│   ├── PlaylistManager.h/cpp # Playlist handling
│   └── VizEngine.h/cpp    # ProjectM wrapper
├── ui/             # User interface
│   ├── MainWindow.h/cpp   # Main application window
│   ├── widgets/          # Custom UI components
│   ├── menus/           # Menu bar implementation
│   └── dialogs/         # Settings and configuration
└── data/           # Data management
    └── SettingsManager.h/cpp # Configuration system
```

## Configuration

### Settings Dialog
- **Visualizer**: Preset paths, mesh size, FPS, beat sensitivity
- **Typography**: Text overlays, watermarks, scaling options
- **Recording**: FFmpeg command templates and encoding settings

### Key Settings
```ini
[viz]
preset_path=/usr/share/projectM/presets
shuffle=true
fps=60

[overlay]
watermark_visible=false
watermark_text=your-site.com
global_scale=1.0

[recording]
ffmpeg_cmd=ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt bgra -s {WIDTH}x{HEIGHT} -r {FPS} -i - -c:v libx264 -preset ultrafast -crf 18 {OUTPUT}
```

## Advanced Features

### Debug Mode
Enable the debug console for real-time system monitoring:
- Color-coded log levels (Debug, Info, Warning, Critical)
- Automatic scrolling and line highlighting
- Integration with Qt's message system

### Memory Safety
Built-in Address Sanitizer support for development:
```bash
cmake .. -DENABLE_SANITIZERS=ON
./NeonVisualizer
```

### Preset Management
- **Favorites**: Mark preferred presets for quick access
- **Blacklist**: Permanently hide unwanted presets
- **Quarantine**: Temporary exclusion with manual review
- **Smart Filtering**: Automatic preset validation and cleanup

## Troubleshooting

### Common Issues
1. **No Presets Found**: Ensure projectM presets are installed
   ```bash
   sudo apt install projectm-data
   ```
2. **Recording Fails**: Check FFmpeg installation and permissions
3. **Audio Issues**: Verify Qt Multimedia backend support

### Debug Information
The debug console provides detailed runtime information:
- Preset loading status
- Audio engine state
- Recording progress
- Memory usage statistics

## Development

### Architecture
- **Model-View Pattern**: Clear separation of data and presentation
- **Signal-Slot Communication**: Qt's reactive programming model
- **Resource Management**: RAII principles and smart pointers
- **Thread Safety**: Mutex protection for concurrent access

### Adding Features
1. **New Text Elements**: Extend `TextEngine` with custom animations
2. **Custom Presets**: Add preset parsing for new visualizer formats
3. **Recording Options**: Modify `VideoRecorder` for additional formats
4. **UI Components**: Create new dock widgets for specialized views

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/new-feature`
3. Make changes with proper testing
4. Enable sanitizers for memory safety: `cmake .. -DENABLE_SANITIZERS=ON`
5. Submit a pull request with detailed description

## License

This project is open source. Please see LICENSE file for details.

## Acknowledgments

- **projectM Team**: For the legendary audio visualizer engine
- **Qt Framework**: For the robust cross-platform UI toolkit
- **FFmpeg**: For comprehensive multimedia processing
- **TagLib**: For reliable audio metadata extraction

---

**vibe-sync** - Where AI meets music visualization 🎵✨