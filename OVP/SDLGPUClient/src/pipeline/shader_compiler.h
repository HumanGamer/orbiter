// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#pragma once

/**
 * @file pipeline/shader_compiler.h
 * @brief Runtime shader compilation using SDL3_shadercross.
 * 
 * This module wraps SDL_shadercross to compile HLSL shaders at runtime
 * into the native format for the current platform (Metal on macOS,
 * DXC/DXIL on Windows, GLSL/SPIR-V on Linux).
 */

#include <unordered_map>
#include <string_view>

struct CompiledShader {
    void* bytecode;
    size_t byte_count;
    SDL_GPUShaderFormat format;
    
    ~CompiledShader() {
        if (bytecode) SDL_free(bytecode);
    }
};

class ShaderCompiler {
public:
    static bool Initialize();
    static void Shutdown();
    
    /** Compile HLSL source to bytecode. Results are cached by shader name + hash. */
    static CompiledShader* Compile(const char* name, 
                                    const char* hlsl_source,
                                    SDL_GPUShaderStage stage);

private:
    static bool initialized_;
    static std::unordered_map<std::string_view, CompiledShader*> cache_;
};
