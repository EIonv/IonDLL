#ifndef TEST_H
#define TEST_H

#include "../../resources/obfuscate.hpp"
#include <cstdlib>
#include <iostream>
#include <spdlog/spdlog.h>
#include <windows.h>
#include <string>

extern "C" __declspec(dllexport) bool DumpMemoryInfo();

#endif /* TEST_H */
