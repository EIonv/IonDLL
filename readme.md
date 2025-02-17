# DLL Injection and Memory Dumping Utility

This project demonstrates how to inject a DLL (`iondll.dll`) into a process and call a function (`DumpMemoryInfo`) from the DLL to dump memory information.

---

## Table of Contents
1. [Overview](#overview)
2. [Requirements](#requirements)
3. [How It Works](#how-it-works)
4. [Usage](#usage)
5. [Building the Project](#building-the-project)
6. [License](#license)

---

## Overview

The project consists of two main components:
1. **DLL Injection**: The code locates `iondll.dll` in the same directory as the executable, injects it into the process, and calls the `DumpMemoryInfo` function.
2. **DLL Entry Point**: The `DllMain` function is automatically called by the operating system when the DLL is loaded or unloaded. It invokes the `main` function when the DLL is attached to a process.

---

## Requirements

- **Operating System**: Windows 10 or later (tested on Windows 11).
- **Compiler**: A C++ compiler that supports C++11 or later (e.g., MSVC, MinGW, or Clang).
- **Libraries**:
  - Windows SDK (for Windows API functions).
  - `spdlog` (for logging purposes).
- **Tools**:
  - PowerShell (for downloading the DLL if it is missing).

---

## How It Works

1. **Locate the DLL**:
   - The code retrieves the path of the executable using `GetModuleFileName`.
   - It constructs the path to `iondll.dll` by replacing the executable's filename with `iondll.dll`.

2. **Download the DLL (if missing)**:
   - If `iondll.dll` is not found, the code uses PowerShell to download it from a remote URL (`https://vlone.ddns.net/api/ion/files/download/iondll.dll`).

3. **Load the DLL**:
   - The DLL is loaded into the process using `LoadLibrary`.

4. **Call the Function**:
   - The `DumpMemoryInfo` function is located using `GetProcAddress` and executed.

5. **DLL Entry Point**:
   - The `DllMain` function is the entry point for the DLL. It calls the `main` function when the DLL is attached to a process.

---

## Usage

1. **Build the Project**:
   - Compile the code into a DLL and an executable (if applicable).

2. **Run the Executable**:
   - Place `iondll.dll` in the same directory as the executable.
   - Run the executable. If `iondll.dll` is missing, it will be downloaded automatically.

3. **Check Logs**:
   - The program logs its actions using `spdlog`. Check the logs for details about the injection process.

---

## Building the Project

### Prerequisites
- Install the Windows SDK.
- Install `spdlog` (or replace it with your preferred logging library).

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/dll-injection-project.git
   cd dll-injection-project