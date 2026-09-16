// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __SHADERCROSS_H__
#define __SHADERCROSS_H__

#include "gpuTypes.h"

/**
 * @file pipeline/shaderCross.h
 * @brief SDL3_shadercross integration for runtime shader compilation.
 */

#pragma once

namespace GPUBackend {
namespace ShaderCross {

int InitBackend();
void ShutdownBackend();

ShaderCompilationResult CompileHLSLToMetal(const char* hlsl_code);
ShaderCompilationResult CompileHLSLToDXBC(const char* hlsl_code);
ShaderCompilationResult CompileHLSLToSPIRV(const char* hlsl_code);
ShaderCompilationResult CompileHLSLToGLSL(const char* hlsl_code, 
                                          SDL_GPUShaderStage stage);

} // namespace ShaderCross  
} // namespace GPUBackend

#endif // __SHADERCROSS_H__
