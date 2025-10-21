/*
 *  HeadlessScreen.cpp
 *  golf
 *
 *  Created by Qwen on 2025-10-21.
 *  Copyright 2025 Bork 3D LLC. All rights reserved.
 *
 */

#include "HeadlessScreen.h"
#include "RudeDebug.h"
#include <iomanip>

HeadlessScreen* HeadlessScreen::s_instance = nullptr;

HeadlessScreen::HeadlessScreen() 
    : m_verbosity_level(0)
    , m_iteration_count(0)
    , m_initialized(false)
    , m_first_iteration(true)
{
    Clear();
}

HeadlessScreen::~HeadlessScreen()
{
    if (m_output_file.is_open()) {
        // Close the JSON array
        if (!m_first_iteration) {  // Only add closing bracket if we've written data
            m_output_file << "\n]";
        } else {
            m_output_file << "[]";  // Empty array if no data was written
        }
        m_output_file.close();
    }
}

HeadlessScreen* HeadlessScreen::GetInstance()
{
    if (!s_instance)
        s_instance = new HeadlessScreen();
    return s_instance;
}

void HeadlessScreen::Init(const char* output_path)
{
    if (output_path) {
        SetOutputPath(output_path);
        // Write opening bracket for JSON array
        if (m_output_file.is_open()) {
            m_output_file << "[\n";
            m_output_file.flush();
        }
    }
    m_initialized = true;
    m_first_iteration = true;
    
    // Set verbosity from global verbosity level if not already set
    if (m_verbosity_level == 0) {
        extern int gVerbosityLevel;
        m_verbosity_level = gVerbosityLevel;
    }
}

void HeadlessScreen::Clear()
{
    m_current_iteration = GameIterationData();
    m_current_iteration.iteration_number = ++m_iteration_count;
    m_current_iteration.timestamp = GetCurrentTimestamp();
    
    // Clear the scope stack
    while (!m_scope_stack.empty())
        m_scope_stack.pop();
    
    // Initialize the top-level render scope
    RenderCall root_call;
    root_call.function_name = "render";
    root_call.timestamp = GetCurrentTimestamp();
    m_current_iteration.render_calls.push_back(root_call);
    m_scope_stack.push(&m_current_iteration.render_calls.back());
}

void HeadlessScreen::AddRenderCall(const std::string& function_name, int w, int h, size_t bytes, void* addr, const std::string& additional_info)
{
    RenderCall call;
    call.function_name = function_name;
    call.width = w;
    call.height = h;
    call.data_size = bytes;
    call.address = addr;
    call.timestamp = GetCurrentTimestamp();
    call.additional_info = additional_info;
    
    if (!m_scope_stack.empty()) {
        m_scope_stack.top()->children.push_back(call);
    } else {
        // If no scope is active, add to root render calls
        m_current_iteration.render_calls.push_back(call);
    }
}

void HeadlessScreen::PushScope(const std::string& scope_name, int w, int h, size_t bytes, void* addr, const std::string& additional_info)
{
    RenderCall scope_call;
    scope_call.function_name = scope_name;
    scope_call.width = w;
    scope_call.height = h;
    scope_call.data_size = bytes;
    scope_call.address = addr;
    scope_call.timestamp = GetCurrentTimestamp();
    scope_call.additional_info = additional_info;
    
    if (!m_scope_stack.empty()) {
        // Add to children of current scope
        m_scope_stack.top()->children.push_back(scope_call);
        m_scope_stack.push(&m_scope_stack.top()->children.back());
    } else {
        // If no scope, add as root render call
        m_current_iteration.render_calls.push_back(scope_call);
        m_scope_stack.push(&m_current_iteration.render_calls.back());
    }
}

void HeadlessScreen::PopScope()
{
    if (!m_scope_stack.empty()) {
        m_scope_stack.pop();
    }
}

void HeadlessScreen::AddDebugMessage(const std::string& msg)
{
    m_current_iteration.debug_messages.push_back(msg);
}

void HeadlessScreen::SetFrameParams(float delta, int width, int height)
{
    m_current_iteration.delta_time = delta;
    m_current_iteration.screen_width = width;
    m_current_iteration.screen_height = height;
}

void HeadlessScreen::DumpIteration()
{
    if (!m_initialized) return;
    
    std::string json_output = IterationToJson(m_current_iteration);
    
    if (m_verbosity_level >= 3) {
        // Output to stdout when -v3 is enabled
        std::cout << json_output << std::endl;
        
        // Also output to file if output path is specified
        if (!m_output_path.empty() && m_output_file.is_open()) {
            if (!m_first_iteration) {
                m_output_file << ",\n";  // Add comma separator for array elements (except first)
            }
            m_output_file << json_output << std::endl;
            m_output_file.flush();
            m_first_iteration = false;
        }
    } else if (!m_output_path.empty() && m_output_file.is_open()) {
        // Write to file if output path is specified and -v3 is not enabled
        if (!m_first_iteration) {
            m_output_file << ",\n";  // Add comma separator for array elements (except first)
        }
        m_output_file << json_output << std::endl;
        m_output_file.flush();
        m_first_iteration = false;
    }
}

void HeadlessScreen::SetVerbosity(int level)
{
    m_verbosity_level = level;
}

void HeadlessScreen::SetOutputPath(const char* path)
{
    if (path) {
        m_output_path = std::string(path);
        if (m_output_file.is_open()) {
            m_output_file.close();
        }
        m_output_file.open(m_output_path, std::ios::out | std::ios::app); // Append mode
    }
}

std::string HeadlessScreen::GetCurrentTimestamp()
{
    time_t now = time(0);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buffer);
}

std::string HeadlessScreen::RenderCallToJson(const RenderCall& call, int indent_level)
{
    std::ostringstream oss;
    std::string indent(indent_level * 2, ' ');
    
    oss << "{\n";
    oss << indent << "  \"function_name\": \"" << call.function_name << "\",\n";
    oss << indent << "  \"width\": " << call.width << ",\n";
    oss << indent << "  \"height\": " << call.height << ",\n";
    oss << indent << "  \"data_size\": " << call.data_size << ",\n";
    oss << indent << "  \"address\": \"" << call.address << "\",\n";
    oss << indent << "  \"timestamp\": \"" << call.timestamp << "\",\n";
    
    if (!call.additional_info.empty()) {
        oss << indent << "  \"additional_info\": \"" << call.additional_info << "\",\n";
    }
    
    if (!call.children.empty()) {
        oss << indent << "  \"children\": [\n";
        for (size_t i = 0; i < call.children.size(); ++i) {
            oss << RenderCallToJson(call.children[i], indent_level + 2);
            if (i < call.children.size() - 1) {
                oss << ",";
            }
            oss << "\n";
        }
        oss << "\n" << indent << "  ]\n";
    } else {
        oss << indent << "  \"children\": []\n";
    }
    
    oss << indent << "}";
    return oss.str();
}

std::string HeadlessScreen::IterationToJson(const GameIterationData& data)
{
    std::ostringstream oss;
    
    oss << "{\n";
    oss << "  \"iteration\": " << data.iteration_number << ",\n";
    oss << "  \"delta_time\": " << data.delta_time << ",\n";
    oss << "  \"timestamp\": \"" << data.timestamp << "\",\n";
    oss << "  \"screen_width\": " << data.screen_width << ",\n";
    oss << "  \"screen_height\": " << data.screen_height << ",\n";
    oss << "  \"render\": [\n";
    
    for (size_t i = 0; i < data.render_calls.size(); ++i) {
        oss << RenderCallToJson(data.render_calls[i], 1);
        if (i < data.render_calls.size() - 1) {
            oss << ",";
        }
        oss << "\n";
    }
    
    oss << "\n  ],\n";
    oss << "  \"debug_messages\": [\n";
    for (size_t i = 0; i < data.debug_messages.size(); ++i) {
        oss << "    \"" << data.debug_messages[i] << "\"";
        if (i < data.debug_messages.size() - 1) {
            oss << ",";
        }
        oss << "\n";
    }
    oss << "  ]\n";
    oss << "}";
    
    return oss.str();
}

// Global function implementation
void InitializeHeadlessScreen(const char* output_path)
{
    HeadlessScreen::GetInstance()->Init(output_path);
}