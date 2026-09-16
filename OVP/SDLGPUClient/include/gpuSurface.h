// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __GPUSERFACE_H__
#define __GPUSERFACE_H__

#include "gpuTypes.h"

/**
 * @file gpuSurface.h
 * @brief GPU surface and texture management.
 * 
 * Maps into Orbiter's SURFHANDLE abstract handles for textures, surfaces, etc.
 * Manages SDL_GPUTextures with mipmaps, different formats, and blit support.
 */

class GpuSurfaceManager {
public:
    /** Create a new surface/texture of given dimensions and format */
    virtual SURFHANDLE CreateTexture(int width, int height, SDL_GPUTextureFormat format) = 0;
    
    /** Load a texture from system memory image data into GPU memory */
    virtual void UpdateTexture(SURFHANDLE surf, const uint8_t* data, int width, int height) = 0;
    
    /** Copy surface contents to another (blit) */
    virtual void Blit(SDL_GPUCmdBuffer cmd, SURFHANDLE src, SDL_GPUTexture* dstTex, 
                     int srcX, int srcY, int srcW, int srcH, int dstX, int dstY) = 0;
    
    /** Get the raw SDL GPU texture handle from a SURFHANDLE */
    virtual SDL_GPUTexture* GetNativeTexture(SURFHANDLE surf) const = 0;
    
    /** Release a surface handle and free associated resources */
    virtual void ReleaseSurface(SURFHANDLE surf) = 0;
    
    /** Get width/height of a surface */
    virtual void GetSize(SURFHANDLE surf, int* w, int* h) const = 0;
    
    /** Generate mipmaps for the given texture */
    virtual void GenerateMipmaps(SDL_GPUCmdBuffer cmd, SDL_GPUTexture* tex, uint32_t width, uint32_t height) = 0;
};

#endif // __GPUSERFACE_H__
