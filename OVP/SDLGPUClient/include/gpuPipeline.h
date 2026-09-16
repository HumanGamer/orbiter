// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __GPUPIPELINE_H__
#define __GPUPIPELINE_H__

#include "gpuTypes.h"

/**
 * @file gpuPipeline.h
 * @brief Abstract GPU render pipeline interface.
 * 
 * Each concrete pipeline manages its own shaders, vertex/index buffers,
 * and draw state. The backend factory creates specific pipelines as needed.
 */

class GPUPipeline {
public:
    virtual ~GPUPipeline() = default;
    
    /** Initialize the pipeline with a GPU device. Returns true on success. */
    virtual bool Initialize(SDL_GPUDevice* device) = 0;
    /** Release all pipeline resources. */
    virtual void Shutdown() = 0;
    /** Reset pipeline state to defaults (called each frame). */
    virtual void Reset() = 0;
    
protected:
    SDL_GPUDevice* device_ = nullptr;
};

/**
 * @file gpuPipeline.h
 * @brief Pipeline factory for creating specific render pipelines.
 * 
 * This keeps individual pipeline class counts low while providing a single
 * entry point for the backend to get appropriate pipeline instances.
 */
class PipelineFactory {
public:
    static GPUPipeline* CreateMeshPipeline();
    static GPUPipeline* CreateTerrainPipeline(); 
    static GPUPipeline* CreateParticlePipeline();
    static GPUPipeline* CreateSkyPipeline();
    static GPUPipeline* CreateAtmospherePipeline();
    static GPUPipeline* CreateBeaconArrayPipeline();
    
    static void DestroyPipeline(GPUPipeline* pipeline);
};

