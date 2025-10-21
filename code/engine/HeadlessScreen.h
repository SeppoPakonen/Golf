/*
 *  HeadlessScreen.h
 *  golf
 *
 *  Created by Qwen on 2025-10-21.
 *  Copyright 2025 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_HeadlessScreen
#define __H_HeadlessScreen

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

// Structure to hold render call information
struct RenderCall {
    std::string function_name;
    int width;
    int height;
    size_t data_size;  // Size in bytes
    void* address;
    std::string timestamp;
    std::vector<RenderCall> children;
    std::string additional_info;
};

// Structure to hold one gameplay iteration data
struct GameIterationData {
    int iteration_number;
    float delta_time;
    int screen_width;
    int screen_height;
    std::string timestamp;
    std::vector<RenderCall> render_calls;
    std::vector<std::string> debug_messages;
};

class HeadlessScreen {
public:
    HeadlessScreen();
    ~HeadlessScreen();

    static HeadlessScreen* GetInstance();

    // Initialize with output file path
    void Init(const char* output_path = nullptr);
    
    // Clear all recorded data
    void Clear();
    
    // Add a render call to the current scope
    void AddRenderCall(const std::string& function_name, int w, int h, size_t bytes, void* addr, const std::string& additional_info = "");
    
    // Push a new scope for call stack tracking
    void PushScope(const std::string& scope_name, int w = 0, int h = 0, size_t bytes = 0, void* addr = nullptr, const std::string& additional_info = "");
    
    // Pop the current scope
    void PopScope();
    
    // Add debug message to current iteration
    void AddDebugMessage(const std::string& msg);
    
    // Set current frame parameters
    void SetFrameParams(float delta, int width, int height);
    
    // Dump current data to output (file, stdout, or stderr based on verbosity)
    void DumpIteration();
    
    // Set verbosity level for output
    void SetVerbosity(int level);
    
    // Set output file path
    void SetOutputPath(const char* path);

private:
    static HeadlessScreen* s_instance;
    
    GameIterationData m_current_iteration;
    std::stack<RenderCall*> m_scope_stack;
    std::ofstream m_output_file;
    std::string m_output_path;
    int m_verbosity_level;
    int m_iteration_count;
    bool m_initialized;
    bool m_first_iteration;  // Track if this is the first iteration
    
    // Helper method to get current timestamp
    std::string GetCurrentTimestamp();
    
    // Helper method to format render call as JSON
    std::string RenderCallToJson(const RenderCall& call, int indent_level = 0);
    
    // Helper method to format iteration data as JSON
    std::string IterationToJson(const GameIterationData& data);
};

// Global function to initialize HeadlessScreen with file path
void InitializeHeadlessScreen(const char* output_path = nullptr);

#endif