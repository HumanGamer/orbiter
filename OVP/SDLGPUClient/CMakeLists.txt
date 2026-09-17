# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# SDLGPUClient build instructions for graphics client

cmake_minimum_required(VERSION 3.19)

# Abstract GPU backend for OpenOrbiter using SDL_gpu
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SDLGPUClient VERSION 1.0)

# Dependencies - find system-installed or via CMake package config
find_package(SDL3 REQUIRED)
find_package(SDL3_shadercross CONFIG REQUIRED)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE_TARGET_DIR ${ORBITER_BINARY_SDK_DIR}/include)

# Configure shader cross target based on platform
option(SDLGPU_SHADERCROSS_USE_VULKAN "Use Vulkan backend for SDL_gpu shader compilation" OFF)
option(SDLGPU_SHADERCROSS_USE_METAL "Use Metal backend for SDL_gpu shader cross compilation" OFF)  
option(SDL_GPU_SHADERCROSS_USE_D3D12 "Use D3D12/DXC backend for SDL_gup shadercross" OFF)

if(APPLE)
    set(SDLGPU_SHADERCROSS_TARGET "metal")
    add_compile_definitions(SDLGPU_SHADERCROSS_TARGET_METAL=1)
elseif(MSVC)
    set(SDLGPU_SHADERCROSS_TARGET "dxc")
    add_compile_definitions(SDLGPU_SHADERCROSS_TARGET_D3D12=1)
else()
    find_package(Vulkan QUIET)
    if(Vulkan_FOUND)
        set(SDLGPU_SHADERCROSS_TARGET "vulkan")
        add_compile_definitions(SDLGPU_SHADERCROSS_TARGET_VULKAN=1)
    else()
        message(WARNING "Vulkan not found. Shader cross may need glslang alternative.")
    endif()
endif()

# Core source files - SDL_gpu implementation of graphics client
set(SOURCE_FILES
    src/gpuBackend.cpp
    src/gpuSurfaces.cpp
    src/gpuSketchpad.cpp
    src/gpuMesh.cpp
    src/gpuParticle.cpp
    src/pipeline/shadercross.cpp  
    src/gcCore.cpp
    src/OapiExtension.cpp
)

# Header files in include/ and src/
set(HEADER_FILES  
    include/gpuTypes.h
    include/gpuPipeline.h
    include/gpuSurface.h
    include/gpuBackend.h  
    include/gcCore.h
    src/pipeline/shaderCross.h
    src/pipeline/shader_compiler.h
)

# HLSL shader source files - compiled at runtime via shadercross to GLSL/Metal/DXIL
set(ShaderFiles
    shaders/Common.hlsl
    shaders/MeshVS.hlsl
    shaders/MeshFS.hlsl 
    shaders/ParticleVS.hlsl
    shaders/ParticleFS.hlsl
    shaders/SkyVS.hlsl
    shaders/SkyFS.hlsl
    shaders/AtmoVS.hlsl
    shaders/AtmoFS.hlsl
    shaders/BeaconArrayVS.hlsl
    shaders/BeaconArrayFS.hlsl
    shaders/HazeMgr.hlsl
    shaders/HorizonHaze.vs.hlsl
    shaders/HorizonHaze.fs.hlsl
    shaders/SplatTerrain.vs.hlsl  
    shaders/SplatTerrain.fs.hlsl
    shaders/PBR.vs.hlsl
    shaders/PBR.fs.hlsl
)

# ABI/API headers from D3D9Client (they're shared between clients)
set(APIHeaders
    ${CMAKE_SOURCE_DIR}/OVP/D3D9Client/gcGUI.h  
    ${CMAKE_SOURCE_DIR}/OVP/D3D9Client/gcCore.h
)

source_group(APIHeaders FILES ${APIHeaders})
source_group(Shaders FILES ${ShaderFiles})

# Build SDLGPUClient as a module (shared library/DLL)
add_library(SDLGPUClient MODULE
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${APIHeaders}  
    ${ShaderFiles}
)

target_include_directories(SDLGPUClient PUBLIC
    ${ORBITER_SOURCE_SDK_INCLUDE_DIR}
    ${SRC_DIR}/include
    ${SDL3_INCLUDE_DIRS}
    ${ORBITER_SOURCE_DIR}/include
)

# Link to libraries - orbitersdk provides the oapi::GraphicsClient interface
target_link_libraries(SDLGPUClient PRIVATE
    SDL3::SDL3
    SDL3_shadercross  
    orbitersdk
    orbiterlib
    ${CMAKE_DL_LIBS}
)

if(APPLE)
    target_link_libraries(SDLGPUClient PRIVATE
        -framework Metal
        -framework MetalKit  
        -framework AppKit
        -framework QuartzCore
    )
elseif(MSVC)
    # DXC/DXIL shader cross on Windows requires d3d12, dxgi, and the compiler lib
    target_link_libraries(SDLGPUClient PRIVATE
        d3d12.lib
        dxgi.lib
        DxcApiCompile.lib  
        dxcompiler.lib
    )
else()
    if(Vulkan_FOUND)
        target_link_libraries(SDLGPUClient PRIVATE Vulkan::Vulkan)
    endif()
endif()

# Output to Modules/Plugin directory (same as D3D9Client)  
set_target_properties(SDLGPUClient
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Modules/Plugin"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Modules/Plugin"
        FOLDER "OVP"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

# Install shaders directory alongside the plugin DLL in its subdirectory
install(TARGETS SDLGPUClient DESTINATION ${ORBITER_INSTALL_PLUGIN_DIR})
install(DIRECTORY ${SRC_DIR}/shaders/ DESTINATION ${ORBITER_INSTALL_ROOT_DIR}/Modules/SDLGPUClient)
