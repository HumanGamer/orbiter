// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file include/gcCore.h
 * @brief Core graphics client API - public interface for external graphics clients.
 * 
 * This header defines abstract interfaces shared between D3D9Client and SDLGPUClient
 * implementations. Each platform-specific implementation provides device-dependent
 * versions of surface management, mesh rendering, texture loading, and sketchpad
 * operations using either Direct3D 9 or SDL_gpu.
 */

#pragma once

#include "../../Orbitersdk/include/OrbiterAPI.h"
#include "../../Orbitersdk/include/GraphicsAPI.h"

// Forward declare graphics client interface for external plugins
namespace oapi {
    class GraphicsClient;
}

/** SDL_gpu-specific graphics client derived from oapi::GraphicsClient */
class SDLGPU_graphicsClient : public oapi::GraphicsClient {
public:
    virtual ~SDLGPU_graphicsClient() = default;
    
    /** Set the SDL_Window associated with this graphics client */
    void SetWindow(SDL_Window* win) { window_ = win; }
    
    inline SDL_GPUDevice* GetDevice() const { return device_; }

protected:
    SDL_Window* window_ = nullptr;
    SDL_GPUDevice* device_ = nullptr;
};

/** Register an SDLGPU graphics client with Orbiter core */
DLLEXPORT bool RegisterSDLGraphicsClient(SDLGPU_graphicsClient* client, SDL_Window* win);
