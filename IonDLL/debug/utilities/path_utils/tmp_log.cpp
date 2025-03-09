#include "tmp_log.h"

// Get a unique log file path in the temp directory
std::string GetTempLogFilePath() {
  // Get the temp directory
  char tempPath[MAX_PATH];
  GetTempPathA(MAX_PATH, tempPath);

  // Create a unique filename with timestamp
  auto now = std::chrono::system_clock::now();
  auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
  std::string filename = std::string(tempPath) + "keylog_" + std::to_string(timestamp) + ".txt";

  return filename;
}