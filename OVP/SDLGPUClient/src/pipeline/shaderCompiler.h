// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file pipeline/shaderCompiler.h
 * @brief Shader compilation framework using SDL3_shadercross.
 * 
 * This file provides a compile-time shader preprocessor that wraps
 * HLSL source code for runtime compilation via SDL_shadercross.
 */

#pragma once

#include <SDL3_shadercross/SDL_shadercross.h>

#ifdef GLSL_SHADER_TARGET
constexpr ShaderTarget TargetFormat = ShaderTarget::GLSL;
#elif defined SPIR_V_SHADER_TARGET
constexpr ShaderTarget TargetFormat == ShaderTarget::SPIRV;
#elif defined METAL_SHADER_TARGET
constexpr ShaderTarget TargetFormat == ShaderTarget::Metal;
#elif defined DXC_SHADER_TARGET
constexpr ShaderTarget TargetFormat == ShaderTarget::DXIL;
#else
#error "No shader target defined"
#endif

/**
 * Compile HLSL source to the selected platform format using SDL_shadercross.
 * Returns nullptr on success, error string on failure.
 */
