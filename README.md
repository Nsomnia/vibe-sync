# Vibe-Sync

AI-powered music visualizer with advanced text overlay system and video recording capabilities.

## Overview

Vibe-Sync is a sophisticated C++ application that transforms your music into stunning visual experiences using ProjectM audio visualization. It combines the legendary ProjectM visualizer with modern AI features, advanced text overlay systems, and professional-grade video recording capabilities.

## ✨ Features

### 🎵 Core Functionality
- **ProjectM Integration**: Full audio visualization with custom preset support
- **Smart Music Player**: Drag-and-drop playlist with metadata extraction
- **High-Quality Recording**: FFmpeg-powered video capture with customizable encoding
- **Advanced Text Engine**: Resolution-independent text overlays with animation effects

### 🎨 Visual Customization
- **Resolution Independent**: Perfect scaling from 720p to 4K
- **Animation Effects**: Breathing text, sliding effects, custom positioning
- **Watermark System**: Persistent branding with customizable text and position
- **Preset Management**: Favorites, blacklist, and quarantine system

### 🛠️ Developer Features
- **Debug Console**: Real-time logging with color-coded output
- **Address Sanitizer**: Memory leak detection and debugging support
- **Configuration System**: Persistent settings with TOML support
- **Modular Architecture**: Clean separation of concerns

## 🚀 Quick Start

### Automatic Build (Recommended)
```bash
# Clone the repository
git clone <repository-url>
cd vibe-sync

# Run the automated build script
bash build.sh
```

### Manual Build
```bash
# If you have CMake and dependencies:
mkdir build && cd build
cmake .. && make -j$(nproc)

# If you only have Make:
make
```

### Test the Build
```bash
# Check version and features
./vibe-sync --version

# Verify system dependencies
./vibe-sync --check-deps

# Get detailed build information
./vibe-sync --build-info

# Show help
./vibe-sync --help
```

## 📋 Prerequisites

### For Full Functionality
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install qt6-base-dev qt6-multimedia-dev libprojectm-dev \
                 libtag1-dev ffmpeg build-essential cmake pkg-config \
                 libsdl2-dev libasound2-dev libfreetype6-dev

# RHEL/CentOS
sudo dnf install qt6-qtbase-devel qt6-qtmultimedia-devel projectM-devel \
                 taglib-devel ffmpeg-devel gcc-c++ make cmake pkg-config \
                 SDL2-devel alsa-lib-devel freetype-devel
```

### Minimal Build (Console Mode)
- **Compiler**: GCC 10+ or Clang 10+
- **C++ Standard**: C++20
- **Build Tools**: Make or CMake

The application will automatically adapt to available dependencies:
- ✅ **Full GUI**: With Qt6, ProjectM, and dependencies
- ✅ **Console Mode**: Basic functionality without GUI
- ✅ **Minimal Build**: Core features only

## 🏗️ Build System

### Supported Build Methods

1. **Automated Build Script** (`build.sh`)
   - Detects available dependencies
   - Attempts to install missing packages
   - Falls back to minimal build if needed

2. **CMake** (When available)
   ```bash
   mkdir build && cd build
   cmake ..
   make -j$(nproc)
   ```

3. **Makefile** (Fallback)
   ```bash
   make
   ```

### Dependency Detection

The build system automatically detects and adapts to available packages:

| Feature | Required Package | Build Status |
|---------|------------------|--------------|
| GUI Framework | Qt6 | Optional |
| Visualizer | ProjectM | Optional |
| Config Parsing | TOML++ | Optional |
| Audio Processing | SDL2, ALSA | Optional |
| Video Recording | FFmpeg | Optional |
| Metadata | TagLib | Optional |

## 🔧 Configuration

### Default Configuration Files
- `config/default-settings.toml` - Application settings
- `config/audio-presets.toml` - Audio processing presets  
- `config/video-presets.toml` - Video recording presets
- `config/help.html` - User help documentation

### Command Line Options
```bash
./vibe-sync --help           # Show help
./vibe-sync --version        # Show version
./vibe-sync --check-deps     # Check dependencies
./vibe-sync --build-info     # Detailed build info
```

### Environment Variables
```bash
# Override config directory
export VIBE_SYNC_CONFIG_DIR="/path/to/config"

# Enable debug mode
export VIBE_SYNC_DEBUG=1
```

## 🎯 Usage Examples

### Basic Usage
1. **Load Music**: Drag audio files into the playlist
2. **Select Preset**: Browse visualizer presets
3. **Configure Text**: Set up watermarks and overlays
4. **Record Video**: Capture your music visualization

### Advanced Recording
```bash
# High quality recording
./vibe-sync --preset "crown" --quality ultra

# Custom output location
./vibe-sync --output "/path/to/video.mp4"

# Debug mode
./vibe-sync --debug --log-level 5
```

## 🏛️ Project Structure

```
vibe-sync/
├── src/                     # Source code
│   ├── main.cpp            # Application entry point
│   ├── core/               # Core utilities and systems
│   ├── engine/             # Audio/visual engines
│   ├── ui/                 # User interface components
│   ├── integrations/       # External library integrations
│   ├── utils/              # Utility functions
│   └── debug/              # Debug and monitoring
├── config/                  # Configuration files
├── assets/                  # Application assets
├── docs/                    # Documentation
├── build.sh                # Automated build script
├── Makefile                # Fallback build system
└── CMakeLists.txt          # CMake build configuration
```

## 🐛 Troubleshooting

### Build Issues

**CMake not found:**
```bash
# Install CMake
sudo apt install cmake

# Or use Makefile fallback
make
```

**Qt6 not found:**
```bash
# Install Qt6 development packages
sudo apt install qt6-base-dev qt6-multimedia-dev

# Build will continue in console mode
```

**ProjectM not found:**
```bash
# Install ProjectM
sudo apt install libprojectm-dev

# Visualizer will be disabled
```

### Runtime Issues

**Permission denied on build script:**
```bash
# Run with bash directly
bash build.sh
```

**Missing dependencies at runtime:**
```bash
# Check what's available
./vibe-sync --check-deps

# Install missing packages
sudo apt install <package-name>
```

### Common Solutions

1. **Clean rebuild:**
   ```bash
   rm -rf build
   bash build.sh
   ```

2. **Check compiler version:**
   ```bash
   g++ --version  # Need C++20 support
   ```

3. **Verify library paths:**
   ```bash
   pkg-config --list-all | grep -E "(qt6|projectM|ffmpeg)"
   ```

## 🧪 Testing

### Build Verification
```bash
# Test basic functionality
./vibe-sync --version
./vibe-sync --check-deps

# Test all features
make test  # (when tests are available)
```

### Debug Mode
```bash
# Enable all debug output
./vibe-sync --debug --log-level 5

# Check memory leaks
valgrind ./vibe-sync --test-mode
```

## 🚧 Development

### Adding Dependencies

1. **Update CMakeLists.txt** to find the new package
2. **Modify main.cpp** to use conditional compilation
3. **Test build** with and without the dependency

### Code Style
- Follow C++20 standards
- Use RAII principles
- Implement proper error handling
- Add comprehensive logging

### Contribution Guidelines
1. Fork the repository
2. Create feature branch: `git checkout -b feature-name`
3. Test with minimal dependencies
4. Update documentation
5. Submit pull request

## 📄 License

This project is open source. See LICENSE file for details.

## 🙏 Acknowledgments

- **ProjectM Team** - Audio visualization engine
- **Qt Framework** - Cross-platform UI toolkit  
- **FFmpeg** - Multimedia processing
- **MiniMax** - AI integration and support

---

**Vibe-Sync** - AI-powered music visualization platform 🎵✨

For support and questions, please check the troubleshooting section or create an issue.