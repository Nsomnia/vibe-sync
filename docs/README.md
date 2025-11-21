# VibeSync Documentation

## Overview

VibeSync is a comprehensive AI-powered music visualizer platform built on the debug-production branch with advanced debug capabilities, music player functionality, and AI-driven visualization features. This documentation provides complete guidance for users, developers, and AI agents working with the system.

## Documentation Structure

### 📖 User Guides
- **[Quick Start Guide](user/quick-start.md)** - Get started with VibeSync
- **[Music Player Guide](user/music-player.md)** - Using the music player features
- **[Visualization Guide](user/visualizations.md)** - Working with ProjectM visualizer
- **[Video Recording Guide](user/video-recording.md)** - Recording and exporting videos
- **[Text Overlays Guide](user/text-overlays.md)** - Text animations and overlays
- **[Presets Guide](user/presets.md)** - Managing and customizing visualizer presets

### 🔧 Developer Documentation
- **[Debug Systems Guide](developer/debug-systems.md)** - Understanding debug capabilities
- **[Plugin Development Guide](developer/plugin-development.md)** - Creating plugins
- **[API Reference](developer/api-reference.md)** - Complete API documentation
- **[Build Instructions](developer/build-instructions.md)** - Building from source
- **[Contributing Guide](developer/contributing.md)** - How to contribute to the project

### 🎵 Feature Documentation
- **[Music Player Features](features/music-player.md)** - Detailed music player capabilities
- **[AI Visualizer Features](features/ai-visualizer.md)** - AI-powered visualization system
- **[Text Animation System](features/text-animation.md)** - Dynamic text rendering and animations
- **[Preset Management](features/preset-management.md)** - Advanced preset handling
- **[Video Recording System](features/video-recording.md)** - Complete recording capabilities
- **[Suno AI Integration](features/suno-integration.md)** - Suno.com integration features

### 🔬 Debug & Development
- **[Debug Features Overview](debug/overview.md)** - Complete debug system overview
- **[Debug Categories](debug/categories.md)** - Available debug categories
- **[Performance Monitoring](debug/performance.md)** - Performance tracking and analysis
- **[Error Handling](debug/error-handling.md)** - Comprehensive error management
- **[Plugin System](debug/plugin-system.md)** - Plugin debugging and management

### 🎯 Configuration
- **[Configuration Files](config/configuration.md)** - TOML configuration system
- **[Command Line Options](config/cli-options.md)** - Complete CLI reference
- **[Settings GUI](config/settings-gui.md)** - Settings window documentation
- **[Default Configurations](config/defaults.md)** - Default configuration files

## Quick Navigation

### For End Users
1. Start with the **[Quick Start Guide](user/quick-start.md)**
2. Explore **[Music Player](user/music-player.md)** and **[Visualizations](user/visualizations.md)**
3. Learn about **[Video Recording](user/video-recording.md)** and **[Text Overlays](user/text-overlays.md)**
4. Master **[Preset Management](user/presets.md)**

### For Developers
1. Review the **[Debug Systems Guide](developer/debug-systems.md)**
2. Study **[Plugin Development](developer/plugin-development.md)**
3. Check the **[API Reference](developer/api-reference.md)**
4. Follow **[Build Instructions](developer/build-instructions.md)**

### For AI Agents
1. Start with **[Debug Features Overview](debug/overview.md)**
2. Explore **[Plugin System](debug/plugin-system.md)**
3. Review **[Performance Monitoring](debug/performance.md)**
4. Study **[Error Handling](debug/error-handling.md)**

## Key Features

### 🎵 Music Player
- **Qt Multimedia Integration** - Professional audio engine
- **Format Support** - MP3, FLAC, WAV, and more
- **Playlist Management** - Advanced queue and playlist features
- **Metadata Support** - Rich metadata extraction and display
- **Crossfade & Gapless** - Professional playback features

### 🎨 AI Visualizer
- **ProjectM Engine** - Industry-standard visualization engine
- **Real-time Analysis** - Frequency and beat detection
- **GPU Acceleration** - High-performance rendering
- **Customizable Parameters** - Fine-tuned visual experience

### 🎬 Video Recording
- **FFmpeg Integration** - Professional video encoding
- **System Audio Capture** - PipeWire/PulseAudio support
- **Custom Encoding** - Configurable video and audio codecs
- **Batch Processing** - Automated video generation

### ✨ Text System
- **Layer-based Compositor** - Professional text rendering
- **Dynamic Animations** - Intro/main/outro sequences
- **Custom Fonts** - FreeType text rendering
- **Static Overlays** - URL and branding elements

### 🎛️ Preset Management
- **Milkdrop Compatibility** - Full .milk preset support
- **Favorites System** - User-customizable favorites
- **Broken Preset Detection** - Automatic shader error handling
- **Blacklist System** - Custom preset filtering
- **Test Mode** - Single preset testing

### 🔧 Debug Systems
- **Comprehensive Logging** - 11 debug categories with 6 severity levels
- **Performance Monitoring** - Real-time CPU/memory tracking
- **Error Recovery** - Automatic error handling and recovery
- **Plugin Debugging** - Full plugin lifecycle debugging

### 🤖 AI Integration
- **Suno.com Integration** - Automated song downloading
- **Lyrics Detection** - Local Whisper speech-to-text
- **Smart Metadata** - Enhanced song information
- **Karaoke Support** - Synchronized lyrics display

## System Architecture

The debug-production branch follows a modular architecture:

```
src/
├── core/                    # Core systems (debug, error, performance, plugin)
├── audio/                   # Music player and audio processing
├── visualizer/              # ProjectM integration and AI visualizer
├── recording/               # Video recording and FFmpeg integration
├── text/                    # Text overlay and animation system
├── preset/                  # Preset management and handling
├── ui/                      # Qt GUI components
└── integration/             # Suno AI and external integrations
```

## Getting Started

### For End Users
- Download or build VibeSync from the debug-production branch
- Launch with `./vibe-sync --help` to see command-line options
- Start with your favorite audio files and explore the preset system
- Check the **[Quick Start Guide](user/quick-start.md)** for detailed instructions

### For Developers
- Clone the debug-production branch
- Follow the **[Build Instructions](developer/build-instructions.md)**
- Explore the **[Debug Systems](developer/debug-systems.md)**
- Review the **[Plugin Development Guide](developer/plugin-development.md)**

### For AI Agents
- Review the debug systems and plugin architecture
- Understand the performance monitoring and error handling
- Study the event-driven communication system
- Explore the modular codebase structure

## Support and Contributing

- **Issues**: Report bugs and feature requests on GitHub
- **Discussions**: Join community discussions
- **Contributing**: Follow the **[Contributing Guide](developer/contributing.md)**
- **Documentation**: Help improve this documentation

## License

VibeSync is licensed under the MIT License. See the main repository for details.