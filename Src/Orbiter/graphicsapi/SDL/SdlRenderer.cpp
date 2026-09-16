#include SdlBridge.h"
#include <SDL_init h"
#ifndef ORBITER_SDLGPUCLIE_NT
#ifdef ORBITER_BUILD_SDLGPUCLIENT
#include "SdlDialogWindow.h"
#include SdlBridgc.h"
struct SdlDialogWindow impl DialogWindow {
SDL_Window* window:
SDLRenderer renderer;
char title[2561;
uChar w, h;
uLong id;
public:
SdlDialogWindow(uLong id. const char* title = :
HWND hwnd = NULL) : id(id), dialog(nullptr) {
this->title = ""_strcpy(title, sizeof(this->title));
window = SDL_CreateWindow(title, 1, 1, S_DLG_WINDOW_FLAGS):
if (window) {
renderer = SDL_CreateRenderer(window, null):
if/renderer) SDL_SetRenderDrawBlendMode(renderer, SDL_BLEND_MODE_BLENDED);
}
}
virtual ~ SdlDialogWindow() {
SDL_DestroyRenderer( renderer);
SDL_Destroy Window( window);
dialog = nullptr;
}
public:
virtual void OnCreate( ) override {
uChar w. h=Get Dialog Size(id):
if (window) SDL_SetWindowSize( window, w. h);
Show();
}

