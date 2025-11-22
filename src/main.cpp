#include <iostream>
#include <string>
#include <memory>

// Version info
#define VERSION "1.0.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

void printVersion() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Vibe-Sync AI Music Visualizer v" << VERSION << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Built on: " << BUILD_DATE << " " << BUILD_TIME << std::endl;
    
#ifdef ENABLE_PROJECTM
    std::cout << "ProjectM Support: ENABLED" << std::endl;
#else
    std::cout << "ProjectM Support: DISABLED (visualizer not available)" << std::endl;
#endif

#ifdef ENABLE_TOMLPLUSPLUS
    std::cout << "TOML++ Support: ENABLED" << std::endl;
#else
    std::cout << "TOML++ Support: DISABLED (basic config only)" << std::endl;
#endif

#ifdef QT_CORE_LIB
    std::cout << "Qt6 Support: ENABLED" << std::endl;
#else
    std::cout << "Qt6 Support: DISABLED (console mode only)" << std::endl;
#endif

    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
}

void printHelp() {
    std::cout << "Usage: vibe-sync [OPTIONS]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help         Show this help message" << std::endl;
    std::cout << "  -v, --version      Show version information" << std::endl;
    std::cout << "  --check-deps       Check system dependencies" << std::endl;
    std::cout << "  --build-info       Show detailed build information" << std::endl;
    std::cout << std::endl;
    std::cout << "This is a minimal build to test compilation." << std::endl;
    std::cout << "Full GUI application requires Qt6 and additional dependencies." << std::endl;
}

void checkDependencies() {
    std::cout << "System Dependency Check:" << std::endl;
    std::cout << "========================" << std::endl;
    
    // Check compiler
    std::cout << "Compiler: " << 
#ifdef __GNUC__
        "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__
#elif defined(__clang__)
        "Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__
#else
        "Unknown"
#endif
        << std::endl;
    
    // Check C++ standard
    std::cout << "C++ Standard: " << 
#if __cplusplus >= 202002L
        "C++20"
#elif __cplusplus >= 201703L
        "C++17"
#elif __cplusplus >= 201402L
        "C++14"
#else
        "C++11 or earlier"
#endif
        << std::endl;
    
    // Check platform
    std::cout << "Platform: " << 
#ifdef __linux__
        "Linux"
#elif defined(_WIN32)
        "Windows"
#elif defined(__APPLE__)
        "macOS"
#else
        "Unknown"
#endif
        << std::endl;
    
    // Check features
    std::cout << "Features enabled:" << std::endl;
#ifdef ENABLE_PROJECTM
        std::cout << "  + ProjectM visualizer" << std::endl;
#else
        std::cout << "  - ProjectM visualizer (not available)" << std::endl;
#endif
    
#ifdef ENABLE_TOMLPLUSPLUS
        std::cout << "  + TOML++ configuration parsing" << std::endl;
#else
        std::cout << "  - TOML++ configuration parsing (basic config only)" << std::endl;
#endif
    
#ifdef QT_CORE_LIB
        std::cout << "  + Qt6 GUI framework" << std::endl;
#else
        std::cout << "  - Qt6 GUI framework (console mode)" << std::endl;
#endif
    
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    // Check for help or version flags
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        } else if (arg == "--version" || arg == "-v") {
            printVersion();
            return 0;
        } else if (arg == "--check-deps") {
            checkDependencies();
            return 0;
        } else if (arg == "--build-info") {
            printVersion();
            checkDependencies();
            return 0;
        }
    }
    
    // Default execution
    printVersion();
    
    std::cout << "Vibe-Sync is running in minimal mode." << std::endl;
    std::cout << std::endl;
    
#ifdef QT_CORE_LIB
    std::cout << "This build includes Qt6 support." << std::endl;
    std::cout << "However, full GUI requires additional setup." << std::endl;
#else
    std::cout << "This is a console-only build without Qt6 GUI support." << std::endl;
    std::cout << "To enable the full application:" << std::endl;
    std::cout << "  1. Install Qt6 development packages" << std::endl;
    std::cout << "  2. Install ProjectM libraries" << std::endl;
    std::cout << "  3. Install FFmpeg development libraries" << std::endl;
    std::cout << "  4. Rebuild the project" << std::endl;
#endif
    
    std::cout << std::endl;
    std::cout << "Next steps:" << std::endl;
    std::cout << "  - Run with --help for usage information" << std::endl;
    std::cout << "  - Run with --check-deps to see system status" << std::endl;
    std::cout << "  - Run with --build-info for detailed information" << std::endl;
    
    return 0;
}