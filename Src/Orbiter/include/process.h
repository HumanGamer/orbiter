// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for process.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#define _beginthread(start_address, stack_size, arg) 1
#define _endthread()
#define _execvp(command, argv) system(command)
#define _spawnvp(mode, filename, argv) system(filename)

#endif // ORBITER_BUILD_SDLGPUCLIENT
