// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file SdlBridge.cpp
 * Bridges SDL events to Orbiter's message system. 
 */

#include "SDLBridge.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_gamecontroller.h>
#include <SDL3/SDL_video.h>

#include <cstdint>
#include <cctype>
#include <cstring>
#include <unordered_map>
#include <vector>

uint16_t GetVKFromScan(SDL_Scancode scan) {
    if (scan >= SDL_SCANCODE_0 && scan <= SDL_SCANCODE_9) return '0' + (uint16_t)(scan - SDL_SCANCODE_0);
    if (scan >= SDL_SCANCODE_A && scan <= SDL_SCANCODE_Z) return toupper((int)(scan - SDL_SCANCODE_A + 'a'));
    if (scan >= SDL_SCANCODE_F1 && scan <= SDL_SCANCODE_F12) return VK_F1 + (uint16_t)(scan - SDL_SCANCODE_F1);
    
    switch (scan) {
        case SDL_SCANCODE_ESCAPE: return VK_ESCAPE;
        case SDL_SCANCODE_RETURN: return VK_RETURN;
        case SDL_SCANCODE_BACKSPACE: return VK_BACK;
        case SDL_SCANCODE_TAB: return VK_TAB;
        case SDL_SCANCODE_SPACE: return VK_SPACE;
        case SDL_SCANCODE_MINUS: return VK_OEM_MINUS;
        case SDL_SCANCODE_EQUALS: return VK_OEM_PLUS;
        case SDL_SCANCODE_LEFTBRACKET: return VK_OEM_4;
        case SDL_SCANCODE_RIGHTBRACKET: return VK_OEM_6;
        case SDL_SCANCODE_BACKSLASH: return VK_OEM_PIPE;
        case SDL_SCANCODE_SEMICOLON: return VK_OEM_1;
        case SDL_SCANCODE_APOSTROPHE: return VK_OEM_7;
        case SDL_SCANCODE_GRAVE: return VK_OEM_3;
        case SDL_SCANCODE_COMMA: return VK_OEM_COMMA;
        case SDL_SCANCODE_PERIOD: return VK_DECIMAL;
        case SDL_SCANCODE_SLASH: return VK_DIVIDE;
        case SDL_SCANCODE_KP_0: return VK_NUMPAD0;
        case SDL_SCANCODE_KP_1: return VK_NUMPAD1;
        case SDL_SCANCODE_KP_2: return VK_NUMPAD2;
        case SDL_SCANCODE_KP_3: return VK_NUMPAD3;
        case SDL_SCANCODE_KP_4: return VK_NUMPAD4;
        case SDL_SCANCODE_KP_5: return VK_NUMPAD5;
        case SDL_SCANCODE_KP_6: return VK_NUMPAD6;
        case SDL_SCANCODE_KP_7: return VK_NUMPAD7;
        case SDL_SCANCODE_KP_8: return VK_NUMPAD8;
        case SDL_SCANCODE_KP_9: return VK_NUMPAD9;
        case SDL_SCANCODE_KP_PERIOD: return VK_DECIMAL;
        case SDL_SCANCODE_KP_DIVIDE: return VK_DIVIDE;
        case SDL_SCANCODE_KP_MULTIPLY: return VK_MULTIPLY;
        case SDL_SCANCODE_KP_MINUS: return VK_SUBTRACT;
        case SDL_SCANCODE_KP_PLUS: return VK_ADD;
        case SDL_SCANCODE_KP_ENTER: return VK_RETURN;
        case SDL_SCANCODE_PRINTSCREEN: return VK_SNAPSHOT;
        case SDL_SCANCODE_SCROLLLOCK: return VK_SCROLL;
        case SDL_SCANCODE_PAUSE: return VK_PAUSE;
        case SDL_SCANCODE_INSERT: return VK_INSERT;
        case SDL_SCANCODE_HOME: return VK_HOME;
        case SDL_SCANCODE_PAGEUP: return VK_PRIOR;
        case SDL_SCANCODE_DELETE: return VK_DELETE;
        case SDL_SCANCODE_END: return VK_END;
        case SDL_SCANCODE_PAGEDOWN: return VK_NEXT;
        case SDL_SCANCODE_RIGHT: return VK_RIGHT;
        case SDL_SCANCODE_LEFT: return VK_LEFT;
        case SDL_SCANCODE_DOWN: return VK_DOWN;
        case SDL_SCANCODE_UP: return VK_UP;
        case SDL_SCANCODE_LSHIFT: return VK_LSHIFT;
        case SDL_SCANCODE_RSHIFT: return VK_RSHIFT;
        case SDL_SCANCODE_LCTRL: return VK_LCONTROL;
        case SDL_SCANCODE_RCTRL: return VK_RCONTROL;
        case SDL_SCANCODE_LALT: return VK_LMENU;
        case SDL_SCANCODE_RALT: return VK_RMENU;
        case SDL_SCANCODE_CAPSLOCK: return VK_CAPITAL;
        case SDL_SCANCODE_NUMLOCKCLEAR: return VK_NUMLOCK;
        default: return 0;
    }
}

KeyboardInputState::KeyboardInputState() { 
    memset(key_down_mask_, 0, sizeof(key_down_mask_)); 
}

void KeyboardInputState::SyncFromSDL(const uint8_t* sdl_keys) {
    key_down_mask_[0] = 0;
    for (int s = 0; s <= SDL_SCANCODE_LAST; s++) {
        if (sdl_keys[s]) {
            uint16_t vk = GetVKFromScan((SDL_Scancode)s);
            if (vk) key_down_mask_ |= (1u << vk);
        }
    }
}

uint32_t KeyboardInputState::GetVK(uint8_t scan) const { 
    return GetVKFromScan((SDL_Scancode)scan); 
}

void KeyboardInputState::OnKeyDown(uint16_t vk) { key_down_mask_ |= (1u << vk); }
void KeyboardInputState::OnKeyUp(uint16_t vk) { key_down_mask_ &= ~(1u << vk); }
bool KeyboardInputState::IsKeyDown(uint16_t vk) const { return (key_down_mask_ & (1u << vk)) != 0; }

MouseInputState::MouseInputState() { 
    memset(button_states_, 0, sizeof(button_states_)); 
}

void MouseInputState::OnMotion(int x, int y) { last_x_ = x; last_y_ = y; }
void MouseInputState::OnButtonDown(uint8_t b) { if (b <= 5) button_states_[b-1] = 1; }
void MouseInputState::OnButtonUp(uint16_t /*b*/) { /* TODO: add button up logic */ }

uint32_t MouseInputState::GetButtonState(int /*unused*/) const { 
    uint32_t f = 0;
    if (button_states_[0]) f |= MK_LBUTTON;
    if (button_states_[1]) f |= MK_MBUTTON;
    if (button_states_[2]) f |= MK_RBUTTON;
    return f;
}

void MouseInputState::GetPosition(SDL_Window*, int& x, int& y) const { 
    SDL_GetMouseState(&x, &y); 
}

static bool g_init = false;
static SDL_Window* g_win = nullptr;
static KeyboardInputState g_kb;
static MouseInputState  g_mouse;
static std::unordered_map<uint16_t, bool> g_prev_keys;
static std::vector<EventQueueEntry> g_message_queue;

namespace SdlBridge {

bool Initialize(SDL_Window* win) { 
    g_win = win; 
    g_init = true; 
    SDL_InitSubsystem(SDL_INIT_JOYSTICK); 
    SDL_InitSubsystem(SDL_INIT_GAMECONTROLLER); 
    return win != nullptr; 
}

void Shutdown() { 
    g_init = false; 
    g_win = nullptr; 
    g_message_queue.clear(); 
}

uint8_t GetScancodeFromVK(uint16_t vk) { 
    switch(vk){
        case VK_ESCAPE: return SDL_SCANCODE_ESCAPE;
        case VK_RETURN: return SDL_SCANCODE_RETURN;
        case VK_SPACE: return SDL_SCANCODE_SPACE;
        default: return 0;
    } 
}

void PostSysMessage(uint32_t msg, uintptr_t wp, int64_t lp) {  
    g_message_queue.push_back({msg, wp, lp, (double)SDL_GetTicks() / 1000.0});
}

void PumpWindowEvents(EventQueueEntry* out, uint32_t max_entries) {
    SDL_Event ev{};
    int n = 0;
    
    while(n < (int)max_entries && !g_message_queue.empty()) {
        out[n++] = g_message_queue.front();
        g_message_queue.erase(g_message_queue.begin());
    }
    
    while(n < (int)max_entries && SDL_PollEvent(&ev)) {
        EventQueueEntry entry{};
        
        switch(ev.type) {
            case SDL_EVENT_QUIT:
                entry.msg_type = WM_QUIT; 
                out[n++] = entry;
                return;
                
            case SDL_EVENT_KEY_DOWN: 
            case SDL_EVENT_KEY_UP: {
                bool is_down = (ev.type == SDL_EVENT_KEY_DOWN);
                uint16_t vk = GetVKFromScan(ev.key.scancode);
                if (!vk) break;
                
                entry.msg_type = is_down ? WM_KEYDOWN : WM_KEYUP;
                entry.wParam = vk;
                
                int prev = g_prev_keys[vk] ? 0x80 : 0;
                uint32_t lparam = (uint32_t)SDL_GetScancodeFromVK(vk);
                lparam |= prev << 16; // Repeat count
                lparam |= is_down ? 0 : (1 << 30); // Transition bit
                lparam |= 1 << 31; // Extended flag
                
                entry.lParam = lparam;
                
                g_prev_keys[vk] = is_down;
                out[n++] = entry;
            } break;
            
            case SDL_EVENT_TEXT_INPUT: {
                if (ev.text.text && ev.text.text[0]) {
                    entry.msg_type = WM_CHAR;
                    entry.wParam = (uintptr_t)ev.text.text[0];
                    out[n++] = entry;
                }
            } break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN: 
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                uint8_t btn = ev.button.button;
                bool is_down = (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
                
                if (btn == SDL_BUTTON_LEFT) {
                    entry.msg_type = is_down ? WM_LBUTTONDOWN : WM_LBUTTONUP;
                } else if (btn == SDL_BUTTON_MIDDLE) {
                    entry.msg_type = is_down ? WM_MBUTTONDOWN : WM_MBUTTONUP;
                } else if (btn == SDL_BUTTON_RIGHT) {
                    entry.msg_type = is_down ? WM_RBUTTONDOWN : WM_RBUTTONUP;
                } else {
                    break;
                }
                
                int32_t mx = ev.motion.x, my = ev.motion.y;
                entry.wParam = g_mouse.GetButtonState(-1);
                if (SDL_GetModState() & KMOD_SHIFT) entry.wParam |= MK_SHIFT;
                if (SDL_GetModState() & KMOD_CTRL) entry.wParam |= MK_CONTROL;
                entry.lParam = ((int64_t)(my & 0xFFFF) << 32) | ((uint64_t)(mx & 0xFFFF));  
                
                is_down ? g_mouse.OnButtonDown(btn) : /* TODO OnButtonUp */;
                out[n++] = entry;
            } break;

            case SDL_EVENT_MOUSE_MOTION: {
                int mx = ev.motion.x, my = ev.motion.y;
                
                if (g_mouse.CaptureState()) {
                    g_mouse.OnMotion(mx, my);
                    entry.msg_type = WM_MOUSEMOVE;
                    entry.wParam = g_mouse.GetButtonState(-1);
                    entry.lParam = ((int64_t)(my & 0xFFFF) << 32) | ((uint64_t)(mx & 0FFFF));
                    out[n++] = entry;
                } else {
                    int prev_x, prev_y;
                    g_mouse.GetPosition(nullptr, prev_x, prev_y);
                    if (mx != prev_x || my != prev_y) {
                        g_mouse.OnMotion(mx, my);
                        entry.msg_type = WM_MOUSEMOVE;
                        entry.wParam = g_mouse.GetButtonState(-1);
                        entry.lParam = ((int64_t)(my & 0xFFFF) << 32) | ((uint64_t)(mx & 0xFFFF));
                        out[n++] = entry;
                    }
                }
            } break;

            case SDL_EVENT_WINDOW_RESIZED:
                entry.msg_type = WM_SIZE;
                entry.wParam = SIZE_MAXIMIZE;
                entry.lParam = ((int64_t)(ev.window.data2) << 32) | (uint64_t)(uintptr_t)ev.window.data1;
                out[n++] = entry;
                break;

            case SDL_EVENT_WINDOW_FOCUS_GAINED: {  
                entry.msg_type = WM_ACTIVATE;
                entry.wParam = WA_ACTIVE;
                out[n++] = entry;
            } break; 
            
            case SDL_EVENT_WINDOW_FOCUS_LOST: { 
                entry.msg_type = WM_ACTIVATEAPP;
                entry.wParam = FALSE;
                out[n++] = entry;
            } break;

            default:
                break;
        }
    }
}

int TranslateKeyRepeatCount(int repeat_count) { 
    return repeat_count > 0 ? repeat_count + 1 : 0; 
}

static int32_t g_dialog_result = IDCANCEL;
static bool dialog_closed_ = false;  

SdlDialogWindow* SdlDialogWindow::Create(int style, const Params& params, uint32_t parent_id) {
    (void)style; (void)parent_id;
    SdlDialogWindow* dlg = new SdlDialogWindow();
    
    SDL_PopupWindowParams winparams{};
    winparams.flags = 0;
    winparams.parent = params.parent_window ? *params.parent_window : nullptr;
    winparams.position.x = params.x != CW_USEDEFAULT ? params.x : CW_USEDEFAULT;
    winparams.position.y = params.y != CW_USEDEFAULT ? params.y : CW_USEDEFAULT;
    winparams.size.w = params.w ? (uint32_t)params.w : 300;
    winparams.size.h = params.h ? (uint32_t)params.h : 250;  
    
    return dlg; 
}

void SdlDialogWindow::Show(uint8_t cmd) { 
    if(cmd == SW_SHOW || cmd == SW_SHOWNORMAL) SDL_ShowWindow(window_); 
    else if(cmd == SW_HIDE) SDL_HideWindow(window_);
}

void SdlDialogWindow::ProcessDialogMessages() { 
    while(!dialog_closed_) { 
        SDL_Event ev{};  
        if(SDL_WaitEvent(&ev)) {
            switch(ev.type) {
                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    for(auto& ctrl : controls_) {
                        if(ctrl.rect.x <= ev.motion.x && ev.motion.x < ctrl.rect.x + ctrl.rect.w &&
                           ctrl.rect.y <= ev.motion.y && ev.motion.y < ctrl.rect.y + ctrl.rect.h) {
                            g_dialog_result = OnControlClick(ctrl.id, ev.button.button);
                            dialog_closed_ = true;
                        } else {
                            SDL_PushEvent(&ev);
                        }
                    }
                } break;
                default:
                    SDL_PushEvent(&ev);
            }
        }  
    }
}

void SdlDialogWindow::OnKeyDown(uint16_t vk) { 
    if (vk == VK_ESCAPE || vk == VK_RETURN) {
        g_dialog_result = (vk == VK_RETURN) ? IDOK : IDCANCEL;
        dialog_closed_ = true;
    }
}

bool SdlDialogWindow::ProcessControlEvent(uint32_t id, uint8_t code) {
    for(auto& ctrl : controls_) {
        if(ctrl.id == id) {
            switch(ctrl.type) {
                case CtrlType::Button:
                    if(code == BN_CLICKED) return (g_dialog_result = IDOK) != 0;
                    break;
                default:
                    g_dialog_result = IDCANCEL;
                    break;
            }
            return true;
        }  
    }
    return false;
}

namespace KeyMapping {

uint8_t SDLScanToVK(SDL_Scancode scan) { 
    return (uint8_t)GetVKFromScan(scan); 
}

// Modifier tracking via external key state maps
std::unordered_map<uint16_t, bool>* g_on_keys = nullptr;

void SetModifiers(uint8_t sk, uint8_t ck, uint8_t ak) { 
    int mod = SDL_GetModState();  
    if(mod & KMOD_LSHIFT || (sk & 0x80)) {
        if(g_on_keys) (*g_on_keys)[VK_LSHIFT] = true;
    }
    if(mod & KMOD_RSHIFT || (sk & 0xC0)) {
        if(g_on_keys) (*g_on_keys)[VK_RSHIFT] = true;
    }
    if(mod & KMOD_LCTRL || (ck & 0x80)) {
        if(g_on_keys) (*g_on_keys)[VK_LCONTROL] = true;
    }
    if(mod & KMOD_RCTRL || (ck & 0xC0)) {
        if(g_on_keys) (*g_on_keys)[VK_RCONTROL] = true;
    }
    if(mod & KMOD_LALT || (ak & 0x80)) {
        if(g_on_keys) (*g_on_keys)[VK_LMENU] = true;
    }
    if(mod & KMOD_RALT || (ak & 0xC0)) {
        if(g_on_keys) (*g_on_keys)[VK_RMENU] = true;
    }
}
    
uint16_t GetModifierState() {
    SDL_Keymod mod = SDL_GetModState(); 
    uint16_t r = 0;
    if(mod & KMOD_LSHIFT) r |= 0x1;  // simplified MK_SHIFT
    if(mod & KMOD_RSHIFT) r |= 0x1;
    if(mod & KMOD_LCONTROL || mod & KMOD_RCONTROL) r |= MK_CONTROL;
    if(mod & KMOD_LALT) r |= GET_MOD(VK_LMENU);
    if(mod & KMOD_RALT) r |= GET_MOD(VK_RMENU);
    return r;
}

} // namespace KeyMapping

} // namespace SdlBridge
