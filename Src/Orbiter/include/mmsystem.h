// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for mmsystem.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#include "platform_sdl.h"

// Multimedia timer constants
#define TIME_PERIODIC     1
#define TIME_ONESHOT      0
#define TIME_CALLBACK_FUNCTION 0

typedef void (CALLBACK* TIMECALLBACK)(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

static inline UINT timeSetEvent(UINT uDelay, UINT uResolution, TIMECALLBACK fProc, DWORD_PTR dwUser, UINT fuFlags) { return 1; }
static inline void timeKillEvent(UINT uTimerID) {}
static inline void timeEndEvent(UINT uTimerID) {}

#endif // ORBITER_BUILD_SDLGPUCLIENT
