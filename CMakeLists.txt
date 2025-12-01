# --- START MERGED CONFIGURATION ---
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt settings (From your branch - keep these on)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Platform detection (From main - keep this, it's robust)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(MACOS TRUE)
endif()

# Build type (From main)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Release optimizations (From main)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Dependencies (From main - safer optional checking)
find_package(Qt6 COMPONENTS Core Widgets QUIET)
if(Qt6_FOUND)
    set(QT6_FOUND TRUE)
    message(STATUS "Qt6 found: ${Qt6_VERSION}")
else()
    set(QT6_FOUND FALSE)
    message(WARNING "Qt6 not found - building without GUI support")
endif()

find_package(ProjectM QUIET)
if(ProjectM_FOUND)
    set(PROJECTM_FOUND TRUE)
    message(STATUS "ProjectM found")
else()
    set(PROJECTM_FOUND FALSE)
    message(WARNING "ProjectM not found - visualizer will be disabled")
endif()

find_package(tomlplusplus QUIET)
if(tomlplusplus_FOUND)
    set(TOMLPLUSPLUS_FOUND TRUE)
    message(STATUS "TOML++ found")
else()
    set(TOMLPLUSPLUS_FOUND FALSE)
    message(WARNING "TOML++ not found - using basic config parsing")
endif()

# Core dependencies (pkg-config)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2 QUIET sdl2)
    pkg_check_modules(ALSA QUIET alsa)
    pkg_check_modules(FFMPEG QUIET libavcodec libavformat libavutil libavfilter libswscale libswresample)
    pkg_check_modules(TAGLIB QUIET taglib)
    pkg_check_modules(FREETYPE QUIET freetype2)
    pkg_check_modules(OPENGL QUIET opengl)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

# --- SOURCE DEFINITIONS (From your branch) ---
set(SRC_CORE    src/core/PathUtils.h src/core/StringUtils.h
                src/core/Logger.cpp src/core/Logger.h
                src/core/DebugManager.cpp src/core/DebugManager.h
                src/core/ErrorManager.cpp src/core/ErrorManager.h
                src/core/PerformanceManager.cpp src/core/PerformanceManager.h
                src/core/PluginManager.cpp src/core/PluginManager.h)
set(SRC_DATA    src/data/SettingsManager.cpp src/data/SettingsManager.h src/core/TextFormatter.h)
set(SRC_ENGINE  src/engine/VizEngine.cpp src/engine/VizEngine.h 
                src/engine/VideoRecorder.cpp src/engine/VideoRecorder.h
                src/engine/AudioEngine.cpp src/engine/AudioEngine.h
                src/engine/PresetManager.cpp src/engine/PresetManager.h
                src/engine/PlaylistManager.cpp src/engine/PlaylistManager.h
                src/engine/TextEngine.cpp src/engine/TextEngine.h)
set(SRC_UI_MENU src/ui/menus/AppMenuBar.cpp src/ui/menus/AppMenuBar.h)
set(SRC_UI_WIDG src/ui/widgets/VisualizerView.cpp src/ui/widgets/VisualizerView.h 
                src/ui/widgets/PlaylistWidget.h src/ui/widgets/DebugConsole.h)
set(SRC_UI_MAIN src/ui/MainWindow.cpp src/ui/MainWindow.h src/ui/dialogs/SettingsDialog.h)

# Combine sources into one list (Merging Main's structure with your files)
set(SOURCES
    src/main.cpp
    ${SRC_CORE}
    ${SRC_DATA}
    ${SRC_ENGINE}
    ${SRC_UI_MENU}
    ${SRC_UI_WIDG}
    ${SRC_UI_MAIN}
)

# Add Qt sources if found
if(QT6_FOUND)
    list(APPEND SOURCES ${QT_SOURCES})
