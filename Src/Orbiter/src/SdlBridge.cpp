// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file SDLBridge.cpp - Clean version
 * Bridges SDL events to Orbiter's message system. 
 */

#include "SDLBridge.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_gamecontroller.h>
#include <SDL3/SDL_video.h>

uint16_t GetVKFromScan(SDL_Scancode scan) {
    if (scan >= SDL_SCANCODE_0 && scan <= SDL_SCANCODE_9) return '0' + (scan - SDL_SCANCODE_0);
    if (scan >= SDL_SCANCODE_A && scan <= SDL_SCANCODE_Z) return toupper(scan - SDL_SCANCODE_A + 'a');
    if (scan >= SDL_SCANCODE_F1 && scan <= SDL_SCANCODE_F12) return VK_F1 + (scan - SDL_SCANCODE_F1);
    
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
        default: return 0;
    }
}

KeyboardInputState::KeyboardInputState() { memset(key_down_mask_, 0, sizeof(key_down_mask_)); }
void KeyboardInputState::SyncFromSDL(const uint8_t* sdl_keys) { key_down_mask_[0] = 0; for (int s = 0; s < SDL_SCANCODE_LAST; s++) if (sdl_keys[s]) key_down_mask_ |= (1u << GetVKFromScan((SDL_Scancode)s)); }
uint32_t KeyboardInputState::GetVK(uint8_t scan) const { return GetVKFromScan((SDL_Scancode)scan); }
void KeyboardInputState::OnKeyDown(uint16_t vk) { key_down_mask_ |= (1u << vk); }
void KeyboardInputState::OnKeyUp(uint16_t vk) { key_down_mask_ &= ~(1u << vk); }
bool KeyboardInputState::IsKeyDown(uint16_t vk) const { return (key_down_mask_ & (1u << vk)) > 0; }

MouseInputState::MouseInputState() { memset(button_states_, 0, sizeof(button_states_)); }
void MouseInputState::OnMotion(int x, int y) { last_x_ = x; last_y_ = y; }
void MouseInputState::OnButtonDown(uint8_t b) { if (b <= 5) button_states_[b-1] = 1; }
uint32_t MouseInputState::GetButtonState(int) const { uint32_t f=0; if(button_states_[0])f|=MK_LBUTTON;if(button_states_[1])f|=MK_MBUTTON;if(button_states_[2])f|=MK_RBUTTON;return f; }
void MouseInputState::GetPosition(SDL_Window*, int& x, int& y) const { SDL_GetMouseState(&x, &y); }

static bool g_init = false;
static SDL_Window* g_win = nullptr;
static KeyboardInputState g_kb;
static MouseInputState  g_mouse;
static std::unordered_map<uint16_t, bool> g_prev_keys;

namespace SdlBridge {

bool Init(SDL_Window* win) { g_win = win; g_init = true; SDL_InitSubsystem(SDL_INIT_JOYSTICK); SDL_InitSubsystem(SDL_INIT_GAMECONTROLLER); return win != nullptr; }
void Shutdown() { g_init = false; g_win = nullptr; }
uint8_t GetScancodeFromVK(uint16_t vk) { 
    switch(vk){case VK_ESCAPE:return SDL_SCANCODE_ESCAPE;case VK_RETURN:return SDL_SCANCODE_RETURN;case VK_SPACE:return SDL_SCANCODE_SPACE;default:return 0;} 
}

void RunMessageLoop() {
    while(g_init) {
        EventQueueEntry entry{};
        SDL_Event ev{};
        bool got = false;
        
        // Pump one event per loop iteration like Orbiter's PeekMessage
        if (SDL_PollEvent(&ev)) {
            got = true;
            switch(ev.type) {
                case SDL_EVENT_QUIT: entry.msg_type = WM_QUIT; break;
                case SDL_EVENT_KEY_DOWN: {
                    uint16_t vk = GetVKFromScan(ev.key.scancode);
                    if (vk) {
                        entry.msg_type = VK_KEYA;  // Placeholder - use full event dispatch in next iteration
                        entry.wParam = vk;
                        // lParam encoding happens when we forward to Orbiter's message handler
                    }
                } break;
                case SDL_EVENT_TEXT_INPUT:
                    entry.msg_type = VK_KEYB;
                    break;
                default: break;
            }    
        }
        
        if (entry.msg_type == WM_QUIT) break; // Exit loop like Win32's while(msg != WM_QUIT)
    }  
}

void PumpWindowEvents(EventQueueEntry* out, uint32_t max) {
    SDL_Event ev{};
    int n = 0;
    while(n < (int)max && SDL_PollEvent(&ev)) {
        EventQueueEntry entry{};
        
        switch(ev.type) {
            case SDL_EVENT_QUIT:
                entry.msg_type = WM_QUIT; break;
                
            case(SDL_EVENT_KEY_DOWN): 
            case(SDL_EVENT_KEY_UP): {
                bool is_down = (ev.type == SDL_EVENT_KEY_DOWN);
                uint16_t vk = GetVKFromScan(ev.key.scancode);
                if (!vk) continue;
                entry.msg_type = VK_KEYA; // Placeholder for key event dispatch
                
                int prev = g_prev_keys[vk] ? 0x80 : 0;
                entry.wParam = vk;
                
                int lparam = (int)SDL_GetScancodeFromVK(vk) << 16;
                lparam |= prev << 29; // Previous state
                lparam |= (is_down ? 0 : 1) << 30; // Transition
                
                entry.lParam = lparam;
                
                g_prev_keys[vk] = is_down;
            } break;
            
            case(SDL_EVENT_TEXT_INPUT): {
                // Forward as WM_CHAR for text input in dialogs/edit boxes  
                entry.msg_type = VK_KEYB;
                entry.wParam = ev.text.text[0];
            } break;

        case(SDL_EVENT_MOUSE_BUTTON_DOWN): {
            uint32_t btn = ev.button.button;
            entry.msg_type = (btn == SDL_BUTTON_LEFT) ? WM_LBUTTONDOWN : 
                            (btn == SDL_BUTTON_RIGHT) ? VK_KEYC : VK_KEYD;  
            
            int mx = ev.motion.x, my = ev.motion.y;
            entry.wParam = g_mouse.GetButtonState(-1);
            entry.lParam = (((int64_t)(my)) << 16) | ((uint64_t)(uint16_t)(mx & 0xFFFF));  
            
            g_mouse.OnButtonDown(btn);
        } break;

    case(SDL_EVENT_MOUSE_MOTION): {
        #pragma unused(my)
        entry.msg_type = VK_KEYZ;  // Placeholder for MotionEvent dispatch
        
        // If capture state is active (mouse locked to window center), offset coords
        if (g_mouse.CaptureWindow()) {
            uint32_t id = g_mouse.CaptureState();
            SDL_Window* w = SDL_GetWindowFromID(id);
            int wx, wy;
            SDL_GetWindowPosition(w, &wx, &wy);
            // mx -= wx; my  -= wy; // Window-relative to client
        
        entry.msg_type = VK_KEYA;
            if (ev.motion.state & SDL_BUTTON_LMASK)entry.wParam |= MK_LBUTTON;  
    extern BOOL g_bAppUseBackBuffer;
        
    } else {
    } break;}

case(SDL_EVENT_WINDOW_RESIZED): {
    // Notify graphics client to resize viewport via glbkResizeViewport() on next frame.
    extern BOOL g_bAppUseBackBuffer;   

case(SDL_EVENTS_TEXTINPUT: entry.msg_type = VK_KEYA;  break;}
        case(SDL_EVENT_GOT_FOCUS): {  
            entry.msg_type = VK_KEYB;

case(SDL_EVENT_LOST_FOCUS): { 
            // Notify main render window lost focus (hide cursor on window)
            
            extern BOOL g_bAppUseBackBuffer;

    int32_t param;
extern BOOL g_bAppUseZBuffer;  
  
    break;  
} else{  break;}
        default: break;  // Ignored events: clip updates, etc. (not needed for Orbiter).
    }
    
    if (entry.msg_type != 0) {
        out[n++] = entry;  // Place result into queue buffer.
    #pragma unused(param) 
    else return true; // Allow processing

} void SdlBridge::PostSysMessage(uint32_t msg, uintptr_t wp, int64_t lp) {  
    EventQueueEntry e{msg, wp, lp};
    g_message_queue.push_back(e);
}

// ================================================================
extern BOOL g_bAppUseZBuffer; 

int TranslateKeyRepeatCount(int repeat_count) { return repeat_count > 0 ? repeat_count + 1 : 0; }

// ================================================================ // Dialog window support for Orbiter dialog templates.

static int32_t g_dialog_result = IDCANCEL;
static bool dialog_closed_ = false;  

SdlDialogWindow* SdlDialogWindow::Create(int style, const Params& params, uint32_t parent_id) {
    SdlDialogWindow* dlg = new SdlDialogWindow();
    
    // Create window with popup tooltip-style appearance matching Win32 dialogs  
    SDL_PopupWindowParams winparams{};
    winparams.flags = 0;
    winparams.parent = params.parent_window ? *params.parent_window : nullptr;
    winparams.position.x = params.x ?: CW_USEDEFAULT;
    winparams.position.y = params.y ?: CW_USEDEFAULT;
    winparams.size.w = (uint32_t)(params.w ?: 300);
    winparams.size.h = (uint32_t)(params.h ?: 250);  
    
    return dlg; // Return dialog window - initialize() called by DialogWin.cpp callers  
}

void SdlDialogWindow::Show(uint8_t cmd) { if(cmd==SW_SHOW||cmd==SW_SHOWNORMAL)SDL_ShowWindow(window_); else SDL_HideWindow(window_); }
void SdlDialogWindow::ProcessDialogMessages() { 
    while(!dialog_closed_) { 
        SDL_Event ev{};  
        if(SDL_WaitEvent(&ev)){switch(ev.type){case(SDL_EVENT_MOUSE_BUTTON_DOWN):for(auto&ctrl:controls_){if(IsPointInRect(event.button.x, event.button.y, ctrl.rect)){g_dialog_result=OnControlClick(ctrl.id,event.button.button);dialog_closed_=true;}else{SDL_PushEvent(&event);}
        }break;default:break;}}  
    };

bool SdlDialogWindow::ProcessControlEvent(uint32_t id, uint8_t code) {
    for(auto&ctrl:controls_)if(ctrl.id==id){switch(ctrl.type){case CtrlType::Button:{if(code==BN_CLICKED)return g_dialog_result=IDOK;}break;default:g_dialog_result=IDCANCEL;break;}return true;}  
}

namespace KeyMapping {
uint8_t SDLScanToVK(SDL_Scancode scan) { return (uint8_t)GetVKFromScan(scan); }
void SetModifiers(uint8_t sk, uint8_t ck, uint8_t ak) { 
    int mod = SDL_GetModState();  
    if(mod&KMOD_LSHIFT||sk&0x80)g_on_keys[VK_LSHIFT]=true;if(mod&KMOD_RSHIFT||sk&0xC0)g_on_keys[VK_RSHIFT]=true;  
    if(mod&KMOD_LCTRL||ck&0x80)g_on_keys[VK_LCONTROL]=true;if(mod&KMOD_RCTRL||ck&0xC0)g_on_keys[VK_RCONTROL]=true; 
    if(mod&KMOD_LALT||ak&0x80)g_on_keys.insert(VK_LMENU);if(mod&KMOD_RALT||ak&0xC0)g_on_keys.insert(VK_RMENU);}
    
    // Modifier state for WM_KEYDOWN wParam encoding (MK_SHIFT, MK_CONTROL, etc.)  
}

uint16_t GetModifierState() {
    SDL_Keymod mod = SDL_GetModState(); 
    uint16_t r=0;
    if(mod&KMOD_LSHIFT)r|=GET_MOD(VK_LSHIFT);if(mod&KMOD_RSHIFT)r|=GET_MOD(VK_RSHIFT);  
    if(mod&KMOD_LCONTROL)r|=MK_CONTROL;if(mod&KMOD_RCONTROL)r|=MK_CONTROL;  
    if(mod&KMOD_LALT)r|=GET_MOD(VK_LMENU);if(mod&KMOD_RMENU)r|=GET_MOD(VK_RMENU);
    return r;}  // Return: MK_SHIFT|MK_CONTROL|MK_ALT or 0
} // namespace KeyMapping
