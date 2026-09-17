#pragma once
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_joystick.h>
typedef SDL_Gamepad* SDL_GameController;
#define SDL_INIT_GAMEPAD SDL_INIT_GAMEPAD
static inline SDL_GameController SDL_OpenGamepad(int device_index) { return SDL_OpenGamepad((SDL_JoystickID)device_index); }
static inline void SDL_CloseGameController(SDL_GameController c) { if(c) SDL_CloseGamepad(c); }
static inline int SDL_GameControllerGetAxis(SDL_GameController c, int axis) { return c ? SDL_GetGamepadAxis(c, (SDL_GamepadAxis)axis) : 0; }
static inline int SDL_GameControllerGetButton(SDL_GameController c, int button) { return c ? SDL_GetGamepadButton(c, (SDL_GamepadButton)button) : 0; }
static inline void SDL_GameControllerUpdate() { SDL_UpdateGamepads(); }
