// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for commctrl.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#include "platform_sdl.h"

// Far proc and handle stubs
typedef LRESULT (CALLBACK* FARPROC)(HWND, UINT, WPARAM, LPARAM);
typedef void* HGLOBAL;

// Common control window classes
#define WC_LISTVIEW       "SysListView32"
#define WC_TREEVIEW       "SysTreeView32"
#define WC_TABCONTROL     "SysTabControl32"
#define WC_STATUSBAR      "msctls_statusbar32"
#define WC_PROGRESS       "msctls_progress32"
#define WC_REBAR          "ReBarWindow32"
#define WC_TOOLBAR        "ToolbarWindow32"
#define WC_TRACKBAR       "msctls_trackbar32"
#define WC_UPDOWN         "msctls_updown32"

// ListView styles
#define LVS_ICON          0x0000
#define LVS_LIST          0x0001
#define LVS_SMALLICON     0x0002
#define LVS_DETAILS       0x0003
#define LVS_REPORT        0x0003

// ListView message macros
#define LVM_GETITEMCOUNT  (LVM_FIRST + 4)
#define LVM_INSERTCOLUMN  (LVM_FIRST + 27)
#define LVM_INSERTITEM    (LVM_FIRST + 7)
#define LVM_SETITEMTEXT   (LVM_FIRST + 47)
#define LVM_GETITEMTEXT   (LVM_FIRST + 45)

// TVM messages
#define TVM_INSERTITEM    (TV_FIRST + 1)
#define TVM_SELECTITEM    (TV_FIRST + 11)

// PIM structures
typedef struct {
    int iItem;
    int iSubItem;
    UINT state;
    UINT stateMask;
    LPSTR lpszText;
    int cchTextMax;
    int iImage;
    LPARAM lParam;
} LVITEMA, *PLVITEMA, LVITEM, *PLVITEM;

typedef struct {
    int iColumn;
    int fmt;
    int cx;
    LPSTR lpszText;
    int cchTextMax;
    int iSubItem;
} LVCOLDMA, *PLVCOLDMA;

// Tab control
typedef struct {
    int iTab;
    FARPROC lpfnDlgProc;
    int lParam;
    LPSTR lpszText;
    HINSTANCE hInstance;
    LPSTR lpszHeader;
    HGLOBAL hPage;
    DWORD dwStyle;
} DLGTEMPLATEEX;

#define TCIF_TEXT         0x0001
#define TCIF_IMAGE        0x0002
#define TCIF_RTLREADING   0x0004
#define TCIF_PARAM        0x0008
#define TCIF_STATE        0x0010

typedef struct {
    int cbSize;
    int iItem;
    int iColumn;
    LVITEMA item;
} LVGETINFOW, *PLVGETINFOW, LVGETINFOA, *PLVGETINFOA;

// ListView notification codes
#define LVN_ITEMCHANGED   (-0x100)
#define LVN_GETINFOTEXT   (-0x104)

// Tree view
#define TVIF_TEXT         0x0001
#define TVIF_IMAGE        0x0002
#define TVIF_PARAM        0x0004
#define TVIF_STATE        0x0008

typedef struct {
    UINT cbSize;
    UINT imlNormal;
    UINT imlState;
    UINT imlError;
    UINT imlOverlay;
    int cLines;
} TOOLINFOA, *PTOOLINFOA, LPTOOLINFOA;

// Progress bar
#define PBS_MARQUEE       0x08

// Status bar
#define SBARS_SIZEGRIP    0x0100

// Rebar
#define RBIM_CLASSNAME    0x0001

// Generic control macros
#define LM_SETITEM -1075

#define SendMessageA      SendMessage
#define SendMessageW      SendMessage
#define PostMessageA      PostMessage
#define PostMessageW      PostMessage

#endif // ORBITER_BUILD_SDLGPUCLIENT
