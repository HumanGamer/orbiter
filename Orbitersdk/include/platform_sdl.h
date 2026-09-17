// Copyright (c) Martin Schweiger
// Licensed under the MIT License
// Cross-platform compatibility layer for Win32 types on macOS/Linux
#pragma once

#ifndef PLATFORM_SDL_H
#define PLATFORM_SDL_H

#ifndef __SSE__
#define __SSE__ 0
#endif
#ifndef __ARM_NEON
#define __ARM_NEON 0
#endif
#include <algorithm>
#include <cinttypes>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
typedef uint32_t DWORD;
typedef uint8_t BYTE;
typedef bool BOOL;
typedef void* HANDLE;
typedef void* HWND;
typedef void* HDC;
typedef void* HMODULE;
typedef void* HGDIOBJ;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef char* LPSTR;
typedef const char* LPCSTR;
typedef wchar_t* LPWSTR;
typedef const wchar_t* LPCWSTR;
typedef unsigned short WORD;
typedef long LONG;
typedef long LONG_PTR;
typedef int INT;
typedef uintptr_t UINT_PTR;
typedef uintptr_t DWORD_PTR;
typedef const char* LPCTSTR;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t* LPWSTR;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
#ifndef __int64
#define __int64 long long
#endif
typedef uint64_t UINT64;
typedef UINT_PTR WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;
typedef void* LPDIRECT3D7;
typedef void* LPDIRECT3DDEVICE7;
typedef void* LPDIRECTDRAW7;
typedef void* LPDIRECTINPUTDEVICE8A;
typedef void* LPDIRECTINPUT8A;
typedef void* LPDIRECTINPUT8;
typedef void* LPDIRECTINPUTDEVICE8;
typedef void* HMENU;
typedef void* HACCEL;
typedef void* HINSTANCE;
typedef int UINT;
typedef long HRESULT;
typedef void* HPEN;
typedef void* HBRUSH;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HBITMAP;
typedef void* HFONT;
typedef unsigned long COLORREF;
typedef struct { int cbSize; DWORD fMask; int nMin; int nMax; int nPage; int nPos; int nTrackPos; } SCROLLINFO;
typedef struct { DWORD dwSize; DWORD dwFlags; DWORD ddpfPixelFormat; DWORD dwSurfaceCaps; DWORD dwBackBufferCount; } DDSURFACEDESC2;
typedef void* LPDIRECTDRAWSURFACE7;
typedef struct { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
typedef struct { float x, y, z; } D3DVECTOR;
#ifndef __D3DVERTEX_DEFINED__
typedef struct { float x, y, z, rhw; DWORD color; float tu, tv; float nx, ny, nz; } D3DVERTEX;
#define __D3DVERTEX_DEFINED__
#endif
#ifndef __D3DMATRIX_DEFINED__
typedef struct { float _11, _12, _13, _14; float _21, _22, _23, _24; float _31, _32, _33, _34; float _41, _42, _43, _44; } D3DMATRIX;
#define __D3DMATRIX_DEFINED__
#endif
#ifndef __D3DCOLOR_DEFINED__
typedef struct { float r, g, b, a; } D3DCOLOR;
#define __D3DCOLOR_DEFINED__
#endif
#define D3DRGBA(r,g,b,a) ((D3DCOLOR){(r),(g),(b),(a)})
#define D3DRGB(r,g,b) D3DRGBA((r),(g),(b),1.0f)
#ifndef __D3DCOLORVALUE_DEFINED__
typedef struct { float r, g, b, a; } D3DCOLORVALUE;
#define __D3DCOLORVALUE_DEFINED__
#endif
typedef struct _POINT { LONG x; LONG y; } POINT;
typedef struct _RECT { LONG left; LONG top; LONG right; LONG bottom; } RECT;
typedef struct { HWND hwndFrom; UINT idFrom; UINT code; } NMHDR;
typedef struct { NMHDR hdr; int iDelta; POINT pt; UINT uKeyFlags; DWORD dwItemSpec; DWORD dwItemData; } NMUPDOWN;
typedef RECT* LPRECT;
typedef const RECT* LPCRECT;
typedef struct tagMSG { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; } MSG;
typedef struct _WNDCLASS { UINT style; LRESULT (*lpfnWndProc)(HWND, UINT, WPARAM, LPARAM); int cbClsExtra; int cbWndExtra; HINSTANCE hInstance; HICON hIcon; HCURSOR hCursor; HBRUSH hbrBackground; LPCSTR lpszMenuName; LPCSTR lpszClassName; } WNDCLASS;
#define COLORREF_TO_D3DCOLOR(c) ((D3DCOLOR){((c)&0xff)/255.0f,(((c)>>8)&0xff)/255.0f,(((c)>>16)&0xff)/255.0f,1.0f})
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((WORD)(g)<<8)))|(((DWORD)(BYTE)(b))<<16)))
#define TRUE 1
#define FALSE 0
#define NULL nullptr
#define MAX_PATH 260
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
typedef LRESULT (*DLGPROC)(HWND, UINT, WPARAM, LPARAM);
typedef int INT_PTR;
typedef long LPSIZE;
typedef void* CLSID;
typedef void* IID;
typedef void* LPUNKNOWN;
typedef void* HRSRC;
typedef UINT LANGID;
typedef const char* PCSTR;
typedef char* PSTR;
#define VOID void
#define S_OK 0
#define S_FALSE 1
#define E_FAIL 0x80004005
#define E_HANDLE 0x80070006
#define E_PENDING 0x80070007
#define E_INVALIDARG 0x80070057
#define HRESULT_FAILED(hr) (((unsigned)(hr)) >> 31)
static inline HRESULT FAILED(HRESULT hr) { return HRESULT_FAILED(hr); }
#define MAKEWPARAM(l,h) ((WPARAM)(((WORD)(l))|(((uintptr_t)(WORD)(h)))<<16))
#define MAKELANGID(p,s) ((((WORD)(s))<<10)|(WORD)(p))
#define MAKEINTRESOURCEA(i) ((LPCSTR)((uintptr_t)((WORD)(i))))
#define TA_CENTER 0x0001
#define TA_RIGHT 0x0002
#define L(x) x
#define L(x) x
#define LR_CREATEDIBSECTION 0x00000200
#define LR_LOADFROMFILE 0x00000010
#define FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200
#define DD_OK 0
#define DDERR_WRONGDEVICE 0x8876002B
#define DDERR_WASSTILLDRAWING 0x8876002C
#define DDERR_OUTOFVIDEOMEMORY 0x8876002D
#define DDERR_OVERLAYCANTCLIP 0x8876002E
#define DDERR_OVERLAYCOLORKEYONLYONEACTIVE 0x8876002F
#define DDERR_PALETTEBUSY 0x88760030
#define DDERR_COLORKEYNOTSET 0x88760031
#define DDERR_SURFACEALREADYATTACHED 0x88760032
#define DDERR_SURFACEALREADYDEPENDENT 0x88760033
#define DDERR_SURFACEBUSY 0x88760034
#define DDERR_CANTLOCKSURFACE 0x88760035
#define DDERR_SURFACEISOBSCURED 0x88760036
#define DDERR_SURFACELOST 0x88760037
#define DDERR_SURFACENOTATTACHED 0x88760038
#define DDERR_TOOBIGHEIGHT 0x88760039
#define DDERR_TOOBIGSIZE 0x8876003A
#define DDERR_TOOBIGWIDTH 0x8876003B
#define DDERR_UNSUPPORTED 0x80000001
#define DDERR_UNSUPPORTEDFORMAT 0x8876003C
#define DDERR_UNSUPPORTEDMASK 0x8876003D
#define DDERR_INVALIDSTREAM 0x8876003E
#define DDERR_VERTICALBLANKINPROGRESS 0x8876003F
#define DDERR_DDSCAPSCOMPLEXREQUIRED 0x88760040
#define DDERR_XALIGN 0x88760041
#define DDERR_INVALIDDIRECTDRAWGUID 0x88760042
#define DDERR_DIRECTDRAWALREADYCREATED 0x88760043
#define DDERR_NODIRECTDRAWHW 0x88760044
#define DDERR_PRIMARYSURFACEALREADYEXISTS 0x88760045
#define DDERR_NOEMULATION 0x88760046
#define DDERR_REGIONTOOSMALL 0x88760047
#define DDERR_CLIPPERISUSINGHWND 0x88760048
#define DDERR_NOCLIPPERATTACHED 0x88760049
#define DDERR_NOHWND 0x8876004A
#define DDERR_HWNDSUBCLASSED 0x8876004B
#define DDERR_HWNDALREADYSET 0x8876004C
#define DDERR_NOPALETTEATTACHED 0x8876004D
#define DDERR_NOPALETTEHW 0x8876004E
#define DDERR_BLTFASTCANTCLIP 0x8876004F
#define DDERR_NOBLTHW 0x88760050
#define DDERR_NODDROPSHW 0x88760051
#define DDERR_REACHOUTDISTANCE 0x88760052
#define DDERR_OUTOFTEXTURE 0x88760053
#define DDERR_OUTOFVIDEOMEMORY2 0x88760054
#define DDERR_BADMODE 0x88760055
#define DDERR_DDSCAPSHWREQ 0x88760056
#define DDERR_OVERLAYNOTVISIBLE 0x88760057
#define DDERR_NOOVERLAYDEST 0x88760058
#define DDERR_INVALIDPOSITION 0x88760059
#define DDERR_NOTAOVERLAYSURFACE 0x8876005A
#define DDERR_EXCLUSIVEMODEALREADYSET 0x8876005B
#define DDERR_NOTFLIPPABLE 0x8876005C
#define DDERR_CANTDUPLICATE 0x8876005D
#define DDERR_NOTLOCKED 0x8876005E
#define DDERR_CANTCREATEDC 0x8876005F
#define DDERR_NODC 0x88760060
#define DDERR_WRONGMODE 0x88760061
#define DDERR_IMPLICITLYCREATED 0x88760062
#define DDERR_NOTPALETTIZED 0x88760063
#define DDERR_UNSUPPORTEDMODE 0x88760064
#define DDERR_NOMIPMAPHW 0x88760065
#define DDERR_INVALIDSURFACETYPE 0x88760066
#define DDERR_NOOPTIMIZEHW 0x88760067
#define DDERR_NOTLOADED 0x88760068
#define DDERR_NOFOCUSWINDOW 0x88760069
#define DDERR_NOTONMIPMAPSUBLEVEL 0x88760071
#define DDERR_DCALREADYCREATED 0x88760072
#define DDERR_NONONLOCALVIDMEM 0x88760073
#define DDERR_CANTPAGELOCK 0x88760074
#define DDERR_CANTPAGEUNLOCK 0x88760075
#define DDERR_NOTPAGELOCKED 0x88760076
#define DDERR_EXPIRED 0x88760077
#define DDERR_TESTFINISHED 0x88760078
#define DDERR_NEWMODE 0x88760079
#define DDERR_D3DNOTINITIALIZED 0x8876007A
#define DDERR_VIDEONOTACTIVE 0x8876007B
#define DDERR_NOMONITORINFORMATION 0x8876007C
#define DDERR_NODRIVERSUPPORT 0x8876007D
#define DDERR_DEVICEDOESNTOWNSURFACE 0x8876007E
#define DDERR_NOTINITIALIZED 0x8876007F
#define DIERR_INVALIDPARAM 0x80070057
#define DIERR_NOTINITIALIZED 0x80070001
#define DIERR_OBJECTNOTFOUND 0x80070002
#define DIERR_REPORTFULL 0x80070003
#define DIERR_UNPLUGGED 0x80070004
#define DIERR_UNSUPPORTED 0x80070005
#define PROCESS_QUERY_INFORMATION 0x0400
#define PROCESS_VM_READ 0x0010
#define Z_OK 0
#define __declspec(x) __attribute__((x))
#define _ASSERTE(x) ((x) ? (void)0 : abort())
#define STRSAFE_E_INVALID_LENGTH 0x800700CF
static inline DWORD GetProcessId(HANDLE hProcess) { return 0; }
static inline BOOL EnumProcessModules(HANDLE hProcess, HMODULE* hMods, DWORD cb, DWORD* cbNeeded) { if(cbNeeded) *cbNeeded = 0; return FALSE; }
static inline DWORD GetModuleFileNameExA(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize) { if(lpFilename) *lpFilename = 0; return 0; }
typedef struct { LPVOID lpBaseOfDll; DWORD SizeOfImage; DWORD EntryPoint; } MODULEINFO;
static inline DWORD GetFileVersionInfoSizeA(const char* lpFileName, DWORD* lplpBuffer) { return 0; }
static inline BOOL GetFileVersionInfoA(const char* lpFileName, DWORD dwHandle, DWORD dwLen, LPVOID lpData) { return FALSE; }
typedef struct { DWORD dwSignature; DWORD dwStrucVersion; WORD wMS; WORD wLS; WORD wReserved0; WORD wReserved1; DWORD dwSignature2; DWORD dwDateLo; DWORD dwDateHi; } VS_FIXEDFILEINFO;
static inline BOOL VerQueryValueA(const LPVOID pBlock, LPCSTR lpSubBlock, LPVOID* lplpBuffer, UINT* puLen) { return FALSE; }
static inline HRESULT SHGetFolderPathA(HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPSTR pszPath) { (void)hwndOwner; (void)nFolder; (void)hToken; (void)dwFlags; (void)pszPath; return S_OK; }
static inline DWORD GetTickCount() { return 0; }
static inline HRESULT CoCreateInstance(const void* rclsid, LPVOID pUnkOuter, DWORD dwClsContext, const void* riid, LPVOID* ppv) { (void)rclsid; (void)pUnkOuter; (void)dwClsContext; (void)riid; (void)ppv; return 0; }
#define CLSID_WICImagingFactory 0
#define CLSCTX_INPROC_SERVER 1
#define IID_PPV_ARGS(ppv) IID, (void**)ppv
static inline int StringCbCopyA(char* pszDest, size_t cbDest, const char* pszSrc) { size_t len = strlen(pszSrc); if(len >= cbDest) return STRSAFE_E_INVALID_LENGTH; memcpy(pszDest, pszSrc, len + 1); return 0; }
static inline void LocalFree(HANDLE hMem) { (void)hMem; }
static inline int fopen_s(FILE** pFile, const char* filename, const char* mode) { *pFile = fopen(filename, mode); return *pFile ? 0 : 1; }
static inline HMODULE GetModuleHandle(const char* lpModuleName) { return nullptr; }
static inline HINSTANCE GetModuleHandleA(const char* lpModuleName) { return (HINSTANCE)GetModuleHandle(lpModuleName); }
static inline int GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize) { return 0; }
static inline BOOL GetUserNameA(LPSTR lpBuffer, DWORD* nSize) { return FALSE; }
static inline BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) { return TRUE; }
static inline BOOL ShowWindow(HWND hWnd, int nCmdShow) { return TRUE; }
static inline BOOL DestroyWindow(HWND hWnd) { return TRUE; }
static inline BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint) { return TRUE; }
static inline HWND CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) { return nullptr; }
static inline HMENU CreateMenu() { return nullptr; }
static inline BOOL AppendMenuA(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, LPCSTR lpNewItem) { return TRUE; }
static inline BOOL SetMenu(HWND hWnd, HMENU hMenu) { return TRUE; }
static inline LRESULT DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0; }
static inline BOOL RegisterClassA(const WNDCLASS* lpWndClass) { return TRUE; }
static inline BOOL UnregisterClassA(LPCSTR lpClassName, HINSTANCE hInstance) { return TRUE; }
static inline LRESULT DispatchMessageA(const MSG* lpMsg) { return 0; }
static inline BOOL GetMessageA(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) { return TRUE; }
static inline BOOL TranslateMessageA(const MSG* lpMsg) { return TRUE; }
static inline BOOL PeekMessageA(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) { return FALSE; }
static inline int TranslateAcceleratorA(HWND hWnd, HACCEL hAccTable, MSG* lpMsg) { return 0; }
static inline HWND GetDlgItem(HWND hDlg, int nIDDlgItem) { return nullptr; }
static inline int GetDlgCtrlID(HWND hwnd) { return 0; }
static inline BOOL SetScrollInfo(HWND hwnd, int fnBar, SCROLLINFO* lpsi, BOOL bRedraw) { return TRUE; }
static inline BOOL GetScrollInfo(HWND hwnd, int fnBar, SCROLLINFO* lpsi) { return TRUE; }
static inline int ScrollWindow(HWND hWnd, int XAmount, int YAmount, const RECT* lpRect, const RECT* lpClipRect) { return 0; }
static inline int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) { return 0; }
static inline HINSTANCE ShellExecuteA(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd) { return nullptr; }
static inline DWORD GetVersion() { return 0; }
static inline WORD HIWORD(DWORD dw) { return (WORD)((dw >> 16) & 0xffff); }
static inline WORD LOWORD(DWORD dw) { return (WORD)(dw & 0xffff); }
static inline DWORD FormatMessageA(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, UINT dwLanguageId, LPSTR lpBuffer, DWORD nSize, va_list* Arguments) { (void)dwFlags; (void)lpSource; (void)dwMessageId; (void)dwLanguageId; (void)lpBuffer; (void)nSize; (void)Arguments; return 0; }
static inline HRESULT IDirectDraw7_SetCooperativeLevel(LPDIRECTDRAW7 pDD, HWND hWnd, DWORD dwFlags) { return 0; }
static inline HRESULT IDirectDraw7_CreateSurface(LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pDDSDESC, LPDIRECTDRAWSURFACE7* ppDDS, LPVOID pUnkOuter) { return 0; }
static inline BOOL CreateWindowA(LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) { return FALSE; }
#define MB_OK 0x00000000L
#define MB_YESNO 0x00000004L
#define IDYES 6
#define IDNO 7
#define SE_ERR_FNF 2
#define SWP_NOSIZE 0x0001
#define SWP_NOMOVE 0x0002
#define SWP_NOZORDER 0x0004
#define SWP_NOACTIVATE 0x0010
#define SWP_SHOWWINDOW 0x0040
#define SW_SHOWDEFAULT 10
#define WS_OVERLAPPEDWINDOW 0x00000000L
#define WS_POPUP 0x80000000L
#define WS_VISIBLE 0x10000000L
#define WS_CHILD 0x40000000L
#define MF_STRING 0x00000000L
#define MF_POPUP 0x00000010L
#define PM_REMOVE 0x0001
#define PM_NOYIELD 0x0002
#define DDCKEY_SRCBLT 0x00000001
#define SIF_ALL 0x000F
#define SIF_RANGE 0x0001
#define SIF_PAGE 0x0002
#define SIF_POS 0x0004
#define SIF_DISABLENOSCROLL 0x0008
#define SB_HORZ 0
#define SB_VERT 1
#define SB_CTL 2
#define SB_BOTH 3
#define SB_LINELEFT 0
#define SB_LINERIGHT 1
#define SB_PAGELEFT 2
#define SB_PAGERIGHT 3
#define SB_THUMBPOSITION 4
#define SB_THUMBTRACK 5
#define SB_TOP 6
#define SB_BOTTOM 7
#define SB_ENDSCROLL 8
#define EN_CHANGE 0x0303
#define UDN_DELTAPOS 0xFFFFFF9B
#define CW_USEDEFAULT 0x80000000L
#define CS_HREDRAW 0x0002
#define CS_VREDRAW 0x0001
#define IDC_ARROW MAKEINTRESOURCEA(32512)
#define IDC_IBEAM MAKEINTRESOURCEA(32513)
#define IDC_WAIT MAKEINTRESOURCEA(32514)
#define IDC_CROSS MAKEINTRESOURCEA(32515)
#define IDC_SIZENWSE MAKEINTRESOURCEA(32642)
#define IDC_SIZENESW MAKEINTRESOURCEA(32643)
#define IDC_SIZEWE MAKEINTRESOURCEA(32644)
#define IDC_SIZENS MAKEINTRESOURCEA(32645)
#define IDC_SIZEALL MAKEINTRESOURCEA(32646)
#define IDC_NO MAKEINTRESOURCEA(32648)
#define IDC_HAND MAKEINTRESOURCEA(32649)
#define IDC_APPSTARTING MAKEINTRESOURCEA(32650)
#define IDI_APPLICATION MAKEINTRESOURCEA(32512)
#define IDI_ERROR MAKEINTRESOURCEA(32513)
#define IDI_HAND MAKEINTRESOURCEA(32513)
#define IDI_QUESTION MAKEINTRESOURCEA(32514)
#define IDI_WARNING MAKEINTRESOURCEA(32515)
#define IDI_INFORMATION MAKEINTRESOURCEA(32516)
#define IDI_WINLOGO MAKEINTRESOURCEA(32517)
#define IDI_SHIELD MAKEINTRESOURCEA(32518)
#define IDI_ASTERISK MAKEINTRESOURCEA(32516)
#define IDI_EXCLAMATION MAKEINTRESOURCEA(32515)
#define WS_EX_CLIENTEDGE 0x00000200L
#define WS_EX_WINDOWEDGE 0x00000100L
#define WS_EX_STATICEDGE 0x00020000L
#define WS_EX_OVERLAPPEDWINDOW 0x00000300L
#define WS_EX_TOOLWINDOW 0x00000080L
#define WS_EX_APPWINDOW 0x00040000L
#define WS_EX_TOPMOST 0x00000008L
#define WS_EX_DLGMODALFRAME 0x00000001L
#define WS_EX_NOPARENTNOTIFY 0x00000004L
#define WS_EX_TRANSPARENT 0x00000020L
#define WS_EX_MDICHILD 0x00000040L
#define WS_EX_ACCEPTFILES 0x00000010L
#define WS_EX_CONTEXTHELP 0x00000400L
#define WS_EX_RIGHT 0x00001000L
#define WS_EX_LEFT 0x00000000L
#define WS_EX_RTLREADING 0x00002000L
#define WS_EX_LTRREADING 0x00000000L
#define WS_EX_LEFTSCROLLBAR 0x00004000L
#define WS_EX_RIGHTSCROLLBAR 0x00000000L
#define WS_EX_CONTROLPARENT 0x00010000L
#define WS_EX_COMPOSITED 0x02000000L
#define WS_EX_LAYERED 0x00080000L
#define WS_EX_NOINHERITLAYOUT 0x00100000L
#define WS_EX_NOREDIRECTIONBITMAP 0x00200000L
#define WS_EX_LAYOUTRTL 0x00400000L
#define WS_EX_NOACTIVATE 0x08000000L
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_MBUTTONDOWN 0x0207
#define WM_MBUTTONUP 0x0208
#define WM_MOUSEMOVE 0x0200
#define WM_SIZE 0x0005
#define WM_CLOSE 0x0010
#define WM_DESTROY 0x0002
#define WM_COMMAND 0x0111
#define WM_HSCROLL 0x0114
#define WM_VSCROLL 0x0115
#define WM_PAINT 0x000F
#define WM_SETFOCUS 0x0007
#define WM_KILLFOCUS 0x0008
#define WM_ENABLE 0x000A
#define WM_SETREDRAW 0x000B
#define WM_SETCURSOR 0x0020
#define WM_LBUTTONDBLCLK 0x0203
#define WM_RBUTTONDBLCLK 0x0206
#define WM_MBUTTONDBLCLK 0x0209
#define WM_XBUTTONDOWN 0x020B
#define WM_XBUTTONUP 0x020C
#define WM_MOUSEWHEEL 0x020A
#define WM_NCHITTEST 0x0084
#define WM_NCCREATE 0x0081
#define WM_NCDESTROY 0x0082
#define WM_NCCALCSIZE 0x0083
#define WM_NCPAINT 0x0085
#define WM_NCMOUSEMOVE 0x00A0
#define WM_ENTERSIZEMOVE 0x0231
#define WM_EXITSIZEMOVE 0x0232
#define WM_GETMINMAXINFO 0x0024
#define WM_WINDOWPOSCHANGING 0x0046
#define WM_WINDOWPOSCHANGED 0x0047
#define WM_CONTEXTMENU 0x007B
#define WM_INITMENUPOPUP 0x0117
#define WM_MENUCHAR 0x0120
#define WM_MEASUREITEM 0x0200
#define WM_DRAWITEM 0x02B0
#define WM_DELETEITEM 0x027E
#define WM_VKEYTOITEM 0x022E
#define WM_CHARTOITEM 0x022F
#define WM_SETFONT 0x0030
#define WM_GETFONT 0x0031
#define WM_SETHOTKEY 0x0032
#define WM_GETHOTKEY 0x0033
#define WM_QUERYDRAGICON 0x0037
#define WM_COMPAREITEM 0x0039
#define WM_GETTEXT 0x000D
#define WM_GETTEXTLENGTH 0x000E
#define WM_USER 0x0400
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1B
#define VK_SPACE 0x20
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_END 0x23
#define VK_HOME 0x24
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_INSERT 0x2D
#define VK_DELETE 0x2E
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A
#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75
#define VK_F7 0x76
#define VK_F8 0x77
#define VK_F9 0x78
#define VK_F10 0x79
#define VK_F11 0x7A
#define VK_F12 0x7B
#define VK_F13 0x7C
#define VK_F14 0x7D
#define VK_F15 0x7E
#define VK_F16 0x7F
#define VK_F17 0x80
#define VK_F18 0x81
#define VK_F19 0x82
#define VK_F20 0x83
#define VK_F21 0x84
#define VK_F22 0x85
#define VK_F23 0x86
#define VK_F24 0x87
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_LSHIFT 0xA0
#define VK_RSHIFT 0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_LMENU 0xA4
#define VK_RMENU 0xA5
#define WM_SYSKEYDOWN 0x0104
#define WM_SYSKEYUP 0x0105
#define WM_SYSCHAR 0x0106
#define WM_SYSCOMMAND 0x0112
#define SC_CLOSE 0xF060
#define SC_MINIMIZE 0xF020
#define SC_MAXIMIZE 0xF030
#define SC_RESTORE 0xF120
#define SC_MOVE 0xF010
#define SC_SIZE 0xF000
#define SC_MOUSEMOVE 0xF012
#define SC_SEPARATOR 0xF00E
#if defined(__x86_64__) || defined(__i386__)
#include <emmintrin.h>
#endif

// DirectInput types
typedef struct { LONG lX; LONG lY; LONG lZ; LONG lRx; LONG lRy; LONG lRz; LONG rglSlider[2]; DWORD rgdwButton[32]; DWORD rgbButtons[32]; LONG lVX; LONG lVY; LONG lVZ; LONG lVRx; LONG lVRy; LONG lVRz; LONG lVFRx; LONG lVFRy; LONG lVFRz; LONG lVFRa; LONG lVFRb; LONG lVFRc; LONG lAS; LONG lARS; DWORD rglAxis[16]; DWORD rgdwPOV[16]; DWORD rglVAxis[16]; DWORD rgVAS; DWORD rgdwVPOV[16]; DWORD rglVPOV[16]; } DIJOYSTATE2;
typedef struct { DWORD dwSize; DWORD dwFlags; DWORD dwDevType; DWORD dwAxes; DWORD dwButtons; DWORD dwPOVs; DWORD dwFFSamplePeriod; DWORD dwFsUpdates; DWORD dwTotalInputs; DWORD dwSID; DWORD dwVendorID; DWORD dwVersion; } DIDATAFORMAT;
typedef struct { DWORD dwSize; DWORD dwFlags; DWORD dwDevType; DWORD dwAxes; DWORD dwButtons; DWORD dwPOVs; DWORD dwFFSamplePeriod; DWORD dwFsUpdates; DWORD dwTotalInputs; DWORD dwSID; DWORD dwVendorID; DWORD dwVersion; } DIDEVICEINSTANCE;

// SDL GameController renames (SDL3 API changes)
#define SDL_GetNumGameControllers() SDL_GetNumGamepads()
#define SDL_GameControllerOpen SDL_OpenGamepad
#define SDL_GameControllerClose SDL_CloseGamepad
#define SDL_GameControllerGetAxis SDL_GamepadGetAxis
#define SDL_GameControllerGetButton SDL_GamepadGetButton
#define SDL_GameControllerName SDL_GamepadName
#define SDL_GameControllerMappingForIndex SDL_GamepadMappingForIndex
#define SDL_GameControllerFromInstanceID SDL_GamepadFromInstanceID
#define SDL_GameControllerEventState SDL_GamepadEventState
#define SDL_IsGameController SDL_IsGamepad

#define SDL_GetGameControllerInstanceID SDL_GetGamepadInstanceID
#define SDL_GameControllerGetDeviceInstanceID SDL_GetGamepadDeviceInstanceID

#include <SDL3/SDL.h>

typedef SDL_Gamepad* SDL_GameController;
typedef void* sdl_joystick_handle;
bool SDL_joy_init = false;
int SDL_GetNumGamepads() { return 0; }
typedef void* sdl_window_handle;
typedef void* sdl_joystick_handle;
typedef float D3DVALUE;

// Windows API types
typedef float FLOAT;

typedef unsigned long long DWORDLONG;
typedef struct { DWORD dwOfs; DWORD dwTimeStamp; DWORD dwData; DWORD dwFlags; } DIDEVICEOBJECTDATA;

#define WINAPI __attribute__((cdecl))
typedef int (WINAPI *FARPROC_T)(void);
#define CALLBACK __attribute__((stdcall))
#define LPNMHDR const struct tagNMHDR*
typedef short POINTS;

typedef unsigned char UINT8;
typedef void* LPMSG;
typedef void* HTREEITEM;
typedef long long LONGLONG;
typedef size_t SIZE_T;
typedef void* FARPROC;
typedef void* LPSECURITY_ATTRIBUTES;
typedef DWORD (*LPTHREAD_START_ROUTINE)(LPVOID);
typedef DWORD* LPDWORD;
#define ATTACH_PARENT_PROCESS 0xFFFFFFFF



// HTMLHelp functions
HWND WINAPI HtmlHelpA(HWND hwndCaller, LPCSTR pszFile, UINT uCommand, DWORD_PTR dwData);

// HTMLHelp macros
#define HH_DISPLAY_TOPIC 0x0000



// Win32 API stubs
BOOL GetCursorPos(POINT* p) { *p = {0,0}; return TRUE; }
BOOL ScreenToClient(HWND, POINT*) { return TRUE; }
BOOL SetCursorPos(int, int) { return TRUE; }
HWND GetDesktopWindow() { return 0; }
void AllocConsole() {}
void AttachConsole(DWORD) {}
void sprintf_s(char* d, size_t, const char* f, ...) {}
HWND GetConsoleWindow() { return 0; }
void SetConsoleTitle(const char*) {}
void GetSystemMenu(HWND, BOOL) {}
#define MF_BYCOMMAND 0x00000000L
#define STD_OUTPUT_HANDLE ((HANDLE)-11)
#define WAIT_TIMEOUT 258L
DWORD WaitForSingleObject(HANDLE, DWORD) { return 0; }
void CloseHandle(HANDLE) {}
void ReleaseMutex(HANDLE) {}
#define STD_INPUT_HANDLE ((HANDLE)-10)
#define STD_ERROR_HANDLE ((HANDLE)-12)
SIZE_T lstrlenA(LPCSTR s) { return strlen(s); }
#define lstrlen lstrlenA
void strcpy_s(char* d, size_t, const char* s) { strcpy(d,s); }
#define D3DVAL(x) ((float)(x))
FARPROC GetProcAddress(HMODULE, const char*) { return 0; }
HANDLE GetStdHandle(DWORD) { return 0; }
BOOL DeleteMenu(HMENU, UINT, UINT) { return TRUE; }
HANDLE CreateThread(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD) { return 0; }
BOOL SetConsoleTextAttribute(HANDLE, WORD) { return TRUE; }
#define MAKEINTRESOURCE(x) ((LPSTR)((ULONG_PTR)((WORD)(x))))
#define ENABLE_LINE_INPUT 0x0002
#define ENABLE_ECHO_INPUT 0x0004
#define ENABLE_PROCESSED_INPUT 0x0001
#define FOREGROUND_RED 0x0004
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_INTENSITY 0x0008
#endif // PLATFORM_SDL_H
