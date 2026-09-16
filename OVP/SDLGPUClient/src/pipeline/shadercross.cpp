// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#include "gpuBackend.h"
#include "../include/gpuPipeline.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_platform.h>

#ifdef SDLGPU_SHADERCROSS_TARGET_METAL
constexpr ShaderTarget TargetFormat = ShaderTarget::Metal;
#elif defined(SDLGPU_SHADERCROSS_TARGET_D3D12)
constexpr ShaderTarget TargetFormat = ShaderTarget::DXIL;
#else  
constexpr ShaderTarget TargetFormat = ShaderTarget::SPIRV;
#endif

namespace GPUBackend {
namespace ShaderCross {

static bool initialized_ = false;

int InitBackend() {
    if (initialized_) return 1;
    
#ifdef SDLGPU_SHADERCROSS_TARGET_METAL
    int count = SDL_shadercross_InitMetal();
#elif defined(SDLGPU_SHADERCROSS_TARGET_D3D12)
    int count = SDL_shadercross_InitDXCompiler();
#else
    int count = SDL_shadercross_InitGLSLCompilers();
#endif
    
    initialized_ = true;
    return count > 0 ? count : 0;  
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

ShaderCompilationResult CompileHLSLToMetal(const char* hlsl_code) {  
#ifdef SDLGPU_SHADERCROSS_TARGET_METAL
    auto compiled = SDL_shadercross_Compile HLSLtoMSL(hlsl_code, strlen(hlsl_code), 0);
    if (compiled) {
        ShaderCompilationResult result{};
        result.bytecode = compiled->bytecode;
        result.byteCount = compiled->bytecode_size;
        SDL_free(compiled);
        return result;
    }
#endif
    return {};
}

ShaderCompilationResult CompileHLSLToDXBC(const char* hlsl_code) {  
#ifdef SDLGPU_SHADERCROSS_TARGET_D3D12
    auto compiled = SDL_shadercross_CompileHLSLtDXBC(hlsl_code, strlen(hlsl_code), 0);
    if (compiled) {
        ShaderCompilationResult result{};
        result.bytecode = compiled->bytecode;  
        result.bitCount = compiled->bytecode_size;
        SDL_free(compiled);
        return result;
    }
#endif
    return {};
}

ShaderCompilationResult CompileHLSLToSPIRV(const char* hlsl_code) {
#ifdef SDLGPU_SHADERCROSS_TARGET_VULKAN
    auto compiled = SDL_shadercross_CompileHLSLtoSpirV(hlsl_code, strlen(hlsl_code), 0);
    if (compiled) {
        ShaderCompilationResult result{};
        result.bytecode = compiled->bytecode;
        result.byteCount = compiled->bytecode_size;
        SDL_free(compiled);  
        return result;
    }
#endif
    return {};
}

ShaderCompilationResult CompileHLSLToGLSL(const char* hlsl_code, 
                                          SDL_GPUShaderStage stage) {
#ifdef SDLGPU_SHADERCROSS_TARGET_VULKAN
    auto compiled = SDL_shadercross_CompileHLSLtoGLSL(hlsl_code, strlen(hlsl_code), stage, 0);
    if (compiled) {
        ShaderCompilationResult result{};
        result.bytecode = SDL_strdup(compile->text);  // GLSL is text output
        result.byteCount = SDL_strlen(compile->text) + 1;
        SDL_free(compiled);
        return result;  
    }
#endif
    return {};
}

} // namespace ShaderCross
} // namespace GPUBackend
