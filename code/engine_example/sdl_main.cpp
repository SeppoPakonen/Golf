/*
 *  sdl_main.cpp
 *  golf
 *
 *  Bork3D Game Engine with SDL Support
 *  Copyright (c) 2009-2024 Bork 3D LLC. All rights reserved.
 *
 *  This file provides SDL-based cross-platform windowing for the original
 *  NeHe OpenGL example based code.
 */

#ifdef USE_SDL

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Rude.h"
#include "RBGame.h"
#include "RudeDebug.h"
#include "RudeText.h"
#include "RudeFont.h"
#include "RudeTweaker.h"
#include "RudeUnitTest.h"



RBGame *gVBGame = nullptr;

SDL_Window* gWindow = nullptr;
SDL_GLContext gContext = nullptr;

bool keys[SDL_NUM_SCANCODES];     // Array used for the keyboard routine
bool active = true;               // Window active flag set to TRUE by default
bool fullscreen = false;          // Fullscreen flag set to fullscreen mode by default

int windowWidth = 800;
int windowHeight = 600;

const char* kWindowTitle = "Bork3D Game Engine - Golf";

// Function prototypes
bool InitGL();
void ResizeGLScene(int width, int height);
bool SetupGraphics();
void Cleanup();

// Initialize OpenGL
bool InitGL() {
    glShadeModel(GL_SMOOTH);                           // Enable smooth shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);            // Black background
    glClearDepth(1.0f);                               // Depth buffer setup
    glEnable(GL_DEPTH_TEST);                          // Enables depth testing
    glDepthFunc(GL_LEQUAL);                           // The type of depth testing to do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really nice perspective calculations

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    

    
    return true;                                      // Initialization went OK
}

// Resize and initialize the GL window
void ResizeGLScene(int width, int height) {
    if (height == 0)                                  // Prevent a divide by zero error
        height = 1;

    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);                  // Reset the current viewport

    glMatrixMode(GL_PROJECTION);                      // Select the projection matrix
    glLoadIdentity();                                 // Reset the projection matrix

    // Calculate the aspect ratio of the window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                       // Select the modelview matrix
    glLoadIdentity();                                 // Reset the modelview matrix
}

// Setup graphics context
bool SetupGraphics() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Request OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }

    gWindow = SDL_CreateWindow(kWindowTitle, 
                               SDL_WINDOWPOS_CENTERED, 
                               SDL_WINDOWPOS_CENTERED,
                               windowWidth, windowHeight, 
                               window_flags);

    if (gWindow == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create OpenGL context
    gContext = SDL_GL_CreateContext(gWindow);
    if (gContext == nullptr) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // OpenGL context is already initialized by SDL
    printf("OpenGL initialized: %s\n", glGetString(GL_VERSION));

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }

    return true;
}

// Cleanup resources
void Cleanup() {
    if (gContext) {
        SDL_GL_DeleteContext(gContext);
        gContext = nullptr;
    }

    if (gWindow) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    // Setup graphics
    if (!SetupGraphics()) {
        printf("Failed to setup graphics\n");
        return -1;
    }

    // Initialize OpenGL
    if (!InitGL()) {
        printf("Failed to initialize OpenGL\n");
        Cleanup();
        return -1;
    }

    // Initialize game resources
    RudeFontManager::InitFonts();

    if (gVBGame == nullptr)
        gVBGame = new RBGame();

    RudeText::Init();

#ifndef NO_RUDETWEAKER
    RudeTweaker::GetInstance()->Init();
#endif

    // Main loop
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // Handle window resize
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                ResizeGLScene(e.window.data1, e.window.data2);
            }
            // Handle key press
            else if (e.type == SDL_KEYDOWN) {
                keys[e.key.keysym.scancode] = true;
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                }
                else if (e.key.keysym.scancode == SDL_SCANCODE_F1) {
                    // Toggle fullscreen
                    fullscreen = !fullscreen;
                    SDL_SetWindowFullscreen(gWindow, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
                    keys[e.key.keysym.scancode] = false; // Reset key state
                }
            }
            // Handle key release
            else if (e.type == SDL_KEYUP) {
                keys[e.key.keysym.scancode] = false;
            }
            // Handle mouse events (for touch emulation)
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                RudeScreenVertex pos(e.button.x, e.button.y);
                // Create a temporary RudeTouch object for this event
                RudeTouch tempTouch;
                tempTouch.m_location = pos;
                if (gVBGame)
                    gVBGame->TouchDown(&tempTouch);
            }
            else if (e.type == SDL_MOUSEMOTION) {
                RudeScreenVertex pos(e.motion.x, e.motion.y);
                static RudeScreenVertex prevPos(-1, -1);
                if (prevPos.m_x == -1 && prevPos.m_y == -1)
                    prevPos = pos;
                
                // Create a temporary RudeTouch object for this event
                RudeTouch tempTouch;
                tempTouch.m_prev = prevPos;
                tempTouch.m_location = pos;
                if (gVBGame)
                    gVBGame->TouchMove(&tempTouch);
                prevPos = pos;
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                RudeScreenVertex pos(e.button.x, e.button.y);
                static RudeScreenVertex prevPos(-1, -1);
                if (prevPos.m_x == -1 && prevPos.m_y == -1)
                    prevPos = pos;
                
                // Create a temporary RudeTouch object for this event
                RudeTouch tempTouch;
                tempTouch.m_prev = prevPos;
                tempTouch.m_location = pos;
                if (gVBGame)
                    gVBGame->TouchUp(&tempTouch);
                prevPos = pos;
            }
        }

        // Render the scene
        if (active) {
            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();

            if (gVBGame) {
                gVBGame->Render(0.03f, (float)windowWidth, (float)windowHeight);
            }

            // Update screen
            SDL_GL_SwapWindow(gWindow);
        }

        // Small delay to prevent 100% CPU usage
        SDL_Delay(1);
    }

    // Cleanup
    delete gVBGame;
    Cleanup();

    return 0;
}

#endif