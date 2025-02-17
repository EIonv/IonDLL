#ifndef TEST_H
#define TEST_H

#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <spdlog/spdlog.h>
#include "../resources/obfuscate.hpp"

extern "C" __declspec(dllexport) void test();

extern "C" __declspec(dllexport) bool DumpMemoryInfo();

#endif /* TEST_H */
