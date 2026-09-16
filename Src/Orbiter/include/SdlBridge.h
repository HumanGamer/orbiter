// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file include/SdlBridge.h
 * @brief Bridges SDL event system to Orbiter's existing dialog and message framework.
 * 
 * This replaces Win32's GetMessage/PeekMessage/TranslateMessage/DispatchMessage
 * infrastructure with equivalent SDL functionality:
 * - Event loop feeding modal dialogs and modeless windows  
 * - Keyboard scancode → virtual key conversion (VK codes)
 * - Mouse coordinate translation and capture emulation  
 * - Dialog window management via SDL_CreateWindow() + parent relationships
 * 
 * The bridge maintains the existing Win32 message parameter signatures so that
 * Orbiter's 50+ DialogWin callers don't need changes. Messages are translated
 * on-the-fly between SDL events and WM_* codes.
 */

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT
#include "platform_sdl.h"
#endif
#include <cstdint>
#include <vector>
#include <set>
#include <functional>

struct EventQueueEntry {
    uint32_t msg_type; // WM_ style code (WM_KEYDOWN, WM_LBUTTONDOWN, etc.)
    uintptr_t wParam;
    int64_t lParam;
    double timestamp;
};

/** Keyboard virtual key conversion from SDL scancodes to VK codes */
namespace KeyMapping {
    /** Convert SDL scancode to Windows virtual key code */
    extern uint8_t SDLScanToVK(SDL_Scancode scan);
    
    /** Set keyboard modifier state (shift, ctrl, alt) */
    extern void SetModifiers(uint8_t vk_shift, uint8_t vk_ctrl, uint8_t vk_alt);
    
    /** Get current modifiers as a WORD matching Win32 LMULTIFIRESTATE return value */
    extern uint16_t GetModifierState();
}

/** Dialog window abstraction that bridges to Orbiter's DialogWin API */
class SdlDialogWindow {
public:
    struct Params {
        const char* class_name = "#32770"; // Default dialog class
        const DLGTEMPLATE* template_ptr = nullptr;  
        const void* user_data = nullptr;
        
        /** Position in screen coordinates (nullptr = use template positioning) */
        int x, y, w, h;
    };
    
    /** Create a modal or modeless dialog window from a resource template */
    static SdlDialogWindow* Create(int style, const Params& params, uint32_t parent_id);
    
    /** Show/hide the dialog window */
    void Show(uint8_t show_cmd) { show_state_ = show_cmd; }
    
    /** Get the SDL_Window handle for this dialog */
    SDL_Window* Window() const { return window_; }
    
    /** Enable/disable WM_* message interception for modal dialogs */
    static void ProcessDialogMessages();
    
private:
    SdlDialogWindow() : window_(nullptr), show_state_(SW_SHOWDEFAULT) {}
    ~SdlDialogWindow() { if (window_) SDL_DestroyWindow(window_); }
    
    bool Initialize(const Params& params, uint32_t parent_id);
    
    /** Process a dialog control event (button click, edit change, etc.) */
    bool ProcessControlEvent(uint32_t ctrl_id, uint8_t notification_code);
    
    SDL_Window* window_;
    uint8_t show_state_;
    std::vector<DLGCTRL> controls_;
};

/** Main bridge class - coordinates event loop and dialog management */
class SdlBridge {
public:
    static bool Initialize(SDL_Window* main_window);
    static void Shutdown();
    static int RunMessageLoop();
    
    /** Translate SDL event to WM_* code - add to event queue */
    static void PumpWindowEvents(EventQueueEntry* out_events, uint32_t max_entries);
    
    /** Inject a synthesized WM_* message (used for dialog responses) */  
    static void PostSysMessage(uint32_t msg, uintptr_t wparam = 0, int64_t lparam = 0);
};

/** Convert SDL key event to VK virtual keys and update modifier state */
class KeyboardInputState {
public:
    KeyboardInputState();
    
    /** Update keyboard state from current SDL_GetKeyboardState snapshot */
    void SyncFromSDL(const uint8_t* sdl_keys);
    
    /** Get VK code for a pressed scan key */
    uint32_t GetVK(uint8_t vk_scan_code) const;
    
    /** Check if virtual key is currently down (after modifier translation) */
    bool IsKeyDown(uint16_t vk_code) const { return state_.count(vk_code) > 0; }
    
    /** Track VK code press/release (for WM_KEYDOWN/UP events) */
    void OnKeyDown(uint16_t vk);
    void OnKeyUp(uint16_t vk);

private:
    std::set<uint16_t> state_;
    std::vector<int64_t> key_timestamps_; // Maps VK code → last-pressed timestamp
};

/** Track mouse state including cursor position, capture, and button states */
class MouseInputState {
public:
    MouseInputState();
    
    /** Update from SDL mouse event */
    void OnMotion(int x, int y);
    void OnButtonDown(uint8_t btn);
    void OnButtonUp(uint16_t btn);
    
    /** Get current cursor position relative to a window */
    void GetPosition(SDL_Window* target_window, int& out_x, int& out_y) const;
    
    uint32_t CaptureState() const { return capture_window_; }

private:
    mutable int last_x_ = -1;
    mutable int last_y_ = -1;
    std::vector<int> button_states_; // SDL_BUTTON_LEFT/RIGHT/MIDDLE
    uint32_t capture_window_ = 0;
};
