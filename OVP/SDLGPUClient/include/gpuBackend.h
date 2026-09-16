// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __GPUBACKEND_H__
#define __GPUBACKEND_H__

#include "gpuTypes.h"

/**
 * @file gpuBackend.h
 * @brief GPU backend abstraction layer for SDL_gpu.
 * 
 * This interface hides platform-specific GPU details behind a clean API:
 * - device context management
 * - surface/texture allocation and updates
 * - command buffer creation
 * - render pass setup
 * - pipeline state management
 * - blit operations
 */

class GPUTexAllocator {
public:
    virtual ~GPUTexAllocator() = default;
    
    /** Allocate a new SDL GPU texture with specified parameters */
    virtual SDL_GPUTexture* AllocTexture(uint32_t width, uint32_t height,
                                         SDL_GPUTextureFormat format,
                                         SDL_GPUTextureType tex_type = SDL_GPU_TEXTURETYPE_2D) = 0;

    /** Release and free a previously allocated GPU texture */
    virtual void FreeTexture(SDL_GPUTexture* tex) = 0;
    
    /** Update GPU texture from system memory */
    virtual void UpdateTexture(SDL_GPUCmdBuffer cmd,
                               SDL_GPUTexture* tex,
                               const uint8_t* data,
                               int offset_x, int offset_y,
                               int width, int height,
                               size_t row_length) = 0;
};

/**
 * @file gpuBackend.h
 * @brief GPU backend singleton for managing SDL_GPUDevice.
 */

class GpuBackend {
public:
    /** Get or create the primary GPU device from an existing SDL window */
    static GpuBackend* Create(SDL_Window* window);

    /** Shutdown the GPU backend and release all resources */
    virtual void Shutdown() = 0;

    /** Begin a render pass for the current target surface */
    virtual SDL_GPUColorTargetInfo CreateColorTarget(SURFHANDLE surf) = 0;

    /** Get the native device handle */
    SDL_GPUDevice* GetDevice() { return device_; }

    /** Get SDL-GPUTextureFormat for a given Orbiter pixel format */
    static SDL_GPUTextureFormat SurfaceFormat(int bpp);

    /** Query available shader cross backends count */
    static int ShaderCrossAvailable();

protected:
    GpuBackend() : device_(nullptr) {}
    virtual ~GpuBackend();

    SDL_GPUDevice* device_;
};

/**
 * @file gpuShaderBackend.h
 * @brief Runtime shader compilation via SDL3_shadercross.
 * 
 * Compiles HLSL shaders to the target platform native format at runtime:
 * - macOS → Metal MSL
 * - Windows → DXIL/DXBC
 * - Linux → SPIR-V/GLSL
 */

namespace ShaderCross {

/** Initialize SDL3_shadercross and return count of available backends. Returns 0 on failure. */
int Init();

/** Compile HLSL code to target platform native format at runtime. */
bool CompileShader(const char* hlslCode,
                   ShaderTarget targetType,
                   SDL_GPUShaderStage shaderType,
                   ShaderCompilationResult& result);

/** Get the backend name for current platform. Returns "Metal" or "DXC" or "Vulkan". */
const char* GetBackendName();
}

#endif // __GPUBACKEND_H__
