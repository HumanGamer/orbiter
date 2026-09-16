#pragma once
#include <SDL3/SDL.h>
typedef enum { SDL_SHADERCROSS_TARGET_OS_ANY=0, SDL_SHADERCROSS_TARGET_OS_WINDOWS, SDL_SHADERCROSS_TARGET_OS_LINUX, SDL_SHADERCROSS_TARGET_OS_MACOS, SDL_SHADERCROSS_TARGET_OS_IOS, SDL_SHADERCROSS_TARGET_OS_ANDROID } SDL_ShaderCross_TargetOS;
typedef enum { SDL_SHADERCROSS_SHADER_TYPE_VERTEX, SDL_SHADERCROSS_SHADER_TYPE_FRAGMENT, SDL_SHADERCROSS_SHADER_TYPE_COMPUTE } SDL_ShaderCross_ShaderType;
typedef struct SDL_ShaderCross_ComputePipelineInfo { SDL_ShaderCross_ShaderType type; SDL_ShaderCross_TargetOS targetOS; int workgroupSizeX; int workgroupSizeY; int workgroupSizeZ; } SDL_ShaderCross_ComputePipelineInfo;
typedef struct SDL_ShaderCross_VertexShaderInfo { SDL_ShaderCross_ShaderType type; SDL_ShaderCross_TargetOS targetOS; int numTextureBindings; int numSamplerBindings; int numUniformBindings; } SDL_ShaderCross_VertexShaderInfo;
typedef struct SDL_ShaderCross_FragmentShaderInfo { SDL_ShaderCross_ShaderType type; SDL_ShaderCross_TargetOS targetOS; int numTextureBindings; int numSamplerBindings; int numUniformBindings; } SDL_ShaderCross_FragmentShaderInfo;
typedef void* SDL_ShaderCross_SPIRVToGPUPipeline;
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToGPUPrecompiledPCZVertexShader(void* renderer, void* info) { return nullptr; }
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToGPUPrecompiledPCZFragmentShader(void* renderer, void* info) { return nullptr; }
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToGPUPrecompiledBlitShader(void* renderer) { return nullptr; }
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToGLSLVertexShader(void* renderer, void* info, unsigned char* spirv, int len) { return nullptr; }
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToGLSLFragmentShader(void* renderer, void* info, unsigned char* spirv, int len) { return nullptr; }
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToMSLComputeShader(void* renderer, void* info, unsigned char* spirv, int len) { return nullptr; }
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToMSLVertexShader(void* renderer, void* info, unsigned char* spirv, int len) { return nullptr; }
static inline SDL_ShaderCross_SPIRVToGPUPipeline SDL_ShaderCross_SPIRVToMSLFragmentShader(void* renderer, void* info, unsigned char* spirv, int len) { return nullptr; }
static inline int SDL_ShaderCross_GenerateSPIRVFragmentShader(void* renderer, unsigned char** out, int* len, void* info, const char* fragmentShader) { *out = nullptr; *len = 0; return 0; }
static inline int SDL_ShaderCross_GenerateSPIRVComputeShader(void* renderer, unsigned char** out, int* len, void* info, const char* computeShader) { *out = nullptr; *len = 0; return 0; }
static inline void SDL_ShaderCross_FreeSPIRV(unsigned char* spirv) { (void)spirv; }
static inline void SDL_ShaderCross_FreeComputePipeline(void* pipeline) { (void)pipeline; }
static inline void SDL_ShaderCross_FreeVertexShader(void* shader) { (void)shader; }
static inline void SDL_ShaderCross_FreeFragmentShader(void* shader) { (void)shader; }
