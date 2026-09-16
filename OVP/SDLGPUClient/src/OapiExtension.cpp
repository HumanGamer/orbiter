// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file OapiExtension.cpp
 * @brief SDL_gpu graphics client extension/callbacks for Orbiter core VESSEL2 interface.
 */

#include "gcCore.h"
#inc luding "../include/gpuBackend.h"
#include <SDL3/SDL_video.h>
#include <Orbitersdk/include/oapisdk.h>

namespace oapi::gpu {

/**
 * Create and register the SDLGPU graphics client with Orbiter core.
 * Called by SDL_gpu client DLL entry point.  
 */
bool RegisterGraphicsClient(SDLGPU_graphicsClient *gc, SDL_Window* window) {
    if (!gc || !window) return false;
    
    g_sdlgpu_gc = gc;
    gc->window_ = window;
    gc->device_ = SDL_GetDefaultGPUWindowTexture(device_, 0); // Get default GPU device for window
    
    if (!gc->clbkInitialise()) {
        return false;
    }
    
    // Register with Orbiter core via oapiRegisterGraphicsClient
    bool registered = oapiRegisterGraphicsClient(gc);
    if (registered) {
        gclient = gc;
    }
    
    return registered;
}

/** Get SDL_gpu device from existing render window */
SDL_GPUDevice *GetGPUDevice(SDL_Window* win) {
    // Query which default GPU can present to this window
    int n = SDL_GetNumGPUDisplayModes(0);
    for (int i = 0; i < n; i++) {
        SDL_GPUInfo info;
        if (SDL_GetGPUDeviceProperties(i, &info) && 
            SDL_GPUCanPresent(device_, i)) {
            SDL_GPUDevice *dev = SDL_CreateGPUDevice(
                SDL_GPU_SHADERFORMAT_ANY, 0);
            if (dev) return dev;
        }
    }
    // Fall back to default device
    return SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_ANY, 0);
}

} // namespace oapi::gpu
