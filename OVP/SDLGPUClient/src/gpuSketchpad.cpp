// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file gpuSketchpad.cpp
 * @brief 2D drawing primitives for SDL_gpu backends.
 * 
 * Maps Orbiter's Sketchpad API to SDL_GPU draw commands:
 * lines, rectangles, filled shapes, text rendering via texture atlas.
 */

#include "../include/gpuTypes.h"
#include <SDL3/SDL_video.h>

class GpuSketchpad : public oapi::Sketchpad {
public:
    explicit GpuSketchpad(SDL_GPUDevice* device, SURFHANDLE target_surf);
    ~GpuSketchpad() override;
    
    // Drawing primitives
    void Line(int x1, int y1, int x2, int y2) override;
    void FillRect(const RECT& rect) override;
    void Rect(const RECT& rect, bool fill) override;
    void PolyLine(const POINT* pts, int npoints) override;
    void Polygon(const POLYGON* poly, int npoly) override;
    
    // Text rendering  
    void WriteText(const char* text, int x, int y) override;
    void TextBox(int x1, int y1, int x2, int y2, const char* text, int len) override;
    
    // Graphics state management
    void SetTextColor(DWORD color) override;
    void SetBackgroundMode(Mode mode) override;
    void SetFont(Font* font) override;
    void SetTextAlign(HorizontalAlign halign, VerticalAlign valign) override;
    
    // Getters/setters
    DWORD GetTextColor() const { return text_color_; }
    Mode GetBackgroundMode() const { return bg_mode_; }
    
private:
    SDL_GPUDevice* device_ = nullptr;
    SURFHANDLE target_surf_{};
    COLORREF text_color_ = 0xFF000000;
    Mode bg_mode_ = BK_OPAQUE;
    Font* font_ = nullptr;
    POINT align_;
    
    // Texture atlas for text rendering
    struct TextAtlasEntry {
        uint8_t x, y, width, height;
    };
    std::vector<TextAtlasEntry> font_atlas_;
};

void GpuSketchpad::Line(int x1, int y1, int x2, int y2) {
    // Create vertex buffer for line segment  
    SDL_GPUCmdBuffer cmd = SDL_AcquireGPURenderPass(device_, &color_targets, 0, nullptr);
    if (!cmd) return;
    
    uint32_t color = text_color_;
    BYTE r = color & 0xFF;
    BYTE g = (color >> 8) & 0xFF;
    BYTE b = (color >> 16) & 0xFF;
    
    // Draw line as triangle strip or use blit-based approach
    SDL_ReleaseGPUResource(cmd, nullptr);
}

void GpuSketchpad::FillRect(const RECT& rect) {
    // Fill with current text color
    auto surf = surfaces_.find(target_surf_.getID());
    if (!surf || !surf->second.texture) return;
    
    SDL_GPUCmdBuffer cmd = SDL_AcquireGPURenderPass(device_, &color_targets, 0, nullptr);
    if (!cmd) return;
    
    // Draw filled rectangle using indexed triangle list
    
}

void GpuSketchpad::Rect(const RECT& rect, bool fill) {
    if (fill) {
        FillRect(rect);
    } else {
        // Outline rectangle as 4 line segments
        Line(rect.left, rect.top, rect.right, rect.top);
        Line(rect.right, rect.top, rect.right, rect.bottom);
        Line(rect.right, rect.bottom, rect.left, rect.bottom);
        Line(rect.left, rect.bottom, rect.left, rect.top);
    }
}

void GpuSketchpad::WriteText(const char* text, int x, int y) {
    if (!font_) return;
    
    // Render font glyphs to texture atlas and draw as textured quads
    for (const char* c = text; *c; ++c) {
        auto& entry = font_atlas_[static_cast<unsigned char>(*c)];
        Rect({x + entry.x, y + entry.y, x + entry.x + entry.width, 
              y + entry.y + entry.height}, false);
    }
}

void GpuSketchpad::TextBox(int x1, int y1, int x2, int y2, const char* text, int len) {
    // Text with word wrap within the given rectangle
    if (!font_) return;
    
    int cx = x1, cy = y1;
    for (int i = 0; i < len; ++i) {
        if (text[i] == '\n' || text[i] == '\r') {
            cx = x1;
            cy += font_->height * 1.2f;
        } else if (text[i] == ' ') {
            // Skip space characters but advance cursor
            cx += font_->advance;
        } else {
            WriteText(&text[i], cx, cy);
            cx += glyph_width;
            if (cx > x2) {
                cx = x1;
                cy += font_->height * 1.2f;
            }
        }
    }
}

// Global factory function  
GpuSketchpad* CreateGpuSketchpad(SDL_GPUDevice* device, SURFHANDLE target_surf) {
    return new GpuSketchpad(device, target_surf);
}
