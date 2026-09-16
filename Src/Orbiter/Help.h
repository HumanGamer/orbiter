// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __HELP_H
#define __HELP_H

#define STRICT 1
#ifdef ORBITER_BUILD_SDLGPUCLIENT
#include "platform_sdl.h"
#else
#include <windows.h>
#endif

void OpenHelp (HWND hWnd, const char *file, const char *topic);

void OpenDefaultHelp (HWND hWnd, const char *topic);
// use this only for opening a help window outside the simulation,
// e.g. from the Launchpad dialog. For in-game help, use the mechanism in
// Dialogs.cpp instead.

#endif __HELP_H