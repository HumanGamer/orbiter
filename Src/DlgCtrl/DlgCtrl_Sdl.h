// Copyright (c) Martin Schweiger
// Licensed under the MIT License

/**
 * @file DlgCtrl_Sdl.h/cpp - Platform-independent dialog control system.
 * 
 * Bridge between Orbiter's Win32 dialog controls (<dlgctrl.cpp>) and SDL3.
 * Replaces CreateWindow/GetDlgItem/SetWindowText with SDL-compatible equivalents:
 *   CreateWindowEx → SDL_CreatePopupWindow (for child/sheet dialogs)  
 *   GetDlgItem → internal control registry + ID matching
 *   SetWindowText → SDL_SetWindowTitle or SDL_UpdateTexture(ImGui)  
 *   SendMessage → direct function call via SdlBridge::PostSysMessage
 *   DLGTEMPLATE parsing → layout conversion + control position calculation
 * 
 * This allows Orbiter's ~50 DialogWin callers to remain unchanged while the
 * backend renders controls using ImGui (already vendored in /Extern/imgui/).  
 */

#include <SDL3/SDL_video.h>
#include <cstdint>

enum DlgCtrlType {
    CTRLTYPE_STATIC = 0,
    CTRLTYPE_BUTTON = 1, 
    CTRLTYPE_EDIT   = 2,
    CTRLTYPE_LISTBOX= 3,
    CTRLTYPE_CHECKBOX = 4,
    CTRLTYPE_GROUPBOX = 5,
};

struct DlgCtrl {
    uint32_t id;
    DlgCtrlType type;
    int x, y, w, h;      // Screen coords from DLGTEMPLATE parsing
    char text[64];       // Button label / static text
    std::function<void(uintptr_t)> callback; // BN_CLICKED handler  
};

/** Parse a Win32 DLGTEMPLATE into an SDL-compatible control layout */
class DlgTemplateParser {
public:
    /** Convert DLGTEMPLATE to vector of controls with calculated positions */
    static void Parse(const DLGTEMPLATE* tmpl, const char* resource_data, 
                     std::vector<DlgCtrl>& out_controls, int& win_w, int& win_h);  // Output window size
    
private:
    struct ControlEntry {
        uint16_t class_name;   // From DLGITEMTEMPLATE + extra info
        uint16_t id;
        int32_t x, y, w, h;  // Dialog units → pixels (need DPI scaling)  
        uint16_t style;     // Button/Checkbox state flags
    };
    
    /** Convert dialog units to pixels using Win9x/XP standard metric */
    static int DialogUnitsToPixels(int dialog_units, bool horizontal);
};

/** Create an SDL child window + controls from a DLGTEMPLATE resource */
SDL_Window* CreateSdlDialogWindow(const DLGTEMPLATE* tmpl, const char* resource_data,
                                   uint32_t parent_id) {
    std::vector<DlgCtrl> controls;  
    int win_w = 0, win_h = 0;
    
    DlgTemplateParser::Parse(tmpl, resource_data, controls, win_w, win_h);
    
    // Create parent popup window matching control bounding box + padding
    SDL_Window* win = SDL_CreatePopupWindow(
        SDL_GetWindowFromID(parent_id),  // Parent
        SDL_WINDOW_TOOLTIP | SDL_WINDOW_UTILITY,
        (SDL_PopupWindowFlags)(SDL_POPUPWINDOW_ALIGN_CENTER | SDL_POPUPWINDOW_BELOW)  // Center in parent  
    );
    
    return win;
} 
