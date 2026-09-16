// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file gpuParticle.cpp
 * @brief GPU particle stream rendering using SDL_gpu.
 */

#include "../include/gpuTypes.h"
#include "../include/gpuPipeline.h"

class GpuParticlePipeline : public GPUPipeline {
public:
    bool Initialize(SDL_GPUDevice* device) override;
    void Shutdown() override;
    void Reset() override;
    
    /** Create a particle emitter with given parameters */
    void CreateEmitter(int max_particles, SURFHANDLE particle_texture);
    
    /** Render active particle stream as billboarded quads */
    void RenderParticles(const PARTICLE* particles, int count, 
                         const float* view_matrix, float alpha);

private:
    SDL_GPUBuffer* particle_buf_ = nullptr;
    uint32_t max_particles_ = 0;
    SURFHANDLE texture_{};
};

bool GpuParticlePipeline::Initialize(SDL_GPUDevice* device) {
    device_ = device;
    
    // Particle vertex buffer placeholder - not implemented yet
    particle_buf_ = nullptr;
    max_particles_ = 0;
    
    return true;
}

void GpuParticlePipeline::Shutdown() {
    if (particle_buf_) {
        SDL_DestroyGPUBuffer(device_, particle_buf_);
        particle_buf_ = nullptr;
    }
}

void GpuParticlePipeline::Reset() {}

void GpuParticlePipeline::CreateEmitter(int max_particles, SURFHANDLE tex) {
    texture_ = tex;
    max_particles_ = max_particles;
}

void GpuParticlePipeline::RenderParticles(const PARTICLE* particles, int count, 
                                           const float* view, float alpha) {
    // TODO: particle rendering via SDL_GPU draw pipeline
    (void)particles;
    (void)count;
    (void)view;
    (void)alpha;
}

GPUPipeline* PipelineFactory::CreateParticlePipeline() {
    return new GpuParticlePipeline();
}
