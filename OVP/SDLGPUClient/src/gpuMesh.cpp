// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file gpuMesh.cpp
 * @brief GPU mesh rendering pipeline using SDL_gpu.
 * 
 * Maps Orbiter's mesh system to SDL_GPU: vertex buffers, index buffers,
 * shaders, and transforms with support for multiple materials/textures.
 */

#include "../include/gpuTypes.h"
#include "../include/gpuPipeline.h"
#include <SDL3/SDL_video.h>
#include <unordered_map>
#include <vector>

/** Mesh descriptor - maps to Orbiter MESHHANDLE */
struct MeshDescriptor {
    SDL_GPUBuffer* vert_buf = nullptr;
    SDL_GPUBuffer* index_buf = nullptr;
    uint32_t vert_count = 0;
    uint32_t index_count = 0;
    std::vector<MaterialEntry> materials;
};

/** Material entry - maps to orbital MATERIAL struct */
struct MaterialEntry {
    float ambient[4];
    float diffuse[4]; 
    float specular[4];
    float emissive[4];
    uint32_t texture_binding[8]; // max 8 texture stages
};

class GpuMeshPipeline : public GPUPipeline {
public:
    bool Initialize(SDL_GPUDevice* device) override;
    void Shutdown() override;
    void Reset() override;
    
    /** Draw a single mesh instance with given world/view/projection matrices */
    void DrawMesh(MESHHANDLE mesh_handle,
                  const float* world_matrix,
                  const float* view_matrix,
                  const float* proj_matrix);
    
    /** Set material for the next draw call */
    void SetMaterial(const MATERIAL* mat) override;
    
    /** Add texture binding to current material */
    void SetTextureBinding(uint32_t stage, SURFHANDLE tex) override;

private:
    SDL_GPUTransferBuffer* upload_buf_ = nullptr;
    std::unordered_map<uint32_t, MeshDescriptor> mesh_cache_;
    
    // Compiled shader for mesh rendering
    struct GpuShaderHandle {
        SDL_GPUShader* vertex_shader = nullptr;
        SDL_GPUShader* fragment_shader = nullptr;
        
        ~GpuShaderHandle() {
            if (vertex_shader) SDL_DestroyGPUShader(vertex_shader);
            if (fragment_shader) SDL_DestroyGPUShader(fragment_shader);
        }
    };
    
    std::unordered_map<uint32_t, GpuShaderHandle> shader_cache_;
};

bool GpuMeshPipeline::Initialize(SDL_GPUDevice* device) {
    device_ = device;
    
    // Create transfer buffer for staging mesh uploads
    SDL_GPUBufferDesc buf_desc{};
    buf_desc.usage = SDL_GPU_BUFFERUSAGE_TRANSFER;
    buf_desc.size = 1024 * 1024 * 8; // 8MB staging area
    upload_buf_ = SDL_CreateGPUTransferBuffer(device, &buf_desc);
    
    return upload_buf_ != nullptr;
}

void GpuMeshPipeline::Shutdown() {
    if (upload_buf_) {
        SDL_DestroyGPUTransferBuffer(device_, upload_buf_);
        upload_buf_ = nullptr;
    }
    
    mesh_cache_.clear();
}

void GpuMeshPipeline::Reset() {
    // Reset any per-frame pipeline state
}

void GpuMeshPipeline::DrawMesh(MESHHANDLE mesh_handle,
                               const float* world_matrix,
                               const float* view_matrix, 
                               const float* proj_matrix) {
    auto it = mesh_cache_.find(mesh_handle.getID());
    if (it == mesh_cache_.end() || !it->second.vert_buf) return;
    
    MeshDescriptor& mesh = it->second;
    
    // Acquire render pass for current frame
    SDL_GPUCmdBuffer cmd = SDL_AcquireGPURenderPass(device_, &color_targets, 0, nullptr);
    if (!cmd) return;
    
    // Upload vertex buffer to GPU
    if (needs_upload_) {
        SDL_GPUBufferTransferSpec transfer{};
        transfer.buffer = upload_buf_;
        transfer.data = mesh_data_buffer_.data();
        SDL_UploadToGPUBuffer(cmd, &transfer);
    }
    
    for (auto& mat : mesh.materials) {
        // Set pipeline with material properties
        // Draw using indexed primitives
    }
}

void GpuMeshPipeline::SetMaterial(const MATERIAL* mat) {
    // Update current draw state material
}

void GpuMeshPipeline::SetTextureBinding(uint32_t stage, SURFHANDLE tex) {  
    // Map texture handle to GPU texture for pipeline binding
}

/** Pipeline factory implementation */
GPUPipeline* PipelineFactory::CreateMeshPipeline() {
    return new GpuMeshPipeline();
}

void PipelineFactory::DestroyPipeline(GPUPipeline* pipeline) {
    if (pipeline) delete pipeline;
}

