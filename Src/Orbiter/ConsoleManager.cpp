#include "ConsoleManager.h"

#ifdef ORBITER_BUILD_SDLGPUCLIENT
#include "platform_sdl.h"
#else
#include <windows.h>
#endif

bool ConsoleManager::IsConsoleExclusive(void) {
    DWORD pids[2];
    DWORD num_pids = GetConsoleProcessList(pids, 2);
    return num_pids <= 1;
}

void ConsoleManager::ShowConsole(bool show)
{
    HWND wnd = GetConsoleWindow();
    if (wnd)
        ShowWindow(wnd, show ? SW_SHOW : SW_HIDE);
}
