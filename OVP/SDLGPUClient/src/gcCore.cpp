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
extern oapi::GraphicsClient *gclient;

// Surface entry storage for the graphics client
struct SurfaceEntry {
    SDL_GPUTexture* texture = nullptr;
    int width = 0;
    int height = 0;
    SDL_GPUTextureFormat format = SDL_GPU_TEXTUREFORMAT_UNKNOWN;
};

namespace SDLGPU {

// ======================================================================
// Implementation helper
// ======================================================================

class GpuSurfaceManager {
public:
    virtual ~GpuSurfaceManager() = default;
    virtual SURFHANDLE CreateTexture(int width, int height, SDL_GPUTextureFormat format) = 0;
    virtual void UpdateTexture(SURFHANDLE surf, const uint8_t* data, int w, int h) = 0;
    virtual SDL_GPUTexture* GetNativeTexture(SURFHANDLE surf) const = 0;
    virtual bool ReleaseSurface(SURFHANDLE surf) = 0;
    virtual void GetSize(SURFHANDLE surf, int* w, int* h) const = 0;
};

class GpuSurfaceManagerImpl : public GpuSurfaceManager {
public:
    explicit GpuSurfaceManagerImpl(SDL_GPUDevice* device) : device_(device) {}
    
    SURFHANDLE CreateTexture(int width, int height, SDL_GPUTextureFormat format) override;
    void UpdateTexture(SURFHANDLE surf, const uint8_t* data, int w, int h) override;
    SDL_GPUTexture* GetNativeTexture(SURFHANDLE surf) const override;
    bool ReleaseSurface(SURFHANDLE surf) override;
    void GetSize(SURFHANDLE surf, int* w, int* h) const override;

private:
    std::unordered_map<uint32_t, SurfaceEntry> surfaces_;
    SDL_GPUDevice* device_ = nullptr;
};

SURFHANDLE GpuSurfaceManagerImpl::CreateTexture(int width, int height, SDL_GPUTextureFormat format) {
    SDL_GPUTextureDescriptor desc{};
    desc.type = SDL_GPU_TEXTURETYPE_2D;
    desc.format = format;
    desc.width = width;
    desc.height = height;
    desc.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    
    SDL_GPUTexture* tex = SDL_CreateGPUTexture(device_, &desc);
    if (!tex) return SURFHANDLE(0);
    
    auto key = (uint32_t)(uintptr_t)( surfaces_.size() + 1);
    SurfaceEntry entry{};
    entry.texture = tex;
    entry.width = width;
    entry.height = height;
    entry.format = format;
    surfaces_[key] = entry;
    
    return SURFHANDLE((void*)(uintptr_t)key);
}

void GpuSurfaceManagerImpl::UpdateTexture(SURFHANDLE surf, const uint8_t* data, int w, int h) {
    auto key = (uint32_t)(uintptr_t)surf.getPtr();
    auto it = surfaces_.find(key);
    if (it == surfaces_.end() || !it->second.texture) return;
    
    // Upload image data to texture - SDL_gpu 3.x API
    // Uses temporary command buffer approach for GPU upload
    SDL_GPUCmdBuffer cmd = SDL_AcquireGPUTempCommandBuffer(device_, 0);
    if (cmd) {
        int src_w = w, src_h = h;
        SDL_BlitGPU to dest texture blit operation
        
        SDL_ReleaseGPUTempCommandBuffer(device_, cmd);
    }
}

SDL_GPUTexture* GpuSurfaceManagerImpl::GetNativeTexture(SURFHANDLE surf) const {
    auto key = (uint32_t)(uintptr_t)surf.getPtr();
    auto it = surfaces_.find(key);
    if (it == surfaces_.end()) return nullptr;
    return it->second.texture;
}

bool GpuSurfaceManagerImpl::ReleaseSurface(SURFHANDLE surf) {
    auto key = (uint32_t)(uintptr_t)surf.getPtr();
    auto it = surfaces_.find(key);
    if (it == surfaces_.end()) return false;
    
    if (it->second.texture) {
        SDL_DestroyGPUTexture(device_, it->second.texture);
        it->second.texture = nullptr;
    }
    surfaces_.erase(it);
    return true;
}

void GpuSurfaceManagerImpl::GetSize(SURFHANDLE surf, int* w, int* h) const {
    auto key = (uint32_t)(uintptr_t)surf.getPtr();
    auto it = surfaces_.find(key);
    if (it != surfaces_.end()) {
        *w = it->second.width;
        *h = it->second.height;
    }
}

// ======================================================================
// Surface management implementation
// ======================================================================

SURFHANDLE SDLGPU_graphicsClient::clbkCreateSurface(DWORD w, DWORD h, SURFHANDLE hTemplate) {
    if (!device_) return nullptr;
    
    // Create surface using our manager
    auto* mgr = GetSurfaceManager();
    if (!mgr) return nullptr;
    
    SDL_GPUTextureFormat fmt = SDL_GPU_TEXTUREFORMAT_RGBA8_UNORM_SRGB;
    return mgr->CreateTexture((int)w, (int)h, fmt);
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
        surface_mgr_->UpdateTexture(tex, pixels, w, h);
    }
    
    stbi_image_free(pixels);
    return tex;
}

bool SDLGPU_graphicsClient::clbkReleaseSurface(SURFHANDLE surf) {
    if (!device_ || !surface_mgr_) return false;
    return surface_mgr_->ReleaseSurface(surf);
}

bool SDLGPU_graphicsClient::clbkGetSurfaceSize(SURFHANDLE surf, DWORD *w, DWORD *h) {
    if (!surface_mgr_) return false;
    int iw = 0, ih = 0;
    surface_mgr_->GetSize(surf, &iw, &ih);
    *w = (DWORD)iw;
    *h = (DWORD)ih;
    return true;
}

// ======================================================================
// Blitting and display
// ======================================================================

bool SDLGPU_graphicsClient::clbkBlt(SURFHANDLE tgt, DWORD tgtx, DWORD tgty, 
                                     SURFHANDLE src, DWORD flag) const {
    if (!device_ || !surface_mgr_) return false;
    
    SDL_GPUTexture *src_tex = surface_mgr_->GetNativeTexture(src);
    SDL_GPUTexture *dst_tex = surface_mgr_->GetNativeTexture(tgt);
    if (!src_tex || !dst_tex) return false;
    
    // Get temp command buffer for blit
    SDL_GPUCmdBuffer cmd = SDL_AquireGPUTempCommandBuffer(device_, 0);
    if (!cmd) return false;
    
    RECT src_rect = {0, 0, (LONG)tgtW(), (LONG)tgtH()};
    RECT dst_rect = {(LONG)tgtx, (LONG)tgty, (LONG)(tgtx + tgtW()), (LONG)(tgty + tgtH())};
    
    SDL_GPUImageBlit blit{};
    blit.src_image.texture = src_tex;
    blit.dst_image.texture = dst_tex;
    blit.src_rect = &src_rect;
    blit.dst_rect = &dst_rect;
    
    SDL_BlitGPUCommandBuffer(cmd, &blit, 1);
    
    SDL_ReleaseGPUTempCommandBuffer(device_, cmd);
    return true;
}

bool SDLGPU_graphicsClient::clbkDisplayFrame() {
    if (!device_ || !window_) return false;
    
    // Present the frame - swap the back buffer
    // TODO: SDL GPU present implementation for window
    return true;
}

// ======================================================================
// Initialize shader cross for runtime compilation
// ======================================================================

bool SDLGPU_graphicsClient::clbkInitialise() {
    hInstance = g_pOrbiter->GetInstance();
    
    // Initialize shader cross - compile HLSL shaders to target platform native format
    int backends = GPUBackend::ShaderCross::InitBackend();
    (void)backends;
    
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

namespace GpuSurface {

GpuSurfaceManager* CreateGpuSurfaceManager(SDL_GPUDevice *device) {
    return new GpuSurfaceManagerImpl(device);
}

} // namespace GpuSurface
