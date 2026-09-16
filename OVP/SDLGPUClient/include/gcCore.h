// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file include/gcCore.h
 * @brief Core graphics client API - public interface for external graphics clients.
 * 
 * This header defines the abstract oapi::GraphicsClient class interface, shared
 * between D3D9Client and SDLGPUClient implementations. Each platform-specific
 * implementation provides device-dependent versions of surface management, 
 * mesh rendering, texture loading, and sketchpad operations using either
 * Direct3D 9 or SDL_gpu.
 */

#pragma once

#include "Orbitersdk/include/oapisdk.h"
#include <Orbitersdk/include/GraphicsAPI.h>

// Forward declare graphics client interface for external plugins
namespace oapi {
    class GraphicsClient;
}

/** Register an SDLGPU graphics client with Orbiter core */
DLLEXPORT bool RegisterSDLGraphicsClient(void* client, void* window);
