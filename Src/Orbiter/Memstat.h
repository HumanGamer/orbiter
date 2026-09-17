// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __MEMSTAT_H
#define __MEMSTAT_H

#ifdef ORBITER_BUILD_SDLGPUCLIENT
#include "platform_sdl.h"
#else
#include <windows.h>
#include <psapi.h>
#endif

#ifndef ORBITER_BUILD_SDLGPUCLIENT
typedef BOOL (CALLBACK *Proc_GetProcessMemoryInfo)(HANDLE,PPROCESS_MEMORY_COUNTERS,DWORD);
#else
typedef BOOL (CALLBACK *Proc_GetProcessMemoryInfo)(HANDLE,PPROCESS_MEMORY_COUNTERS,DWORD);
#endif

class MemStat {
public:
    MemStat ();
    ~MemStat ();

    long HeapUsage ();

private:
    static HMODULE hLib;
	static bool bLib;
    HANDLE hProc;
	Proc_GetProcessMemoryInfo pGetProcessMemoryInfo;
    bool active;
};

#endif // !__MEMSTAT_H