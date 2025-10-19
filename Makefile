# Golf Game Makefile
# Cross-platform build for the Bork3D Golf Game Engine

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS =
LIBS =

# Directories
ENGINE_DIR = code/engine
EXAMPLE_DIR = code/engine_example
CLASSES_DIR = $(EXAMPLE_DIR)/Classes

# Choose main source based on SDL availability
SDL_AVAILABLE := $(shell pkg-config --exists sdl2 && echo "sdl2" || (pkg-config --exists sdl3 && echo "sdl3"))

# Define SDL build options
ifeq ($(SDL_AVAILABLE), sdl2)
	SDL_CFLAGS = $(shell pkg-config --cflags sdl2)
	SDL_LIBS = $(shell pkg-config --libs sdl2)
	SDL_DEFS = -DUSE_SDL -DSDL2 -DUSE_PVR_TEXT=0 -DRUDE_NO_SKINNED_MESH
	USE_SDL = 1
	MAIN_SOURCE = $(EXAMPLE_DIR)/sdl_main.cpp
else ifeq ($(SDL_AVAILABLE), sdl3)
	SDL_CFLAGS = $(shell pkg-config --cflags sdl3)
	SDL_LIBS = $(shell pkg-config --libs sdl3)
	SDL_DEFS = -DUSE_SDL -DSDL3 -DUSE_PVR_TEXT=0 -DRUDE_NO_SKINNED_MESH
	USE_SDL = 1
	MAIN_SOURCE = $(EXAMPLE_DIR)/sdl_main.cpp
else
	# Use original Windows main
	SDL_DEFS = -DUSE_PVR_TEXT=1
	USE_SDL = 0
	MAIN_SOURCE = $(EXAMPLE_DIR)/win32/main.cpp
endif

# Source files from the engine (excluding platform-specific files for SDL builds)
# We'll conditionally include files based on platform
ifeq ($(USE_SDL), 1)
# For SDL builds, exclude platform-specific implementations
ENGINE_SOURCES = \
	$(ENGINE_DIR)/RudeButtonAnimControl.cpp \
	$(ENGINE_DIR)/RudeButtonControl.cpp \
	$(ENGINE_DIR)/RudeCollision.cpp \
	$(ENGINE_DIR)/RudeColor.cpp \
	$(ENGINE_DIR)/RudeControl.cpp \
	$(ENGINE_DIR)/RudeDebug.cpp \
	$(ENGINE_DIR)/RudeFile.cpp \
	$(ENGINE_DIR)/RudeFont.cpp \
	$(ENGINE_DIR)/RudeGame.cpp \
	$(ENGINE_DIR)/RudeGL.cpp \
	$(ENGINE_DIR)/RudeGLD.cpp \
	$(ENGINE_DIR)/RudeGlobals.cpp \
	$(ENGINE_DIR)/RudeKey.cpp \
	$(ENGINE_DIR)/RudeMath.cpp \
	$(ENGINE_DIR)/RudeMesh.cpp \
	$(ENGINE_DIR)/RudeObject.cpp \
	$(ENGINE_DIR)/RudePerf.cpp \
	$(ENGINE_DIR)/RudePhysics.cpp \
	$(ENGINE_DIR)/RudePhysicsMesh.cpp \
	$(ENGINE_DIR)/RudePhysicsObject.cpp \
	$(ENGINE_DIR)/RudePhysicsSphere.cpp \
	$(ENGINE_DIR)/RudeRect.cpp \
	$(ENGINE_DIR)/RudeRegistry.cpp \
	$(ENGINE_DIR)/RudeRegistryCF.cpp \
	$(ENGINE_DIR)/RudeRegistryText.cpp \
	$(ENGINE_DIR)/RudeSkybox.cpp \
	$(ENGINE_DIR)/RudeSound.cpp \
	$(ENGINE_DIR)/RudeText.cpp \
	$(ENGINE_DIR)/RudeTextControl.cpp \
	$(ENGINE_DIR)/RudeTexture.cpp \
	$(ENGINE_DIR)/RudeTextureManager.cpp \
	$(ENGINE_DIR)/RudeTimeCounter.cpp \
	$(ENGINE_DIR)/RudeTimer.cpp \
	$(ENGINE_DIR)/RudeTouchTracker.cpp \
	$(ENGINE_DIR)/RudeTweaker.cpp \
	$(ENGINE_DIR)/RudeUnicode.cpp \
	$(ENGINE_DIR)/RudeUnitTest.cpp
else
# For Windows builds, include all files including platform-specific implementations
ENGINE_SOURCES = \
	$(ENGINE_DIR)/MikSound.cpp \
	$(ENGINE_DIR)/RudeButtonAnimControl.cpp \
	$(ENGINE_DIR)/RudeButtonControl.cpp \
	$(ENGINE_DIR)/RudeCollision.cpp \
	$(ENGINE_DIR)/RudeColor.cpp \
	$(ENGINE_DIR)/RudeControl.cpp \
	$(ENGINE_DIR)/RudeDebug.cpp \
	$(ENGINE_DIR)/RudeFile.cpp \
	$(ENGINE_DIR)/RudeFont.cpp \
	$(ENGINE_DIR)/RudeGame.cpp \
	$(ENGINE_DIR)/RudeGL.cpp \
	$(ENGINE_DIR)/RudeGLD.cpp \
	$(ENGINE_DIR)/RudeGlobals.cpp \
	$(ENGINE_DIR)/RudeKey.cpp \
	$(ENGINE_DIR)/RudeMath.cpp \
	$(ENGINE_DIR)/RudeMesh.cpp \
	$(ENGINE_DIR)/RudeObject.cpp \
	$(ENGINE_DIR)/RudePerf.cpp \
	$(ENGINE_DIR)/RudePhysics.cpp \
	$(ENGINE_DIR)/RudePhysicsMesh.cpp \
	$(ENGINE_DIR)/RudePhysicsObject.cpp \
	$(ENGINE_DIR)/RudePhysicsSphere.cpp \
	$(ENGINE_DIR)/RudeRect.cpp \
	$(ENGINE_DIR)/RudeRegistry.cpp \
	$(ENGINE_DIR)/RudeRegistryCF.cpp \
	$(ENGINE_DIR)/RudeRegistrySymbian.cpp \
	$(ENGINE_DIR)/RudeRegistryText.cpp \
	$(ENGINE_DIR)/RudeRegistryWin.cpp \
	$(ENGINE_DIR)/RudeSkinnedMesh.cpp \
	$(ENGINE_DIR)/RudeSkybox.cpp \
	$(ENGINE_DIR)/RudeSound.cpp \
	$(ENGINE_DIR)/RudeText.cpp \
	$(ENGINE_DIR)/RudeTextControl.cpp \
	$(ENGINE_DIR)/RudeTexture.cpp \
	$(ENGINE_DIR)/RudeTextureManager.cpp \
	$(ENGINE_DIR)/RudeTimeCounter.cpp \
	$(ENGINE_DIR)/RudeTimer.cpp \
	$(ENGINE_DIR)/RudeTouchTracker.cpp \
	$(ENGINE_DIR)/RudeTweaker.cpp \
	$(ENGINE_DIR)/RudeUnicode.cpp \
	$(ENGINE_DIR)/RudeUnitTest.cpp \
	$(ENGINE_DIR)/SoundEngine.cpp
endif

# Source files from the example game
GAME_SOURCES = \
	$(CLASSES_DIR)/RBCamera.cpp \
	$(CLASSES_DIR)/RBDecorators.cpp \
	$(CLASSES_DIR)/RBGame.cpp \
	$(CLASSES_DIR)/RBKeyMap.cpp \
	$(CLASSES_DIR)/RBKeyTracker.cpp \
	$(CLASSES_DIR)/RBUITitle.cpp

# All sources
SOURCES = $(ENGINE_SOURCES) $(GAME_SOURCES) $(MAIN_SOURCE)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
TARGET = golf

# Platform-specific configuration
UNAME_S := $(shell uname -s)

# Add SDL flags if found
ifeq ($(USE_SDL), 1)
	CXXFLAGS += $(SDL_CFLAGS)
	LIBS += $(SDL_LIBS) -lGL
endif

# Platform-specific settings
ifeq ($(UNAME_S), Linux)
	# Linux
	CXXFLAGS += -D__LINUX__
	ifeq ($(USE_SDL), 1)
		LIBS += -lGLU -lglut -lSDL2_image -lSDL2_mixer
	else
		LDFLAGS += -lopengl32 -lglu32 -lgdi32 -lwinmm -luser32 -lkernel32
	endif
endif

ifeq ($(UNAME_S), Darwin)
	# macOS
	CXXFLAGS += -D__MACOS__
	LDFLAGS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	ifeq ($(USE_SDL), 1)
		LIBS += -lSDL2_image -lSDL2_mixer
	endif
endif

# Include directories
INCLUDES = -I$(ENGINE_DIR) -I$(CLASSES_DIR)

ifeq ($(USE_SDL), 1)
	INCLUDES += -I$(EXAMPLE_DIR)
else
	INCLUDES += -I$(EXAMPLE_DIR)/win32
endif

# Add Windows definitions if not using SDL
ifeq ($(USE_SDL), 0)
	CXXFLAGS += -D_WIN32 -D_WINDOWS
endif

# Don't define USE_POWERVR_SDK at all to use fallback implementation
# Don't define USE_BULLET_PHYSICS to let the header files use fallback implementations
# For now, we'll comment out Bullet Physics dependency to allow build without it
CXXFLAGS += $(INCLUDES) $(SDL_DEFS)

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)
	@echo "Build completed: $(TARGET)"

# Pattern rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean completed"

# Install the game
install:
	@echo "Installing $(TARGET) to system..."
	cp $(TARGET) /usr/local/bin/
	@echo "Installation completed"

# Uninstall the game
uninstall:
	@echo "Removing $(TARGET) from system..."
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstallation completed"

# Help target
help:
	@echo "Bork3D Golf Game Engine Makefile"
	@echo "Available targets:"
	@echo "  all       - Build the golf game (default)"
	@echo "  clean     - Clean build artifacts"
	@echo "  install   - Install the game to system"
	@echo "  uninstall - Uninstall the game from system"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "This build system will attempt to detect and use SDL2/SDL3 if available."
	@echo "For best results, install SDL2 development libraries:"
	@echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev"
	@echo "  macOS: brew install sdl2 sdl2_image sdl2_mixer"