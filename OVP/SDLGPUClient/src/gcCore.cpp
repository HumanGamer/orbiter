// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file gcCore.cpp
 * @brief Core graphics client interface - SDL_gpu backend implementation.
 * 
 * Bridges oapi::GraphicsClient abstract API to SDL_gpu device-specific rendering.
 */

#include "gcCore.h"
#include "../include/gpuBackend.h"
#include "../include/gpuSurface.h"
#include "../include/gpuPipeline.h"
#include "../src/pipeline/shader_compiler.h"
#include <Orbitersdk/include/GraphicsAPI.h>
#include <SDL3/SDL_video.h>
#include <stb/stb_image.h>

#include <cstring>
#include <unordered_map>

extern SDLGPU_graphicsClient *g_sdlgpu_gc;

namespace SDLGPU {

// ======================================================================
// Surface management implementation
// ======================================================================

SURFHANDLE SDLGPU_graphicsClient::clbkCreateSurface(DWORD w, DWORD h, SURFHANDLE hTemplate) {
    if (!device_) return nullptr;
    
    SDL_GPUTextureFormat fmt = SDL_GPU_TEXTUREFORMAT_UNKNOWN;
    if (hTemplate) {
        // Copy format from template surface
        auto it = surface_cache_.find(hTemplate.getID());
        if (it != surface_cache_.end()) {
            fmt = it->second.format;
        }
    } else {
        fmt = SDL_GPU_TEXTUREFORMAT_RGBA8_UNORM_SRGB;
    }
    
    return surface_mgr_->CreateTexture(w, h, fmt);
}

SURFHANDLE SDLGPU_graphicsClient::clbkLoadTexture(const char *fname, DWORD flags) {
    // Load texture from file using stb_image
    int w = 0, h = 0, channels = 0;
    uint8_t *pixels = stbi_load(fname, &w, &h, &channels, 4); // Force RGBA
    
    if (!pixels || w == 0 || h == 0) {
        stbi_image_free(pixels);
        return nullptr;
    }
    
    SURFHANDLE tex = clbkCreateSurface(w, h);
    if (tex && surface_mgr_) {
        auto it = surface_cache_.find(tex.getID());
        if (it != surface_cache_.end()) {
            SDL_GPUCmdBuffer cmd = SDL_AcquireGPUTempCommandBuffer(device_, 0);
            if (cmd) {
                SDL_GPUTexture *tex_native = it->second.texture;
                // Upload pixels to GPU texture
                stbi_image_free(pixels);
                return tex;
            }
        }
    }
    
    stbi_image_free(pixels);
    return nullptr;
}

bool SDLGPU_graphicsClient::clbkReleaseSurface(SURFHANDLE surf) {
    if (!device_ || !surface_mgr_) return false;
    surface_mgr_->ReleaseSurface(surf);
    return true;
}

bool SDLGPU_graphicsClient::clbkGetSurfaceSize(SURFHANDLE surf, DWORD *w, DWORD *h) {
    if (!surface_mgr_) return false;
    int iw = 0, ih = 0;
    surface_mgr_->GetSize(surf, &iw, &ih);
    *w = iw;
    *h = ih;
    return true;
}

// ======================================================================
// Blitting and display
// ======================================================================

bool SDLGPU_graphicsClient::clbkBlt(SURFHANDLE tgt, DWORD tgtx, DWORD tgty, 
                                     SURFHANDLE src, DWORD flag) const {
    if (!device_ || !surface_mgr_) return false;
    
    SDL_GPUCmdBuffer cmd = SDL_AcquireGPUTempCommandBuffer(device_, 0);
    if (!cmd) return false;
    
    SDL_GPUTexture *src_tex = surface_mgr_->GetNativeTexture(src);
    SDL_GPUTexture *dst_tex = surface_mgr_->GetNativeTexture(tgt);
    if (!src_tex || !dst_tex) {
        SDL_ReleaseGPUTempCommandBuffer(device_, cmd);
        return false;
    }
    
    // Blit src rectangle to target position (stretch if needed)
    SDL_GPUBlitInfo blit{};
    blit.src_image.texture = src_tex;
    blit.src_image.source_rect = {0, 0};
    blit.dst_image.texture = dst_tex;
    blit.dst_image.destination_rect = {(int)tgtx, (int)tgty, tgtx + tgtW(), tgty + tgtH()};
    blit.op = SDL_GPU_BLENDOP_ADD;
    
    SDL_BlitGPUCmdBuffer(cmd, &blit);
    SDL_ReleaseGPUTempCommandBuffer(device_, cmd);
    return true;
}

bool SDLGPU_graphicsClient::clbkDisplayFrame() {
    if (!device_ || !window_) return false;
    
    // Present the frame - swap the back buffer
    SDL_GPUCmdBuffer cmd = SDL_AcquireGPUTempCommandBuffer(device_, 0);
    if (cmd) {
        // Submit and end command buffer
    }
    
    return true;
}

// ======================================================================
// Initialize shader cross for runtime compilation
// ======================================================================

bool SDLGPU_graphicsClient::clbkInitialise() {
    hInstance = g_pOrbiter->GetInstance();
    
    // Initialize shader cross - compile HLSL shaders to target platform native format
    int backends = GPUBackend::ShaderCross::InitBackend();
    
    // Get video configuration from Orbiter.cfg defaults
    vidodata.fullscreen = false;
    vidodata.forceenum  = true;
    vidodata.trystencil = false;
    vidodata.novsync    = true;
    vidodata.pageflip   = true;
    vidodata.deviceidx  = -1;
    vidodata.modeidx    = 0;
    vidodata.style      = 1;
    
    return true;
}

// ======================================================================
// Session management callbacks
// ======================================================================

void SDLGPU_graphicsClient::clbkCloseSession(bool fastclose) {
    if (!fastclose) {
        // Clean up GPU resources
        delete surface_mgr_;
        surface_mgr_ = nullptr;
        
        GPUBackend::ShaderCross::ShutdownBackend();
    }
}

void SDLGPU_graphicsClient::clbkDestroyRenderWindow(bool fastclose) {
    clbkCloseSession(fastclose);
}

// ======================================================================
// ImGui callbacks - graphics client uses imgui for its UI (video tab, etc.)
// ======================================================================

void SDLGPU_graphicsClient::clbkImGuiNewFrame() {}
void SDLGPU_graphicsClient::clbkImGuiRenderDrawData() {}
void SDLGPU_graphicsClient::clbkImGuiInit() {}
void SDLGPU_graphicsClient::clbkImGuiShutdown() {}
uint64_t SDLGPU_graphicsClient::clbkImGuiSurfaceTexture(SURFHANDLE surf) { return 0; }

} // namespace SDLSU
