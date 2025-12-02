# Music Player Guide

The VibeSync music player is built on Qt Multimedia and provides professional-grade audio playback with advanced features for music visualization.

## Core Features

### Audio Engine
- **Qt Multimedia Integration** - Industry-standard audio engine
- **Cross-platform Support** - Works on Linux, Windows, and macOS
- **Low Latency** - Optimized for real-time visualization
- **High-quality Audio** - Support for lossless formats

### Format Support
| Format | Extension | Quality | Metadata | Notes |
|--------|-----------|---------|----------|-------|
| MP3 | .mp3 | Lossy | Full | Universal support |
| FLAC | .flac | Lossless | Full | Best quality |
| WAV | .wav | PCM | Basic | Uncompressed |
| OGG Vorbis | .ogg | Lossy | Full | Open source |
| M4A/AAC | .m4a | Lossy | Full | Apple format |
| Opus | .opus | Lossy | Good | Modern codec |

### Playlist Management
- **Multiple Formats** - M3U, M3U8, PLS, XSPF support
- **Smart Playlists** - Auto-generated based on criteria
- **Crossfade** - Seamless transitions between tracks
- **Gapless Playback** - No silence between tracks
- **Replay Gain** - Volume normalization

## Interface Overview

### Main Window Components
```
┌─────────────────────────────────────────────────────────────┐
│ Menu Bar: File | Playback | Playlist | Settings | Help      │
├─────────────────────────────────────────────────────────────┤
│ [◀][■][▶]  [Vol]██████  [Shuffle] [Repeat] [EQ]  [Suno]    │ <- Transport Controls
├─────────────────────────────────────────────────────────────┤
│ ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐ │
│ │  Now Playing    │ │  Playlist       │ │  Visualizer     │ │
│ │ ┌─────────────┐ │ │ ┌─────────────┐ │ │ ┌─────────────┐ │ │
│ │ │ Album Art   │ │ │ │ Track List  │ │ │ │ ProjectM    │ │ │
│ │ │ Song Info   │ │ │ │ Search      │ │ │ │ Window      │ │ │
│ │ │ Progress    │ │ │ │ Manage      │ │ │ │ Controls    │ │ │
│ │ └─────────────┘ │ │ └─────────────┘ │ │ └─────────────┘ │ │
│ └─────────────────┘ └─────────────────┘ └─────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### Playlist Panel
- **Track List** - Hierarchical view of all tracks
- **Search Box** - Real-time filtering of tracks
- **Context Menu** - Right-click options for tracks
- **Drag & Drop** - Reorder tracks by dragging
- **Metadata Display** - Title, artist, album, duration

### Now Playing Panel
- **Album Art** - Cover image display
- **Song Information** - Title, artist, album, year
- **Progress Bar** - Scrubbable playback position
- **Time Display** - Current/Total time
- **Bitrate Info** - Audio quality information

## File Operations

### Loading Audio Files

#### Single File Selection
```bash
# Command line
./vibe-sync /path/to/song.mp3

# GUI
File → Open Audio File...
```

#### Multiple Files
```bash
# Command line
./vibe-sync /path/to/track1.mp3 /path/to/track2.flac /path/to/track3.wav

# GUI
File → Add Multiple Files...
Select files with Ctrl+click or Shift+click
```

#### Drag & Drop
1. Open file manager to audio files
2. Select one or more audio files
3. Drag and drop into VibeSync playlist panel
4. Files are automatically added to current playlist

### Supported Input Sources
- **Local Files** - Any supported audio format
- **URLs** - HTTP/HTTPS streaming URLs
- **Suno.com Integration** - Automated downloading (requires API)
- **Network Streams** - Internet radio stations
- **Audio CDs** - CD audio extraction (if supported)

### Playlist Files

#### Creating Playlists
1. **Menu**: Playlist → Save Playlist As...
2. **Save As** dialog will appear
3. Choose location and filename
4. **Format**: Select M3U, PLS, or XSPF format
5. **Include Metadata**: Optionally save track information

#### Loading Playlists
1. **Menu**: Playlist → Load Playlist...
2. Navigate to playlist file
3. **Format Auto-detection** - Supported formats auto-load
4. **Merge Option** - Choose to merge or replace current playlist

#### Export Formats
- **M3U** - Most compatible playlist format
- **M3U8** - UTF-8 encoded M3U with Unicode support
- **PLS** - Shoutcast playlist format
- **XSPF** - XML Shareable Playlist Format

## Playback Controls

### Transport Controls
| Button | Keyboard | Function |
|--------|----------|----------|
| ▶ Play | Space | Play/Pause toggle |
| ⏹ Stop | S | Stop playback |
| ⏭ Next | N | Next track |
| ⏮ Previous | P | Previous track |
| ⏩ Fast Forward | → | Seek forward 10s |
| ⏪ Rewind | ← | Seek backward 10s |

### Volume Control
- **Volume Slider** - 0-100% volume control
- **Mute Button** - Toggle audio on/off
- **Keyboard Shortcuts** - Volume up/down keys
- **System Integration** - Follows system volume settings

### Shuffle and Repeat
- **Shuffle Off** - Play tracks in order
- **Shuffle On** - Random track selection
- **Repeat None** - Stop after last track
- **Repeat One** - Loop current track
- **Repeat All** - Loop entire playlist

### Seek Controls
- **Progress Bar** - Click to jump to position
- **Keyboard Seeking** - Left/Right arrows for 10s jumps
- **Fine Seeking** - Shift+Arrow for 1s jumps
- **Mouse Wheel** - Scroll for seeking (if enabled)

## Advanced Features

### Crossfade
Crossfade provides seamless transitions between tracks:

**Configuration:**
```bash
# Command line
./vibe-sync --crossfade-duration 5 --crossfade-type linear

# GUI Settings
Settings → Audio → Crossfade → Duration: 5s, Type: Linear
```

**Crossfade Types:**
- **Linear** - Smooth constant transition
- **Equal Power** - Perceptually smooth volume
- **Custom** - User-defined curve

### Gapless Playback
Eliminates silence between tracks:

**Requirements:**
- Both tracks must have exact sample rate
- No resampling needed
- Gapless-compatible format (FLAC, OGG, etc.)

### Equalizer
Built-in 10-band equalizer:

```
Frequency Bands:
31Hz  | 63Hz  | 125Hz | 250Hz | 500Hz | 1kHz  | 2kHz  | 4kHz  | 8kHz  | 16kHz
[___] | [___] | [___] | [___] | [___] | [___] | [___] | [___] | [___] | [___]
-12dB                                              +12dB
```

**Preset Options:**
- **Flat** - No EQ adjustment
- **Rock** - Enhanced low/high frequencies
- **Pop** - Boosted mids and highs
- **Jazz** - Enhanced upper mids
- **Classical** - Minimal adjustments
- **Vocal** - Boosted vocal frequencies
- **Electronic** - Enhanced bass and highs
- **Custom** - User-defined settings

### Replay Gain
Volume normalization across tracks:

**Modes:**
- **Off** - Use original track volume
- **Track** - Normalize each track individually
- **Album** - Normalize to album level
- **Auto** - Detect and apply optimal mode

## Suno AI Integration

### Setup
1. **Obtain API Key** from Suno.com
2. **Configure Settings**:
   ```bash
   Settings → Suno AI → API Key: your_api_key_here
   ```
3. **Test Connection** - Use "Test Connection" button

### Features
- **Automated Download** - Search and download Suno songs
- **Metadata Enhancement** - Fill in missing track information
- **Playlist Integration** - Add to existing playlists
- **Batch Processing** - Download multiple songs

### Usage
```bash
# Command line download
./vibe-sync --suno-search "electronic ambient" --suno-download

# GUI Process
1. Suno AI → Search...
2. Enter search terms
3. Browse results
4. Select tracks to download
5. Download to local playlist
```

### Supported Suno Features
- **Song Search** - Search by title, artist, genre
- **Album Downloads** - Download complete albums
- **Metadata Import** - Title, artist, duration, genre
- **Quality Selection** - Choose audio quality
- **Playlist Creation** - Auto-create playlists from searches

## Metadata Management

### Supported Metadata Formats
- **ID3v1** - Basic MP3 metadata
- **ID3v2** - Extended MP3 metadata
- **Vorbis Comments** - OGG/FLAC metadata
- **APEv2** - APE metadata
- **MP4** - M4A/AAC metadata

### Displayed Information
- **Title** - Song name
- **Artist** - Performer name(s)
- **Album** - Album name
- **Year** - Release year
- **Track Number** - Position in album
- **Genre** - Music genre
- **Duration** - Track length
- **Bitrate** - Audio quality
- **Sample Rate** - Audio quality
- **Channels** - Mono/Stereo
- **File Size** - Disk space usage

### Metadata Editing
1. **Right-click track** in playlist
2. **Select "Edit Metadata"**
3. **Edit fields** in dialog
4. **Save Changes** - Updates applied immediately
5. **Reload Files** - Re-read metadata from files

## Playlist Operations

### Adding Tracks
- **Individual** - Add single track
- **Multiple** - Select and add multiple tracks
- **Directory** - Add all supported files from folder
- **URL** - Add streaming URLs

### Removing Tracks
- **Selected** - Delete selected tracks
- **All** - Clear entire playlist
- **Duplicates** - Remove duplicate entries
- **Broken Links** - Remove inaccessible files

### Reordering
- **Drag & Drop** - Manual reordering
- **Sort Options** - Sort by various criteria:
  - Title (A-Z, Z-A)
  - Artist (A-Z, Z-A)
  - Album (A-Z, Z-A)
  - Year (Newest/Oldest first)
  - Duration (Shortest/Longest first)
  - File Type
  - File Size

### Searching and Filtering
- **Real-time Search** - Type to filter playlist
- **Advanced Filters**:
  - Genre filter
  - Artist filter
  - Year range
  - Duration range
  - File format
  - Bitrate range

## Performance Optimization

### Memory Usage
- **Streaming** - Load audio data as needed
- **Caching** - Cache recently played tracks
- **Garbage Collection** - Automatic memory cleanup
- **Limit Settings** - Configure maximum memory usage

### CPU Usage
- **Hardware Acceleration** - Use GPU for audio processing
- **Background Processing** - Non-blocking I/O operations
- **Efficient Algorithms** - Optimized audio decoding
- **CPU Affinity** - Pin audio threads to specific cores

### Disk I/O
- **Buffering** - Read-ahead buffering for smooth playback
- **Prefetching** - Preload next track
- **Async I/O** - Non-blocking file operations
- **SSD Optimization** - Optimize for SSD storage

## Troubleshooting

### Audio Issues

#### No Audio Output
```bash
# Check audio system
./vibe-sync --debug-audio --test-audio

# Common solutions:
# 1. Check system volume
# 2. Verify audio device selection
# 3. Restart audio system
pulseaudio --kill && pulseaudio --start

# 4. Try different audio backend
./vibe-sync --audio-backend pulse
```

#### Audio Stuttering
- **Buffer Size** - Increase buffer size:
  ```bash
  ./vibe-sync --audio-buffer 8192
  ```
- **Sample Rate** - Ensure all tracks use same sample rate
- **Disk Speed** - Check storage performance
- **CPU Usage** - Monitor system load

#### Sync Issues
- **Jitter Buffer** - Adjust buffer size:
  ```bash
  ./vibe-sync --jitter-buffer 100
  ```
- **Thread Priority** - Increase audio thread priority:
  ```bash
  ./vibe-sync --audio-realtime
  ```

### Playback Issues

#### Tracks Won't Load
- **File Format** - Verify format support
- **File Permissions** - Check read permissions
- **File Corruption** - Verify file integrity
- **Codec Missing** - Install required codecs

#### Playlist Corruption
- **Backup** - Always backup playlist files
- **Import** - Try different playlist formats
- **Rebuild** - Rebuild playlist from scratch
- **Recovery** - Use playlist recovery tools

#### Memory Issues
```bash
# Monitor memory usage
./vibe-sync --debug-performance

# Reduce memory usage
./vibe-sync --max-memory 512MB
```

### Suno Integration Issues

#### Authentication Failed
- **API Key** - Verify API key is correct
- **Rate Limits** - Check Suno.com usage limits
- **Network** - Verify internet connection
- **Firewall** - Allow VibeSync through firewall

#### Download Fails
- **Permissions** - Check write permissions
- **Disk Space** - Ensure sufficient space
- **Format Support** - Verify audio format
- **Network Issues** - Check connection stability

## Next Steps

Now that you're familiar with the music player:

1. **Explore Visualizations** - Read the [Visualizations Guide](visualizations.md)
2. **Learn Text Overlays** - Check out [Text Overlays Guide](text-overlays.md)
3. **Video Recording** - See the [Video Recording Guide](video-recording.md)
4. **Preset Management** - Study the [Presets Guide](presets.md)
5. **Advanced Features** - Explore debug and developer features

## Keyboard Shortcuts Reference

| Key | Function |
|-----|----------|
| Space | Play/Pause |
| S | Stop |
| N | Next track |
| P | Previous track |
| → | Seek forward 10s |
| ← | Seek backward 10s |
| ↑/↓ | Volume up/down |
| M | Mute toggle |
| F | Fullscreen toggle |
| D | Toggle debug panel |
| R | Start/Stop recording |
| 1-9 | Equalizer band adjustment |
| 0 | Reset equalizer |
| Ctrl+O | Open audio files |
| Ctrl+S | Save playlist |
| Ctrl+L | Load playlist |
| Ctrl+F | Search in playlist |
| Ctrl+A | Select all tracks |
| Delete | Remove selected tracks |

For more advanced usage and debugging, see the [Developer Guide](../developer/debug-systems.md) and [API Reference](../developer/api-reference.md).