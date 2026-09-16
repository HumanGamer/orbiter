// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for direct.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#include <unistd.h>
#include <stdlib.h>

#ifndef _mkdir
#define _mkdir(d) mkdir((d), 0755)
#endif

#ifndef _getcwd
#define _getcwd(b, s) getcwd((b), (s))
#endif

#endif // ORBITER_BUILD_SDLGPUCLIENT
