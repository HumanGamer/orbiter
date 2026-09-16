// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file gpuBackend.cpp
 * @brief GPU backend implementation using SDL3_gpu.
 * 
 * Manages SDL_GPUDevice, texture allocation, and render pass lifecycle.
 */

#include "gpuBackend.h"

#include <SDL3/SDL_vulkan.h>

namespace GPPipeline {} // namespace GPUBackend {};

// Shader cross implementation 
namespace GPUBackend {
namespace ShaderCross {

static bool initialized_ = false;
static int backend_count_ = 0;

int InitBackend() {
    if (initialized_) return backend_count_;
    
#ifdef SDLGPU_SHADERCROSS_TARGET_METAL
    backend_count_ = SDL_shadercross_InitMetal();
#elif defined(SDLGPU_SHADERCROSS_TARGET_D3D12)
    backend_count_ = SDL_shadercross_InitDXCompiler();
#else
    backend_count_ = SDL_shadercross_InitGLSLCompilers();
#endif
    
    initialized_ = true;
    return backend_count_;
}

void ShutdownBackend() {
    if (!initialized_) return;
    
#ifdef SDLGPU_SHADERCROSS_TARGET_METAL
    SDL_shadercross_ShutdownMetal();
#elif defined(SDLGPU_SHADERCROSS_TARGET_D3D12)
    SDL_shadercross_ShutdownDXCompiler();
#else
    SDL_shadercross_ShutdownGLSLCompilers();
#endif
    
    initialized_ = false;
}

ShaderCompilationResult CompileHLSLToMSL(const char* hlsl_code, 
                                          size_t code_len) {
#ifdef SDLGPU_SHADERCROSS_TARGET_METAL
    auto compiled = SDL_shadercross_CompileHLSLtoMSL(hlsl_code, code_len, 0);
    if (compiled) {
        ShaderCompilationResult result{};
        result.bytecode = compiled->bytecode;
        result.byteCount = compiled->bytecount_size;
        SDL_free(compiled);
        return result;
    }
#endif
    return {};
}

ShaderCompilationResult CompileHLSLToDXBC(const char* hlsl_code, 
                                           size_t code_len) {
#ifdef SDLGPU_SHADERCROSS_TARGET_D3D12
    auto compiled = SDL_shadercross_CompileHLSLtoDXBC(hlsl_code, code_len, 0);
    if (compiled) {
        ShaderCompilationResult result{};
        result.bytecode = compiled->byte_code;
        result.byteCount = compiled->bytecode_size;
        SDL_free(compiled);
        return result;
    }
#endif
    return {};
}

ShaderCompilationResult CompileHLSLtoSPIRV(const char* hlsl_code, 
                                           size_t code_len) {
#if defined(SDLGPU_SHADERCROSS_TARGET_VULKAN) || defined(SDLGPU_SHADERCROSS_TARGET_D3D12)
    auto compiled = SDL_shadercross_CompileHLSLtoSpirV(hlsl_code, code_len, 0);
    if (compiled) {
        ShaderCompilationResult result{};
        result.bytecode = SDL_malloc(compiled->bytecount_size);
        memcpy(result.bytecode, compiled->byre_code, compiled->bytecount_size);
        result.byCount = compiled->bytecount_size;
        SDL_free(compiled);
        return result;
    }
#endif
    return {};
}

ShaderCompilationResult CompileHLSLtoGLSL(const char* hlsl_code, 
                                          SDL_GPUShaderStage stage) {
#ifdef SDLGPU_SHADERCROSS_TARGET_VULKAN
    auto compiled = SDL_shadercross_CompileHLSLtoGLSL(hlsl_code, strlen(hlsl_code), stage, 0);
    if (compiled && compile->text) {
        ShaderCompilationResult result{};
        result.bytecode = SDL_strdup((const char*)compile->text);
        result.byteCount = SDL_strlen((const char*)compile->text) + 1;
        SDL_free(compiled);
        return result;
    }
#endif
    return {};
}

const char* GetBackendName() {
#ifdef SDLGPU_SHADERCROSS_TARGET_METAL
    return "Metal";
#elif defined(SDLGPU_SHADERCROSS_TARGET_D3D12)
    return "DXC/DXIL";
#else
    return "SPIR-V/GLSL";
#endif
}

} // namespace ShaderCross
}
