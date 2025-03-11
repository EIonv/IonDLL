#include "console.h"
#include <iostream>
#include <stdexcept>

// Helper function to get temp directory path
std::string GetTempDir() {
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    return std::string(tempPath);
}

bool DebugConsole::InitializeLogging() {
    if (!isEnabled)
        return false;

    try {
        std::vector<spdlog::sink_ptr> sinks;

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::info);
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%^%l%$] %v");
        sinks.push_back(console_sink);

        std::string logFilePath = GetTempDir() + "debug_log_" + std::to_string(std::time(nullptr)) + ".txt";
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath, true);
        file_sink->set_level(spdlog::level::info);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%^%l%$] %v");
        sinks.push_back(file_sink);

        auto logger = std::make_shared<spdlog::logger>("ion", sinks.begin(), sinks.end());
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::info);

        spdlog::info("Logging initialized successfully");
        spdlog::info("Debug log file created at: {}", logFilePath);

        return true;
    } catch (const spdlog::spdlog_ex &ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        return false;
    }
}

bool DebugConsole::Initialize() {
    if (!isEnabled) {
        return true; // Successfully "did nothing"
    }

    if (!AllocConsole()) {
        spdlog::error("Failed to allocate console"); // This will still log if logger is already set up elsewhere
        return false;
    }

    // No need to redirect stdout/stderr; spdlog handles console output
    SetConsoleTitle(fmt::format("IonDLL - PID: {}", GetCurrentProcessId()).c_str());
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