// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for SDL3/SDL_gamecontroller.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#include <SDL3/SDL.h>

// SDL_GameController types (already defined in platform_sdl.h)
typedef SDL_Gamepad* SDL_GameController;
typedef void* SDL_GameControllerButtonBind;

#endif // ORBITER_BUILD_SDLGPUCLIENT
