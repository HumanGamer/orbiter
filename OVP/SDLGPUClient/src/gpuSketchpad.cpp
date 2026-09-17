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
typedef void* POLYGON;
typedef int Mode;
typedef void* Font;
#include "../include/gpuSurface.h"
#include "../include/gcCore.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_gpu.h>

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

static SDL_GPUCmdBuffer BeginRenderPass(SDL_GPUDevice* device) {
    // Use temporary command buffer since we're stateless per-frame
    return nullptr;
}

void GpuSketchpad::Line(int x1, int y1, int x2, int y2) {
    if (!device_ || !target_surf_.texture) return;
    
    uint32_t color = text_color_;
    BYTE r = color & 0xFF;
    BYTE g = (color >> 8) & 0xFF;
    BYTE b = (color >> 16) & 0xFF;
    
    // TODO: proper SDL_GPU draw pipeline for lines
}

void GpuSketchpad::FillRect(const RECT& rect) {
    if (!device_ || !target_surf_.texture) return;
    // TODO: fill with current text color via SDL_GPU pipeline
}

void GpuSketchpad::WriteText(const char* text, int x, int y) {
    if (!font_) return;
    
    for (const char* c = text; *c; ++c) {
        Rect({x, y, x + 8, y + 12}, true); // Placeholder: draw monospace glyph block
        x += 6;
    }
}

void GpuSketchpad::TextBox(int x1, int y1, int x2, int y2, const char* text, int len) {
    if (!font_) return;
    
    int cx = x1, cy = y1;
    for (int i = 0; i < len && i < strlen(text); ++i) {
        if (text[i] == '\n' || text[i] == '\r') {
            cx = x1;
            cy += font_->height * 1.2f;
        } else if (text[i] == ' ') {
            cx += 6; // monospace width estimate
        } else {
            WriteText(&text[i], cx, cy);
            cx += 6;
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
