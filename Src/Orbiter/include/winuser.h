// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for winuser.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#define SWP_NOACTIVATE 0x0010
#define SWP_NOMOVE 0x0002
#define SWP_NOOWNERZORDER 0x0200
#define SWP_NOZORDER 0x0004
#define SWP_NOSIZE 0x0001
#define SWP_NOCOPYBITS 0x0100

#endif // ORBITER_BUILD_SDLGPUCLIENT
