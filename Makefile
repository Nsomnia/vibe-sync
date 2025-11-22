# Simple Makefile for Vibe-Sync
# This is a fallback build system when CMake is not available

# Detect the source directory
ifndef SRCDIR
    SRCDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
endif

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2
INCLUDES = -I$(SRCDIR)/src
SOURCES = $(SRCDIR)/src/main.cpp
TARGET = vibe-sync

# Detect platform
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    PLATFORM_LIBS = -lpthread
endif

all: $(TARGET)

$(TARGET): $(SOURCES)
	@echo "Building Vibe-Sync..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET) $(PLATFORM_LIBS)
	@echo "Build complete: $(TARGET)"

clean:
	@echo "Cleaning build..."
	rm -f $(TARGET)
	@echo "Clean complete"

run: $(TARGET)
	@echo "Running Vibe-Sync..."
	./$(TARGET)

help:
	@echo "Vibe-Sync Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the application"
	@echo "  make clean    - Clean build artifacts"
	@echo "  make run      - Build and run the application"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  ./vibe-sync --version"
	@echo "  ./vibe-sync --help"
	@echo "  ./vibe-sync --check-deps"

.PHONY: all clean run help