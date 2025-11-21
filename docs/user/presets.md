# Presets Guide

VibeSync includes an advanced preset management system inspired by your aurora-projectm-visualizer work, providing comprehensive Milkdrop compatibility, favorites systems, and intelligent preset selection.

## Preset Management Overview

### Supported Preset Formats
- **Milkdrop (.milk)** - Original Milkdrop preset format
- **ProjectM (.prjm)** - Enhanced ProjectM native format
- **Custom (.custom)** - User-defined preset format

### Preset Categories
Based on musical genres and visual styles:
- **Electronic** - Techno, House, Trance, Dubstep
- **Ambient** - Chill, Downtempo, Ambient
- **Rock/Metal** - Rock, Metal, Punk, Alternative
- **Pop** - Pop, Dance, Synth-pop
- **Classical** - Orchestral, Chamber, Modern Classical
- **Experimental** - Avant-garde, Noise, Glitch

## Preset Discovery and Loading

### Automatic Discovery
```bash
# Specify preset directory
./vibe-sync --preset-directory /path/to/presets

# Recursive search (search subdirectories)
./vibe-sync --preset-recursive-search

# Filter presets by pattern
./vibe-sync --preset-pattern "electronic*"
./vibe-sync --preset-pattern "*bass*"
./vibe-sync --preset-pattern "ambient-calm*"

# Category-based discovery
./vibe-sync --preset-categories electronic,ambient,experimental
```

### Manual Preset Loading
```bash
# Load specific preset
./vibe-sync --load-preset /path/to/preset.milk

# Load multiple presets
./vibe-sync --load-presets /path/to/preset1.milk,/path/to/preset2.milk

# Load from preset list file
./vibe-sync --preset-list-file /path/to/preset-list.txt
```

### Preset List File Format
Create text files with preset paths:
```
# Electronic presets
/path/to/presets/electronic/neon_lights.milk
/path/to/presets/electronic/techno_beats.milk
/path/to/presets/electronic/future_synth.milk

# Ambient presets
/path/to/presets/ambient/peaceful_waves.milk
/path/to/presets/ambient/ethereal_dreams.milk
/path/to/presets/ambient/cosmic_journey.milk
```

## Preset Cycling and Navigation

### Random Cycling (Default)
```bash
# Enable random preset cycling
./vibe-sync --shuffle-enabled true

# Set cycling interval
./vibe-sync --preset-cycle-interval 30

# Adjust cycling probability
./vibe-sync --cycling-probability 0.8

# Disable cycling for specific presets
./vibe-sync --no-cycle-preset /path/to/slow-preset.milk
```

### Sequential Cycling
```bash
# Switch to sequential mode
./vibe-sync --sequential-mode

# Custom sequence file
./vibe-sync --preset-sequence-file /path/to/sequence.txt

# Bi-directional cycling
./vibe-sync --bidirectional-cycling
```

### Smart Cycling
```bash
# AI-powered preset selection
./vibe-sync --ai-preset-selection \
    --ai-learning-mode \
    --ai-model-path /path/to/model

# Genre-aware cycling
./vibe-sync --genre-aware-cycling \
    --match-genre-presets \
    --smooth-transitions

# Mood-based selection
./vibe-sync --mood-based-cycling \
    --energy-level high \
    --complexity medium
```

## Advanced Preset Features

### Favorites System

#### Manual Favorites Management
```bash
# Mark current preset as favorite
./vibe-sync --favorite-preset-key f

# Add preset to favorites (command line)
./vibe-sync --add-to-favorites /path/to/preset.milk

# Remove from favorites
./vibe-sync --remove-from-favorites /path/to/preset.milk

# View favorites
./vibe-sync --view-favorites
```

#### Favorites-Only Mode
```bash
# Show only favorite presets
./vibe-sync --favorites-only

# Favorites cycling speed
./vibe-sync --favorites-cycle-interval 45

# Separate favorites directory
./vibe-sync --favorites-directory ~/.config/vibe-sync/favorites
```

#### Favorites Configuration File
```json
{
  "version": "1.0",
  "favorites_file": "~/.config/vibe-sync/favorites.json",
  "auto_save_favorites": true,
  "favorites_count": 127,
  "favorites": [
    {
      "path": "/usr/share/vibe-sync/presets/electronic/neon_lights.milk",
      "name": "Neon Lights",
      "artist": "Presets Anonymous",
      "category": "electronic",
      "tags": ["neon", "fast", "energetic"],
      "rating": 5,
      "date_added": "2024-01-15T10:30:00Z",
      "last_played": "2024-01-20T15:45:00Z",
      "play_count": 15,
      "notes": "Great for high-energy electronic music"
    }
  ]
}
```

### Broken Preset Detection

#### Automatic Detection Process
```bash
# Enable automatic broken preset detection
./vibe-sync --broken-preset-detection

# Detection sensitivity
./vibe-sync --broken-preset-sensitivity high

# Move broken presets to quarantine
./vibe-sync --broken-preset-directory /path/to/quarantine

# Mark preset as broken (manual)
./vibe-sync --mark-broken-preset-key b

# Report broken preset with details
./vibe-sync --report-broken-preset /path/to/broken.milk \
    --error-type "shader_compilation" \
    --error-description "GLSL syntax error in vertex shader"
```

#### Detection Algorithm
1. **File Validation** - Check file integrity and format
2. **Shader Compilation** - Attempt to compile GLSL shaders
3. **Runtime Testing** - Execute preset for 5 seconds
4. **Error Monitoring** - Monitor stderr/stdout for errors
5. **Performance Check** - Ensure reasonable FPS
6. **Automatic Quarantine** - Move to broken preset directory

#### Broken Preset Management
```bash
# View broken presets
./vibe-sync --view-broken-presets

# Restore broken preset (after fixing)
./vibe-sync --restore-broken-preset /path/to/fixed.milk

# Batch test broken presets
./vibe-sync --test-broken-presets

# Auto-retry broken presets periodically
./vibe-sync --auto-retry-broken-presets \
    --retry-interval 86400
```

### Preset Blacklist System

#### Adding to Blacklist
```bash
# Add preset to blacklist
./vibe-sync --blacklist-preset /path/to/unwanted.milk

# Add with reason
./vibe-sync --blacklist-preset /path/to/unwanted.milk \
    --reason "too distracting for ambient music" \
    --category "distracting"

# Temporary blacklist (session only)
./vibe-sync --blacklist-preset-temp /path/to/preset.milk

# Batch blacklist
./vibe-sync --blacklist-batch /path/to/blacklist-list.txt
```

#### Blacklist Management
```bash
# View blacklist
./vibe-sync --view-blacklist

# Remove from blacklist
./vibe-sync --unblacklist-preset /path/to/preset.milk

# Export blacklist
./vibe-sync --export-blacklist /path/to/export.json

# Import blacklist
./vibe-sync --import-blacklist /path/to/import.json
```

#### Blacklist File Format
```json
{
  "version": "1.0",
  "auto_blacklist": true,
  "auto_blacklist_on_crash": true,
  "blacklist_count": 23,
  "blacklisted_presets": [
    {
      "path": "/path/to/unwanted.milk",
      "reason": "causes GPU driver crash",
      "category": "broken",
      "date_added": "2024-01-15T10:30:00Z",
      "added_by": "auto-detection",
      "retry_count": 0,
      "last_retry": null
    },
    {
      "path": "/path/to/distracting.milk",
      "reason": "too distracting for ambient music",
      "category": "user_preference",
      "date_added": "2024-01-16T14:20:00Z",
      "added_by": "user",
      "retry_count": 0,
      "last_retry": null
    }
  ]
}
```

## Test Mode and Preset Isolation

### Single Preset Testing
```bash
# Test mode with specific preset
./vibe-sync --test-mode \
    --default-preset /path/to/test.milk

# Disable preset cycling in test mode
./vibe-sync --test-mode --no-preset-cycling

# Extended test duration
./vibe-sync --test-mode --test-duration 300

# Detailed test reporting
./vibe-sync --test-mode --detailed-report
```

### Preset Validation
```bash
# Validate all presets in directory
./vibe-sync --validate-presets /path/to/presets

# Validation report
./vibe-sync --validate-presets /path/to/presets \
    --report-file validation-report.json \
    --include-metadata

# Quick validation (less thorough)
./vibe-sync --quick-validate /path/to/presets

# Validation with performance testing
./vibe-sync --validate-with-performance /path/to/presets
```

### Preset Benchmarking
```bash
# Benchmark preset performance
./vibe-sync --benchmark-preset /path/to/preset.milk

# Benchmark all presets
./vibe-sync --benchmark-all-presets \
    --benchmark-duration 60 \
    --benchmark-metrics fps,cpu,gpu,memory

# Generate performance report
./vibe-sync --generate-performance-report \
    --output-file preset-performance.json
```

## Preset Filtering and Search

### Advanced Filtering
```bash
# Filter by category
./vibe-sync --filter-category electronic,ambient

# Filter by tags
./vibe-sync --filter-tags "neon,fast,energetic"

# Filter by rating
./vibe-sync --filter-rating ">=4"

# Filter by performance
./vibe-sync --filter-performance "fps>=30,gpu<=80%"

# Combined filters
./vibe-sync --filter-category electronic \
    --filter-tags "neon" \
    --filter-rating ">=4" \
    --filter-performance "fps>=30"
```

### Text Search
```bash
# Search in preset names
./vibe-sync --search-presets "neon"

# Search in descriptions
./vibe-sync --search-descriptions "energetic"

# Search in metadata
./vibe-sync --search-metadata "techno"

# Case-insensitive search
./vibe-sync --search-presets "BASS" --case-insensitive

# Regex search
./vibe-sync --search-presets ".*bass.*" --regex
```

### Smart Search
```bash
# AI-powered semantic search
./vibe-sync --semantic-search "upbeat electronic music"

# Similarity search
./vibe-sync --similar-presets /path/to/preset.milk \
    --similarity-threshold 0.8

# Genre-based search
./vibe-sync --genre-search electronic

# Mood-based search
./vibe-sync --mood-search energetic
```

## Preset Customization

### Parameter Adjustment
```bash
# Adjust current preset parameters
./vibe-sync --preset-param-wave-mode 3 \
    --preset-param-rot-speed 0.5 \
    --preset-param-zoom-speed 1.2

# Save parameter adjustments
./vibe-sync --save-preset-modifications /path/to/output.milk

# Load preset with modifications
./vibe-sync --load-preset-modified /path/to/preset.milk \
    --modifications /path/to/mods.json
```

### Preset Mixing
```bash
# Blend two presets
./vibe-sync --blend-presets /path/to/preset1.milk /path/to/preset2.milk \
    --blend-ratio 0.3 \
    --output-preset /path/to/blended.milk

# Create preset variants
./vibe-sync --create-variants /path/to/preset.milk \
    --variant-count 5 \
    --parameter-ranges "wave_mode:1-3,rot_speed:0-2"
```

### Custom Preset Creation
```bash
# Create preset from template
./vibe-sync --create-preset-template \
    --template-type electronic \
    --output-file /path/to/custom.milk

# Generate random preset
./vibe-sync --generate-random-preset \
    --category electronic \
    --complexity medium \
    --output-file /path/to/random.milk
```

## Performance Optimization

### Preset Performance Management
```bash
# Performance-based filtering
./vibe-sync --performance-filter \
    --min-fps 30 \
    --max-gpu-usage 80% \
    --max-memory-usage 512MB

# GPU-intensive preset detection
./vibe-sync --detect-gpu-intensive-presets \
    --gpu-threshold 90%

# Automatic performance adjustment
./vibe-sync --auto-performance-adjust \
    --target-fps 60 \
    --adapt-quality true
```

### Caching and Preloading
```bash
# Preset caching
./vibe-sync --preset-caching \
    --cache-size 100 \
    --cache-strategy lru

# Preload frequently used presets
./vibe-sync --preload-frequent-presets \
    --frequency-threshold 10

# Background preset compilation
./vibe-sync --background-compilation \
    --compilation-threads 4
```

## Integration Features

### Music Player Integration
- **Genre matching** - Auto-select presets matching music genre
- **Metadata sync** - Update preset selection with song changes
- **Playlist presets** - Different presets for different playlist types
- **Mood detection** - AI-powered mood detection for preset selection

### Suno AI Integration
- **AI preset selection** - Machine learning-based preset selection
- **Style matching** - Match presets to Suno music styles
- **Learning system** - Improve selection over time
- **Feedback loop** - User feedback improves AI selection

### Text Overlay Integration
- **Color coordination** - Preset colors influence text colors
- **Theme consistency** - Unified visual themes
- **Performance balance** - Coordinate preset and text rendering

### Recording Integration
- **Preset logging** - Log used presets for video metadata
- **Quality preservation** - Maintain preset quality during recording
- **Preview mode** - Test presets before recording

## Debug and Analytics

### Preset Debug System
```bash
# Enable preset debugging
./vibe-sync --debug-presets \
    --debug-preset-loading \
    --debug-shader-compilation \
    --debug-performance

# Preset performance tracking
./vibe-sync --track-preset-performance \
    --metrics-interval 5.0

# Memory usage tracking
./vibe-sync --track-preset-memory \
    --memory-profiling
```

### Analytics and Statistics
```bash
# Generate preset usage statistics
./vibe-sync --preset-statistics \
    --output-file preset-stats.json

# Most/least used presets
./vibe-sync --popular-presets \
    --time-range 30d

# Performance analytics
./vibe-sync --performance-analytics \
    --include-preset-breakdown
```

### Error Reporting
```bash
# Detailed error logging
./vibe-sync --preset-error-logging \
    --log-level detailed

# Error reporting
./vibe-sync --report-preset-errors \
    --include-system-info

# Crash analysis
./vibe-sync --analyze-crashes \
    --preset-involved
```

## Configuration Files

### Preset Configuration
Create `~/.config/vibe-sync/presets.toml`:

```toml
# Preset Management Configuration
[presets]
enabled = true
directory = "/usr/share/vibe-sync/presets"
recursive_search = true
shuffle_enabled = true
cycle_interval = 30

[presets.favorites]
enabled = true
auto_save = true
favorites_only = false
favorites_file = "~/.config/vibe-sync/favorites.json"

[presets.blacklist]
enabled = true
auto_blacklist = true
auto_blacklist_on_crash = true
blacklist_file = "~/.config/vibe-sync/blacklist.json"

[presets.broken_detection]
enabled = true
sensitivity = "medium"
quarantine_directory = "~/.config/vibe-sync/broken-presets"
auto_retry = false

[presets.performance]
min_fps = 30
max_gpu_usage = 80
auto_filter = true

[presets.caching]
enabled = true
cache_size = 100
preload_frequent = true
```

### Custom Preset List
Create `~/.config/vibe-sync/custom-presets.txt`:

```
# Custom Electronic Presets
/usr/share/vibe-sync/presets/electronic/neon_lights.milk
/usr/share/vibe-sync/presets/electronic/techno_beats.milk
/usr/share/vibe-sync/presets/electronic/future_synth.milk

# Custom Ambient Presets  
/usr/share/vibe-sync/presets/ambient/peaceful_waves.milk
/usr/share/vibe-sync/presets/ambient/ethereal_dreams.milk
/usr/share/vibe-sync/presets/ambient/cosmic_journey.milk

# Seasonal Presets
/usr/share/vibe-sync/presets/seasonal/winter_lights.milk
/usr/share/vibe-sync/presets/seasonal/spring_bloom.milk
```

## Keyboard Shortcuts

| Key | Function |
|-----|----------|
| N | Next random preset |
| P | Previous preset |
| F | Toggle favorite |
| B | Mark as broken |
| R | Reset to default preset |
| 1-9 | Direct preset selection |
| / | Search presets |
| F | Filter presets |
| T | Test mode toggle |
| S | Save current preset |
| L | Load preset |
| [ | Previous category |
| ] | Next category |
| D | Debug preset info |
| H | Preset help |

## Future Enhancements

Planned preset features:

1. **Cloud Presets** - Online preset sharing and discovery
2. **Preset Marketplace** - Community-driven preset sharing
3. **AI Preset Generation** - Machine learning-generated presets
4. **Collaborative Filtering** - Community-based recommendation
5. **Advanced Analytics** - Deep preset usage analytics
6. **Version Control** - Preset versioning and history
7. **Preset Collaboration** - Multi-user preset development
8. **VR Presets** - Virtual reality optimized presets

For more technical details, see the [Developer Guide](../developer/debug-systems.md) and [API Reference](../developer/api-reference.md).