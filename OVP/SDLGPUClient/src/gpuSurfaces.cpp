// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file gpuSurfaces.cpp
 * @brief GPU surface/texture management using SDL_gpu textures.
 */

#include "gpuBackend.h"
#include "../include/gpuSurface.h"
#include "../include/gpuTypes.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_gpu.h>
#include <stb/stb_image.h>

class GpuSurfaceManagerImpl : public GpuSurfaceManager {
public:
    explicit GpuSurfaceManagerImpl(SDL_GPUDevice* device) : device_(device) {}
    
    SURFHANDLE CreateTexture(int width, int height, SDL_GPUTextureFormat format) override;
    void UpdateTexture(SURFHANDLE surf, const uint8_t* data, int w, int h) override;
    void Blit(SDL_GPUCmdBuffer cmd, SURFHANDLE src, SDL_GPUTexture* dstTex, 
              int srcX, int srcY, int srcW, int srcH, int dstX, int dstY) override;
    SDL_GPUTexture* GetNativeTexture(SURFHANDLE surf) const override;
    void ReleaseSurface(SURFHANDLE surf) override;
    void GetSize(SURFHANDLE surf, int* w, int* h) const override;
    void GenerateMipmaps(SDL_GPUCmdBuffer cmd, SDL_GPUTexture* tex, 
                         uint32_t width, uint32_t height) override;

private:
    struct SurfaceEntry {
        SDL_GPUTexture* texture = nullptr;
        int width = 0;
        int height = 0;
        SDL_GPUTextureFormat format = SDL_GPU_TEXTUREFORMAT_UNKNOWN;
        int refcount = 1;
    };
    
    std::unordered_map<uint32_t, SurfaceEntry> surfaces_;
    mutable SDL_mutex* mutex_ = nullptr;
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
    
    auto key = surfaces_.size() + 1;
    SurfaceEntry entry{};
    entry.texture = tex;
    entry.width = width;
    entry.height = height;
    entry.format = format;
    entry.refcount = 1;
    surfaces_[key] = entry;
    
    return SURFHANDLE(key);
}

void GpuSurfaceManagerImpl::UpdateTexture(SURFHANDLE surf, const uint8_t* data, int w, int h) {
    auto it = surfaces_.find(surf.getID());
    if (it == surfaces_.end() || !it->second.texture) return;
    
    SDL_GPUTransferBufferDesc tbuf_desc{};
    tbuf_desc.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tbuf_desc.size = w * h * 4;
    SDL_GPUTransferBuffer* tbuf = SDL_CreateGPUTransferBuffer(device_, &tbuf_desc);
    
    SDL_CPUCopyToGPUTransferBuffer(device_, tbuf, data, w * h * 4);
    
    SDL_GPUCmdBuffer cmd = SDL_AquireGPUTempCommandBuffer(device_, 0);
    if (cmd) {
        SDL_GPUBlitInfo blit{};
        blit.src_image.texture = nullptr;
        blit.dst_image.texture = it->second.texture;
        SDL_UploadToGPUTextureFromCPUBuffer(cmd, tbuf, {0, 0, 0, w}, it->second.texture, {0, 0, w, h});
        SDL_ReleaseGPUTempCommandBuffer(device_, cmd);
    }
    
    SDL_DestroyGPUTransferBuffer(device_, tbuf);
}

SDL_GPUTexture* GpuSurfaceManagerImpl::GetNativeTexture(SURFHANDLE surf) const {
    auto it = surfaces_.find(surf.getID());
    if (it == surfaces_.end()) return nullptr;
    return it->second.texture;
}

void GpuSurfaceManagerImpl::ReleaseSurface(SURFHANDLE surf) {
    auto it = surfaces_.find(surf.getID());
    if (it == surfaces_.end()) return;
    
    it->second.refcount--;
    if (it->second.refcount <= 0 && it->second.texture) {
        SDL_DestroyGPUTexture(device_, it->second.texture);
        it->second.texture = nullptr;
        surfaces_.erase(it);
    }
}

void GpuSurfaceManagerImpl::GetSize(SURFHANDLE surf, int* w, int* h) const {
    auto it = surfaces_.find(surf.getID());
    if (it != surfaces_.end()) {
        *w = it->second.width;
        *h = it->second.height;
    }
}

void GpuSurfaceManagerImpl::GenerateMipmaps(SDL_GPUCmdBuffer cmd, 
                                            SDL_GPUTexture* tex, 
                                            uint32_t width, uint32_t height) {
    if (!cmd || !tex) return;
    
    int mip_levels = SDL_CalculateGPUTextureMipCount(width, height);
    for (int i = 1; i < mip_levels; i++) {
        uint32_t src_w = width >> (i-1);
        uint32_t src_h = height >> (i-1);
        uint32_t dst_w = width >> i;
        uint32_t dst_h = height >> i;
        
        SDL_GPUBlitInfo blit{};
        blit.src_image.texture = tex;
        blit.dst_image.texture = tex;
        SDL_Rect src_rect = {0, 0, (int)src_w, (int)src_h};
        SDL_Rect dst_rect = {0, 0, (int)dst_w, (int)dst_h};
        blit.src_image.source_rect = &src_rect;
        blit.dst_image.destination_rect = &dst_rect;
        blit.src_image.level = i - 1;
        blit.dst_image.level = i;
        
        SDL_BlitGPUCommandBuffer(cmd, &blit);
    }
}

namespace GpuSurface {

GpuSurfaceManager* CreateGpuSurfaceManager(SDL_GPUDevice *device) {
    return new GpuSurfaceManagerImpl(device);
}

} // namespace GpuSurface
