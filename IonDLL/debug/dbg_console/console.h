#ifndef CONSOLE_H
#define CONSOLE_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <fmt/format.h>
#include <windows.h>

class DebugConsole {
public:
    DebugConsole(bool enable = true) : isEnabled(enable), consoleFile(nullptr) {}
    ~DebugConsole() { Cleanup(); }

    bool Initialize();
    bool InitializeLogging();
    void Cleanup();

public:
    bool isEnabled;
    FILE* consoleFile;
};

#endif