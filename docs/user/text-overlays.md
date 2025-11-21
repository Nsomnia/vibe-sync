# Text Overlays Guide

VibeSync includes a powerful text overlay system inspired by your aurora-projectm-visualizer work. This system provides dynamic text animations, customizable fonts, and professional-quality text rendering with framebuffer support.

## System Architecture

### Layer-Based Compositor
The text system uses a layer-based rendering pipeline:

```
┌─────────────────────────────────────────────────┐
│              Final Output                       │
├─────────────────────────────────────────────────┤
│ ┌─────────────────────────────────────────────┐ │
│ │           Text Overlay Layer                 │ │
│ │ ┌─────────────────────────────────────────┐ │ │
│ │ │      FreeType Text Rendering             │ │ │
│ │ │  Title | Artist | URL | Custom Text      │ │ │
│ │ │                                         │ │ │
│ │ │  Animations: Fade | Bounce | Slide      │ │ │
│ │ └─────────────────────────────────────────┘ │ │
│ └─────────────────────────────────────────────┘ │
├─────────────────────────────────────────────────┤
│ ┌─────────────────────────────────────────────┐ │
│ │        ProjectM Visualizer Layer             │ │
│ │         (Background Visualization)           │ │
│ └─────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────┘
```

### Text Types
1. **Song Information** - Title and artist name overlay
2. **Custom Text** - User-defined text elements
3. **Static Overlays** - URL/branding elements
4. **Animated Text** - Dynamic text effects
5. **Metadata Text** - Track information display

## Basic Configuration

### Font Setup
```bash
# Command line
./vibe-sync --font-path /usr/share/fonts/TTF/DejaVuSans-Bold.ttf \
    --url-font-path /usr/share/fonts/TTF/DejaVuSans.ttf

# GUI Settings
Settings → Text & Font → Font Files
```

### Basic Text Display
```bash
# Enable text overlays
./vibe-sync --enable-text-overlays

# Customize text content
./vibe-sync --custom-title "My Song Title" \
    --custom-artist "Artist Name" \
    --custom-url "https://mysite.com"

# Hide specific text elements
./vibe-sync --hide-title --hide-artist --show-url
```

### Font Settings
- **Font Path** - TTF/OTF font file location
- **Font Size** - Point size for different text types
- **Font Weight** - Normal, Bold, Light, etc.
- **Font Style** - Regular, Italic, etc.

## Text Animation System

### Animation States
The text system implements three main animation states based on your aurora design:

#### 1. Intro Sequence
- **Duration**: 2-5 seconds
- **Effect**: Text fades in from transparent
- **Position**: Centered on screen
- **Opacity**: 0% → 100%
- **Size**: Starts at 80%, grows to 100%

#### 2. Main Sequence
- **Duration**: Variable based on song length
- **Effect**: Text becomes semi-transparent and animates
- **Position**: Moves/bounces around screen
- **Opacity**: 60-80% (configurable)
- **Audio Reactive**: Position responds to music

#### 3. Outro Sequence
- **Duration**: 2-5 seconds before song ends
- **Effect**: Text returns to center and fades out
- **Position**: Returns to center
- **Opacity**: 60% → 0%
- **Size**: Remains at 100%

### Animation Types

#### Fade Animation
```bash
# Configure fade timing
./vibe-sync --transition-time 3.0 \
    --pre-fade-delay 8.0 \
    --fade-to-min-duration 4.0
```

**Parameters:**
- **Transition Time** - Duration of fade effects (default: 3.0s)
- **Pre-fade Delay** - Time at full opacity before fading (default: 8.0s)
- **Fade-to-min Duration** - Time to fade to minimum transparency (default: 4.0s)
- **Min Transparency** - Minimum opacity during main sequence (default: 0.15)

#### Bounce Animation
```bash
# Configure bounce settings
./vibe-sync --bounce-duration 5.0 \
    --bounce-speed 67.0 \
    --bounce-height 50.0
```

**Parameters:**
- **Bounce Duration** - How long text bounces before fading (default: 7.0s)
- **Bounce Speed** - Speed of bouncing text in pixels/second (default: 67.0)
- **Bounce Height** - Maximum bounce height in pixels (default: 50.0)

#### Slide Animation
```bash
# Configure slide effects
./vibe-sync --slide-direction in-from-left \
    --slide-distance 200 \
    --slide-ease-out
```

#### Typewriter Effect
```bash
# Enable typewriter effect
./vibe-sync --typewriter-enabled \
    --typewriter-speed 50 \
    --typewriter-delay 1.0
```

### Audio Reactive Animations
Text can react to the audio in real-time:

```bash
# Enable audio reactivity
./vibe-sync --audio-reactive-text \
    --react-to-bass \
    --react-to-treble \
    --react-sensitivity 0.8

# Frequency-based movement
./vibe-sync --frequency-based-movement \
    --bass-range 20-250Hz \
    --mid-range 250-4000Hz \
    --treble-range 4000-20000Hz
```

## Text Customization

### Color Settings
```bash
# Custom colors (hex format)
./vibe-sync --song-info-color AAFF00 \
    --url-color AA0099 \
    --song-info-border-color AA00FF \
    --url-border-color 000000 \
    --custom-text-color FFFFFF

# Transparency
./vibe-sync --text-transparency 0.9
```

### Border Settings
```bash
# Border configuration
./vibe-sync --song-info-border-thickness 0.2 \
    --url-border-thickness 0.1 \
    --border-shadow-enabled \
    --border-shadow-offset 2.0 \
    --border-shadow-blur 1.0
```

### Font Size Control
```bash
# Individual font sizes
./vibe-sync --song-info-font-size 42 \
    --url-font-size 28 \
    --custom-text-font-size 36 \
    --metadata-font-size 24

# Scaling
./vibe-sync --responsive-font-scaling \
    --min-font-size 16 \
    --max-font-size 72
```

### Positioning
```bash
# Text positions
./vibe-sync --title-position top-center \
    --artist-position below-title \
    --url-position bottom-right \
    --custom-text-position center

# Offset positioning
./vibe-sync --text-offset-x 50 \
    --text-offset-y -20 \
    --margin 20
```

## Advanced Text Features

### Static Overlays
```bash
# URL/Branding overlay
./vibe-sync --static-overlay-url "https://mysite.com" \
    --static-overlay-text "Follow @mysite" \
    --static-overlay-position top-left \
    --static-overlay-opacity 0.7 \
    --static-overlay-size small

# Multiple static elements
./vibe-sync --static-overlay-count 3 \
    --static-overlay-1-text "Created with VibeSync" \
    --static-overlay-2-text "© 2024 My Brand" \
    --static-overlay-3-text "v2.0.1"
```

### Winter Text Effects
Inspired by seasonal/holiday themes:

```bash
# Snowflake effect
./vibe-sync --winter-effect snowflakes \
    --snowflake-count 50 \
    --snowflake-speed 20.0 \
    --snowflake-size small

# Holiday color scheme
./vibe-sync --winter-color-scheme red-green-gold \
    --winter-text-glow \
    --winter-border-style ornate

# Custom winter text
./vibe-sync --winter-custom-text "Merry Music & Bright Vizuals" \
    --winter-font "Candles" \
    --winter-animation snowfall
```

### Custom Text Elements
```bash
# Dynamic custom text
./vibe-sync --custom-text-enabled \
    --custom-text-source metadata \
    --custom-text-field genre \
    --custom-text-format "Genre: {genre}"

# Real-time custom text
./vibe-sync --realtime-text-enabled \
    --realtime-text-source web-api \
    --realtime-text-interval 30.0 \
    --realtime-text-url "https://api.example.com/quote"

# Event-based text
./vibe-sync --event-text-enabled \
    --event-beat-text "♪ BOOM ♪" \
    --event-drop-text "DROP!" \
    --event-text-scale 2.0
```

## Text Presets

### Built-in Presets
```bash
# Use predefined text presets
./vibe-sync --text-preset minimal
./vibe-sync --text-preset cinematic
./vibe-sync --text-preset electronic
./vibe-sync --text-preset retro
./vibe-sync --text-preset winter
```

### Custom Presets
Create your own text presets:

1. **Configure text settings** as desired
2. **Save preset**:
   ```bash
   ./vibe-sync --save-text-preset "My Custom Preset"
   ```
3. **Load preset**:
   ```bash
   ./vibe-sync --load-text-preset "My Custom Preset"
   ```

### Preset Format
Text presets are stored in JSON format:

```json
{
  "name": "My Custom Preset",
  "version": "1.0",
  "settings": {
    "font_path": "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf",
    "title_font_size": 48,
    "artist_font_size": 36,
    "title_color": "FFFFFF",
    "artist_color": "AAAAAA",
    "animation_type": "fade",
    "transition_time": 3.0,
    "pre_fade_delay": 8.0,
    "min_transparency": 0.15,
    "border_enabled": true,
    "border_thickness": 0.2,
    "border_color": "000000"
  }
}
```

## Performance Optimization

### Text Rendering Performance
```bash
# Optimize for performance
./vibe-sync --text-caching \
    --text-preload-fonts \
    --text-thread-pool-size 4

# Disable effects for performance
./vibe-sync --disable-text-animation \
    --static-text-only
```

### Memory Management
```bash
# Limit text system memory
./vibe-sync --text-max-memory 100MB

# Garbage collection
./vibe-sync --text-gc-interval 30.0
```

### Frame Buffer Optimization
```bash
# Framebuffer settings
./vibe-sync --fb-width 1920 \
    --fb-height 1080 \
    --fb-scale-factor 0.5

# Texture optimization
./vibe-sync --texture-compression \
    --texture-pool-size 10
```

## Framebuffer Integration

### Framebuffer Object (FBO) Usage
The text system uses OpenGL framebuffer objects for optimal performance:

```bash
# Enable FBO optimization
./vibe-sync --enable-fbo \
    --fbo-antialiasing MSAA_4x

# Framebuffer resolution
./vibe-sync --fbo-resolution 1920x1080 \
    --fbo-scale 0.75
```

### Multi-layer Rendering
```bash
# Layer configuration
./vibe-sync --text-layer-count 4 \
    --text-layer-blend-mode normal \
    --text-layer-opacity 0.9

# Z-order management
./vibe-sync --layer-z-order "background,visualizer,text,overlay"
```

## Advanced Configuration

### TOML Configuration File
Create `~/.config/vibe-sync/text-config.toml`:

```toml
# Text Overlay Configuration
[text]
enabled = true
animation_enabled = true
audio_reactive = true

[text.fonts]
title_font = "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf"
artist_font = "/usr/share/fonts/TTF/DejaVuSans.ttf"
url_font = "/usr/share/fonts/TTF/DejaVuSans.ttf"

[text.sizes]
title_size = 48
artist_size = 36
url_size = 28
custom_size = 32

[text.colors]
title_color = "FFFFFF"
artist_color = "AAAAAA"
url_color = "888888"
border_color = "000000"

[text.animations]
type = "fade"
transition_time = 3.0
pre_fade_delay = 8.0
min_transparency = 0.15

[text.winter_effects]
enabled = false
theme = "christmas"
custom_text = "Winter Music Magic"
```

### Dynamic Configuration
```bash
# Load configuration at runtime
./vibe-sync --config-file ~/.config/vibe-sync/text-config.toml

# Override specific settings
./vibe-sync --config-file config.toml \
    --title-font-size 60 \
    --title-color FF0000
```

## Debug and Troubleshooting

### Debug Text System
```bash
# Enable text debug logging
./vibe-sync --debug-text \
    --debug-font-loading \
    --debug-text-rendering

# Text performance monitoring
./vibe-sync --debug-performance-text \
    --text-fps-monitor
```

### Common Issues

#### Text Not Displaying
1. **Check fonts** - Ensure font files exist and are readable
2. **Verify colors** - Ensure color codes are valid hex
3. **Check opacity** - Ensure text opacity > 0
4. **Position verification** - Ensure text not off-screen

#### Poor Performance
1. **Reduce complexity** - Disable animations:
   ```bash
   ./vibe-sync --disable-text-animation
   ```
2. **Lower resolution** - Reduce font sizes
3. **Cache optimization** - Enable text caching
4. **Thread optimization** - Increase thread pool size

#### Animation Glitches
1. **Timing issues** - Adjust animation durations
2. **Memory issues** - Reduce text system memory
3. **Thread conflicts** - Check for thread safety issues

### Performance Metrics
```bash
# Monitor text system performance
./vibe-sync --text-metrics \
    --metrics-interval 1.0

# Output includes:
# - FPS (text rendering)
# - Memory usage
# - CPU usage
# - Animation timing
# - Font loading times
```

## Integration with Other Features

### Music Player Integration
- **Metadata extraction** - Automatically populate text from audio metadata
- **Synchronized updates** - Text updates when song changes
- **Karaoke mode** - Lyrics display integration

### Visualizer Integration
- **Reactive positioning** - Text position responds to visualization
- **Color synchronization** - Text colors match visualization theme
- **Performance coordination** - Text rendering coordinated with visualizer

### Recording Integration
- **Include in recording** - Text overlays included in video output
- **Quality settings** - Text quality settings affect recording quality
- **Real-time recording** - Text animations captured in real-time

### Suno AI Integration
- **Metadata enhancement** - Fill in missing text information
- **Suno-specific text** - Custom text for Suno tracks
- **Search result text** - Display search/playlist information

## Future Enhancements

Based on your development roadmap, planned features include:

1. **Custom GLSL Shaders** - User-defined text effects
2. **3D Text Rendering** - Three-dimensional text elements
3. **Particle Text Effects** - Text with particle systems
4. **VR/AR Support** - Text in virtual/augmented reality
5. **Multi-language Support** - Internationalization
6. **Accessibility Features** - Screen reader support

## Keyboard Shortcuts

| Key | Function |
|-----|----------|
| T | Toggle text overlay display |
| F | Toggle fullscreen text |
| R | Toggle text animation |
| [ | Previous text preset |
| ] | Next text preset |
| - | Decrease font size |
| = | Increase font size |
| 1 | Enable title text |
| 2 | Enable artist text |
| 3 | Enable URL text |
| 4 | Enable custom text |
| W | Toggle winter effects |

For more technical details, see the [Developer Guide](../developer/debug-systems.md) and [API Reference](../developer/api-reference.md).