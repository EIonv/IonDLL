#include "console.h"
#include <stdexcept>
#include <iostream>

// Helper function to get temp directory path
std::string GetTempDir() {
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    return std::string(tempPath);
}

// Initialize logging with console and file sinks
bool DebugConsole::InitializeLogging() {
    // If logging is disabled, don't initialize
    if (!isEnabled)
        return false;

    try {
        // Create vector of sinks
        std::vector<spdlog::sink_ptr> sinks;

        // Console sink (with colors for better readability)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);
        // Pattern for console: include timestamp, log level, and message
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%^%l%$] %v");
        sinks.push_back(console_sink);

        // File sink in temp directory
        std::string logFilePath = GetTempDir() + "debug_log_" +
                                 std::to_string(std::time(nullptr)) + ".txt";
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
        file_sink->set_level(spdlog::level::info);
        // Pattern for file: same as console but we'll ensure no extra newlines
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%^%l%$] %v");
        sinks.push_back(file_sink);

        // Create and configure logger
        auto logger = std::make_shared<spdlog::logger>("ion", sinks.begin(), sinks.end());
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::info);

        // Log initialization messages
        spdlog::info("Logging initialized successfully");
        spdlog::info("Debug log file created at: {}", logFilePath);

        return true;
    } catch (const spdlog::spdlog_ex &ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        return false;
    }

    return false;
}

bool DebugConsole::Initialize() {
    if (!isEnabled)
        return true;

    // Allocate console
    if (!AllocConsole()) {
        spdlog::error("Failed to allocate console");
        return false;
    }

    // Redirect stdout
    if (freopen_s(&consoleFile, "CONOUT$", "w", stdout) != 0) {
        spdlog::error("Failed to redirect stdout to console");
        FreeConsole();
        return false;
    }

    // Redirect stderr
    if (freopen_s(&consoleFile, "CONOUT$", "w", stderr) != 0) {
        spdlog::error("Failed to redirect stderr to console");
        FreeConsole();
        return false;
    }

    // Set console title
    SetConsoleTitle(
        fmt::format("IonDLL - PID: {}", GetCurrentProcessId()).c_str());

    // The pattern is already set in InitializeLogging, no need to set it again here
    spdlog::info("Console initialized successfully");
    return true;
}

void DebugConsole::Cleanup() {
    if (!isEnabled)
        return;

    if (consoleFile) {
        fclose(consoleFile);
        consoleFile = nullptr;
    }
    FreeConsole();
}