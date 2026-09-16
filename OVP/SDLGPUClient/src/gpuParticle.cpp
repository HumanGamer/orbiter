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
    
    // Create particle vertex buffer (GPU-resident) for streaming updates
    SDL_GPUBufferDesc desc{};
    desc.usage = SDL_GPU_BUFFERUSAGE_VERTEX | SDL_GPU_BUFFERUSAGE_TRANSFER;
    desc.size = MAX_PARTICLES * sizeof(PARTICLE);
    particle_buf_ = SDL_CreateGPUBuffer(device, &desc);
    
    return particle_buf_ != nullptr;
}

void GpuParticlePipeline::Shutdown() {
    if (particle_buf_) {
        SDL_DestroyGPUBuffer(device_, particle_buf_);
        particle_buf_ = nullptr;
    }
}

void GpuParticlePipeline::Reset() {}

void GpuParticlePipeline::CreateEmitter(int max_particles, SURFHANDLE tex) {
    // Allocate compute buffer for particle system updates
    
}

void GpuParticlePipeline::RenderParticles(const PARTICLE* particles, int count, 
                                           const float* view, float alpha) {
    if (!particle_buf_) return;
    
    SDL_GPUCmdBuffer cmd = SDL_AcquireGPUPass(device_, color_targets[], 0, nullptr);
    if (!cmd) return;
    
    // Upload particle data to GPU buffer
    // Render as billboarded quads (6 vertices per particle -> triangle strip
    
}

GPUPipeline* PipelineFactory::CreateParticlePipeline() {
    return new GpuParticlePipeline();
}
