cmake_minimum_required(VERSION 3.18)

project(vibe-sync VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(MACOS TRUE)
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Release optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Enable warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Try to find Qt6 (optional)
find_package(Qt6 COMPONENTS Core Widgets QUIET)
if(Qt6_FOUND)
    set(QT6_FOUND TRUE)
    message(STATUS "Qt6 found: ${Qt6_VERSION}")
else()
    set(QT6_FOUND FALSE)
    message(WARNING "Qt6 not found - building without GUI support")
endif()

# Try to find ProjectM (optional)
find_package(ProjectM QUIET)
if(ProjectM_FOUND)
    set(PROJECTM_FOUND TRUE)
    message(STATUS "ProjectM found")
else()
    set(PROJECTM_FOUND FALSE)
    message(WARNING "ProjectM not found - visualizer will be disabled")
endif()

# Try to find TOML++ (optional)
find_package(tomlplusplus QUIET)
if(tomlplusplus_FOUND)
    set(TOMLPLUSPLUS_FOUND TRUE)
    message(STATUS "TOML++ found")
else()
    set(TOMLPLUSPLUS_FOUND FALSE)
    message(WARNING "TOML++ not found - using basic config parsing")
endif()

# Core dependencies
find_package(PkgConfig QUIET)

# Try to find audio/media dependencies (optional)
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2 QUIET sdl2)
    pkg_check_modules(ALSA QUIET alsa)
    pkg_check_modules(FFMPEG QUIET libavcodec libavformat libavutil libavfilter libswscale libswresample)
    pkg_check_modules(TAGLIB QUIET taglib)
    pkg_check_modules(FREETYPE QUIET freetype2)
    pkg_check_modules(OPENGL QUIET opengl)
endif()

# Set include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
)

# Source files - start with minimal set
set(SOURCES
    src/main.cpp
)

# Add more source files only if dependencies are found
if(QT6_FOUND)
    # Qt-specific files would go here
    set(QT_SOURCES
        # Add Qt-specific source files when available
    )
    list(APPEND SOURCES ${QT_SOURCES})
endif()

# Generate the executable
add_executable(vibe-sync ${SOURCES})

# Target properties
set_target_properties(vibe-sync PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Platform-specific libraries
if(LINUX)
    target_link_libraries(vibe-sync pthread)
endif()

# Link optional libraries based on what's available
if(QT6_FOUND)
    target_link_libraries(vibe-sync Qt6::Core Qt6::Widgets)
else()
    message(STATUS "Qt6 not found - GUI functionality disabled")
endif()

if(PROJECTM_FOUND)
    target_link_libraries(vibe-sync projectM)
else()
    message(STATUS "ProjectM not found - visualization disabled")
endif()

if(TOMLPLUSPLUS_FOUND)
    target_link_libraries(vibe-sync tomlplusplus)
else()
    message(STATUS "TOML++ not found - basic config only")
endif()

# Link optional pkg-config libraries
if(SDL2_FOUND)
    target_link_libraries(vibe-sync ${SDL2_LIBRARIES})
    target_include_directories(vibe-sync PRIVATE ${SDL2_INCLUDE_DIRS})
endif()

if(ALSA_FOUND)
    target_link_libraries(vibe-sync ${ALSA_LIBRARIES})
    target_include_directories(vibe-sync PRIVATE ${ALSA_INCLUDE_DIRS})
endif()

if(FFMPEG_FOUND)
    target_link_libraries(vibe-sync ${FFMPEG_LIBRARIES})
    target_include_directories(vibe-sync PRIVATE ${FFMPEG_INCLUDE_DIRS})
endif()

if(TAGLIB_FOUND)
    target_link_libraries(vibe-sync ${TAGLIB_LIBRARIES})
    target_include_directories(vibe-sync PRIVATE ${TAGLIB_INCLUDE_DIRS})
endif()

if(FREETYPE_FOUND)
    target_link_libraries(vibe-sync ${FREETYPE_LIBRARIES})
    target_include_directories(vibe-sync PRIVATE ${FREETYPE_INCLUDE_DIRS})
endif()

if(OPENGL_FOUND)
    target_link_libraries(vibe-sync ${OPENGL_LIBRARIES})
    target_include_directories(vibe-sync PRIVATE ${OPENGL_INCLUDE_DIRS})
endif()

# Compiler-specific settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC-specific options
    target_compile_options(vibe-sync PRIVATE -fPIC)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang-specific options
    target_compile_options(vibe-sync PRIVATE -fPIC)
endif()

# Sanitizer support (optional)
option(USE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
if(USE_SANITIZERS)
    target_compile_options(vibe-sync PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(vibe-sync PRIVATE -fsanitize=address -fsanitize=undefined)
endif()

# Install targets
install(TARGETS vibe-sync
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "============================================================")
message(STATUS "  Vibe-Sync Build Configuration")
message(STATUS "============================================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt6 Support: ${QT6_FOUND}")
message(STATUS "ProjectM Support: ${PROJECTM_FOUND}")
message(STATUS "TOML++ Support: ${TOMLPLUSPLUS_FOUND}")
message(STATUS "SDL2 Support: ${SDL2_FOUND}")
message(STATUS "FFmpeg Support: ${FFMPEG_FOUND}")
message(STATUS "============================================================")
message(STATUS "Build complete! Run './vibe-sync --version' to test.")
message(STATUS "============================================================")