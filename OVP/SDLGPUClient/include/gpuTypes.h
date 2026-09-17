// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __GPUPROPERTIES_H__
#define __GPUPROPERTIES_H__

// Shared types between SDLGPUClient and Orbiter core using SDL_gpu

#include <cmath>
#include <SDL3/SDL_gpu.h>
typedef SDL_GPUCommandBuffer SDL_GPUCmdBuffer;
#include <SDL3_shadercross/SDL_shadercross.h>

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// Forward declarations for graphics APIs
struct SURFACEDESC;
class GpuBackend;
class GPUPipeline;
namespace oapi {
    class Sketchpad;
};

// Texture/surface handle wrapper 
// Opaque handle pointing to GPU backend internal data  
class SURFHANDLE {
public:
    SURFHANDLE() : id(0) {}
    SURFHANDLE(uint32_t i) : id(i) {}
    explicit SURFHANDLE(void* ptr) : id(ptr ? (uint32_t)(uintptr_t)ptr : 0) {}
    
    operator uint32_t() const { return id; }
    operator bool() const { return id != 0; }
    uint32_t getID() const { return id; }
    void* getPtr() const { return id ? (void*)(uintptr_t)id : nullptr; }
    
    bool operator==(const SURFHANDLE& o) const { return id == o.id; }
    bool operator!=(const SURFHANDLE& o) const { return id != o.id; }
    SURFHANDLE& operator=(uint32_t i) { id = i; return *this; }
    SURFHANDLE& operator=(void* ptr) { id = ptr ? (uint32_t)(uintptr_t)ptr : 0; return *this; }

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
