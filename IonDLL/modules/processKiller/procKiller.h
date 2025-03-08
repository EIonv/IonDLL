#ifndef PROCKILLER_H
#define PROCKILLER_H

#include <windows.h>
#include <tlhelp32.h>
#include <string>

DWORD WINAPI ProcessKillerThread(LPVOID lpParam);

#endif /* PROCKILLER_H */
