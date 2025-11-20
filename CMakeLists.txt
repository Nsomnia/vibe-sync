cmake_minimum_required(VERSION 3.24)
project(NeonVisualizer VERSION 1.2.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# --- DEBUGGING OPTIONS ---
option(ENABLE_SANITIZERS "Enable Address/Leak Sanitizers" OFF)

if(ENABLE_SANITIZERS)
    message(STATUS "🛡️  Enabling Address Sanitizer (Anti-Doughnut Shield)...")
    add_compile_options(-fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address -fsanitize=leak)
endif()

# Dependencies
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia OpenGLWidgets Core Gui)
find_package(libprojectM REQUIRED)

# Sources
set(SRC_CORE    src/main.cpp 
                src/core/PathUtils.h src/core/StringUtils.h
                src/core/Logger.cpp src/core/Logger.h
                # Debug/Production System
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

add_executable(NeonVisualizer
    ${SRC_CORE}
    ${SRC_DATA}
    ${SRC_ENGINE}
    ${SRC_UI_MENU}
    ${SRC_UI_WIDG}
    ${SRC_UI_MAIN}
)

target_link_libraries(NeonVisualizer PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::OpenGLWidgets
    projectM::projectM
)

target_include_directories(NeonVisualizer PRIVATE src)

# Install target
install(TARGETS NeonVisualizer DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION share/doc/NeonVisualizer)