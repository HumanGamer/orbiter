// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __GPUPROPERTIES_H__
#define __GPUPROPERTIES_H__

// Shared types between SDLGPUClient and Orbiter core using SDL_gpu

#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// Forward declarations for graphics APIs
struct SURFACEDESC;
class GPUBackend;
class GPUPipeline;

// Texture/surface handle wrapper
// Opaque handle pointing to GPU backend internal data  
class SURFHANDLE {
public:
    SURFHANDLE() : id(0) {}
    SURFHANDLE(uint32_t i) : id(i) {}
    
    operator bool() const { return id != 0; }
    uint32_t getID() const { return id; }
    
    bool operator==(const SURFHANDLE& o) const { return id == o.id; }
    bool operator!=(const SURFHANDLE& o) const { return id != o.id; }

private:
    uint32_t id;
};

// Forward declare the surface descriptor
struct SurfaceDesc {
    int width, height;
    SDL_GPUTextureFormat format;
    SDL_GPUTextureType type;
    bool hasMipmaps;
    uint32_t refcount;
    SURFHANDLE handle;
};

// Shader types enum for shadercross compilation targets
enum class ShaderTarget {
    GLSL,
    SPIRV,
    Metal,
    HLSL,
    DXIL
};

// Shader compilation result from runtime shadercross invocation
struct ShaderCompilationResult {
    void* bytecode;
    size_t byteCount;
    
    ~ShaderCompilationResult() {
        if (bytecode) SDL_free(bytecode);
    }
};

#endif // __GPUPROPERTIES_H__
