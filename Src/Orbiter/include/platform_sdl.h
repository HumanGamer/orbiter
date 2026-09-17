// Copyright (c) Martin Schweiger
// Licensed under the MIT License
// Cross-platform compatibility layer for Win32 types on macOS/Linux
#pragma once

// Fix zlib compatibility issues on macOS
#ifdef __APPLE__
#undef fdopen
#undef OS_CODE
#endif

#ifndef PLATFORM_SDL_H
#define PLATFORM_SDL_H

// Win32 API macros - defined early to ensure visibility
#define CreateDialog CreateDialogA
#define SetCurrentDirectory SetCurrentDirectoryA
#define MessageBox MessageBoxA
#define LoadLibrary LoadLibraryA
#define LoadCursor LoadCursorA
#define LoadIcon LoadIconA
#define RegisterClass RegisterClassA
#define GetClassInfo GetClassInfoA
#define SetWindowText SetWindowTextA
#define PeekMessage PeekMessageA
#define GetMessage GetMessageA

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
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
typedef uint32_t DWORD;
typedef uint8_t BYTE;
typedef bool BOOL;
typedef void* HANDLE;
typedef void* HWND;
typedef void* HDC;
typedef void* HMODULE;
typedef void* HGDIOBJ;
typedef void* LPVOID;
typedef void* PVOID;
typedef void* HKEY;
typedef const void* LPCVOID;
typedef char* LPSTR;
typedef const char* LPCSTR;
typedef char* LPTSTR;
typedef wchar_t WCHAR;
typedef wchar_t* LPWSTR;
typedef const wchar_t* LPCWSTR;
typedef unsigned short WORD;
typedef WORD ATOM;
typedef long LONG;
typedef long LONG_PTR;
typedef int INT;
typedef uintptr_t UINT_PTR;
typedef uintptr_t DWORD_PTR;
typedef uintptr_t ULONG_PTR;
typedef unsigned long ULONG;
typedef int64_t LONG64;
typedef int64_t LARGE_INTEGER;
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
typedef int INT_PTR;
typedef UINT_PTR WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;
typedef void* LPDIRECT3D7;
typedef void* LPDIRECTDRAW7;
typedef void* LPDIRECTINPUTDEVICE8A;
typedef void* LPDIRECTINPUT8A;
typedef void* LPDIRECTINPUT8;
typedef void* HMENU;
typedef void* HACCEL;
typedef void* HINSTANCE;
typedef int UINT;
typedef long HRESULT;
typedef void* HPEN;
typedef void* HBRUSH;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HRGN;
typedef void* HBITMAP;
typedef void* HFONT;
typedef unsigned long COLORREF;
typedef struct { int cbSize; DWORD fMask; int nMin; int nMax; int nPage; int nPos; int nTrackPos; } SCROLLINFO;
typedef struct { DWORD dwSize; DWORD dwFlags; DWORD ddpfPixelFormat; DWORD dwSurfaceCaps; DWORD dwBackBufferCount; } DDSURFACEDESC2;
typedef void* LPDIRECTDRAWSURFACE7;
static inline HRESULT LPDIRECTDRAWSURFACE7_SetColorKey(LPDIRECTDRAWSURFACE7 surf, DWORD flags, void* key) { (void)surf; (void)flags; (void)key; return (HRESULT)0x80000000L; }
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
typedef POINT* LPPOINT;
typedef struct _RECT { LONG left; LONG top; LONG right; LONG bottom; } RECT;
typedef struct { UINT CtlType; UINT CtlID; UINT itemID; UINT itemAction; UINT itemState; HWND hwndItem; HDC hDC; RECT rcItem; LPARAM itemData; } DRAWITEMSTRUCT;
typedef DRAWITEMSTRUCT* LPDRAWITEMSTRUCT;
typedef struct { HWND hwndFrom; UINT idFrom; UINT code; } NMHDR, *LPNMHDR, *LPCNMHDR;
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
#define _strdup strdup
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define stricmp strcasecmp
#define _tcsicmp strcasecmp
#define _MAX_PATH 260

// _snprintf -> vsnprintf
#define _snprintf snprintf
#define _snprintf_s(str, size, max, ...) snprintf(str, size, __VA_ARGS__)
#define _vsnprintf vsnprintf

// _putenv -> putenv
#define _putenv putenv

// _getcwd -> getcwd
#define _getcwd getcwd

// TCHAR compatibility
typedef char TCHAR;
#define _T(x) x
#define TEXT(x) x

// MB_ICONERROR
#define MB_ICONERROR 0x00000030L

// _exception struct (for _matherr)
typedef struct {
    int except_code;
    const char* name;
    double arg1;
    double arg2;
    double arg3;
    double retval;
} _exception;

typedef struct { DWORD cb; DWORD Generation; DWORD PageFaultCount; size_t WorkingSetSize; size_t QuotaPeakPagedPoolUsage; size_t QuotaPagedPoolUsage; size_t QuotaPeakNonPagedPoolUsage; size_t QuotaNonPagedPoolUsage; size_t PagefileUsage; size_t PeakPagefileUsage; size_t PeakWorkingSetSize; } PROCESS_MEMORY_COUNTERS;
typedef PROCESS_MEMORY_COUNTERS* PPROCESS_MEMORY_COUNTERS;
typedef void* HANDLE;
typedef INT_PTR (*DLGPROC)(HWND, UINT, WPARAM, LPARAM);
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
#define DDERR_ALREADYLOCKED 0x88760080
#define DDERR_DEPTHMASKCLIPPED 0x88760081
#define DDERR_Locked 0x88760082
#define DDERR_NOFOCUS 0x88760083
#define DDERR_ALREADYACQUIRED 0x88760084
#define DDERR_NOTACQUIRED 0x88760085
#define DDERR_INVALIDDIRECTDRAWGUID 0x88760086
#define DDERR_PRIMARYSURFACEALREADYEXISTS 0x88760087
#define DDERR_NOEMULATION 0x88760088
#define DDERR_REGIONTOOSMALL 0x88760089
#define DDERR_CLIPPERISUSINGHWND 0x8876008A
#define DDERR_NOCLIPPERATTACHED 0x8876008B
#define DDERR_NOHWND 0x8876008C
#define DDERR_HWNDSUBCLASSED 0x8876008D
#define DDERR_HWNDALREADYSET 0x8876008E
#define DDERR_NOPALETTEATTACHED 0x8876008F
#define DDERR_NOPALETTEHW 0x88760090
#define DDERR_BLTFASTCANTCLIP 0x88760091
#define DDERR_NOBLTHW 0x88760092
#define DDERR_NOOVERLAYDEST 0x88760093
#define DDERR_INVALIDPOSITION 0x88760094
#define DDERR_NOTAOVERLAYSURFACE 0x88760095
#define DDERR_EXCLUSIVEMODEALREADYSET 0x88760096
#define DDERR_NOTFLIPPABLE 0x88760097
#define DDERR_CANTDUPLICATE 0x88760098
#define DDERR_NOTLOCKED 0x88760099
#define DDERR_CANTCREATEDC 0x8876009A
#define DDERR_NODC 0x8876009B
#define DDERR_WRONGMODE 0x8876009C
#define DDERR_IMPLICITLYCREATED 0x8876009D
#define DDERR_NOTPALETTIZED 0x8876009E
#define DDERR_UNSUPPORTEDMODE 0x8876009F
#define DDERR_NOMIPMAPHW 0x887600A0
#define DDERR_INVALIDSURFACETYPE 0x887600A1
#define DDERR_NOOPTIMIZEHW 0x887600A2
#define DDERR_NOTLOADED 0x887600A3
#define DDERR_NOFOCUSWINDOW 0x887600A4
#define DDERR_NOTONMIPMAPSUBLEVEL 0x887600A5
#define DDERR_DCALREADYCREATED 0x887600A6
#define DDERR_NONONLOCALVIDMEM 0x887600A7
#define DDERR_CANTPAGELOCK 0x887600A8
#define DDERR_CANTPAGEUNLOCK 0x887600A9
#define DDERR_NOTPAGELOCKED 0x887600AA
#define DDERR_MOREDATA 0x887600AB
#define DDERR_EXPIRED 0x887600AC
#define DDERR_TESTFINISHED 0x887600AD
#define DDERR_NEWMODE 0x887600AE
#define DDERR_D3DNOTINITIALIZED 0x887600AF
#define DDERR_D3DINVALIDOBJECT 0x887600B0
#define DDERR_NO3D 0x887600B1
#define DDERR_WASSTILLDRAWING 0x887600B2
#define DDERR_DDSCAPSCOMPLEXREQUIRED 0x887600B3
#define DDERR_XALIGN 0x887600B4
#define DDERR_INVALIDDIRECTDRAWGUID 0x887600B5
#define DDERR_DIRECTDRAWALREADYCREATED 0x887600B6
#define DDERR_NODIRECTDRAWHW 0x887600B7
#define DDERR_PRIMARYSURFACEALREADYEXISTS 0x887600B8
#define DDERR_NOEMULATION 0x887600B9
#define DDERR_REGIONTOOSMALL 0x887600BA
#define DDERR_CLIPPERISUSINGHWND 0x887600BB
#define DDERR_NOCLIPPERATTACHED 0x887600BC
#define DDERR_NOHWND 0x887600BD
#define DDERR_HWNDSUBCLASSED 0x887600BE
#define DDERR_HWNDALREADYSET 0x887600BF
#define DDERR_NOPALETTEATTACHED 0x887600C0
#define DDERR_NOPALETTEHW 0x887600C1
#define DDERR_BLTFASTCANTCLIP 0x887600C2
#define DDERR_NOBLTHW 0x887600C3
#define DDERR_NOOVERLAYDEST 0x887600C4
#define DDERR_INVALIDPOSITION 0x887600C5
#define DDERR_NOTAOVERLAYSURFACE 0x887600C6
#define DDERR_EXCLUSIVEMODEALREADYSET 0x887600C7
#define DDERR_NOTFLIPPABLE 0x887600C8
#define DDERR_CANTDUPLICATE 0x887600C9
#define DDERR_NOTLOCKED 0x887600CA
#define DDERR_CANTCREATEDC 0x887600CB
#define DDERR_NODC 0x887600CC
#define DDERR_WRONGMODE 0x887600CD
#define DDERR_IMPLICITLYCREATED 0x887600CE
#define DDERR_NOTPALETTIZED 0x887600CF
#define DDERR_UNSUPPORTEDMODE 0x887600D0
#define DDERR_NOMIPMAPHW 0x887600D1
#define DDERR_INVALIDSURFACETYPE 0x887600D2
#define DDERR_NOOPTIMIZEHW 0x887600D3
#define DDERR_NOTLOADED 0x887600D4
#define DDERR_NOFOCUSWINDOW 0x887600D5
#define DDERR_NOTONMIPMAPSUBLEVEL 0x887600D6
#define DDERR_DCALREADYCREATED 0x887600D7
#define DDERR_NONONLOCALVIDMEM 0x887600D8
#define DDERR_CANTPAGELOCK 0x887600D9
#define DDERR_CANTPAGEUNLOCK 0x887600DA
#define DDERR_NOTPAGELOCKED 0x887600DB
#define DDERR_ALREADYLOCKED 0x887600DC
#define DDERR_DEPTHMASKCLIPPED 0x887600DD
#define DDERR_ALREADYACQUIRED 0x887600DE
#define DDERR_NOTACQUIRED 0x887600DF
#define DDERR_CANSTILLDATACH 0x887600E0
#define DDERR_DEVICEDOESNTOWNSURFACE 0x887600E1
#define DDERR_ALREADYINITIALIZED 0x887600E2
#define DDERR_CANNOTATTACHSURFACE 0x887600E3
#define DDERR_CANNOTDETACHSURFACE 0x887600E4
#define DDERR_CURRENTLYNOTAVAIL 0x887600E5
#define DDERR_EXCEPTION 0x887600E6
#define DDERR_GENERIC 0x80000000L
#define DDERR_HEIGHTALIGN 0x887600E7
#define DDERR_INCOMPATIBLEPRIMARY 0x887600E8
#define DDERR_INVALIDCAPS 0x887600E9
#define DDERR_INVALIDCLIPLIST 0x887600EA
#define DDERR_INVALIDMODE 0x887600EB
#define DDERR_INVALIDOBJECT 0x887600EC
#define DDERR_INVALIDPARAMS 0x80070057
#define DDERR_INVALIDRECT 0x887600ED
#define DDERR_LOCKEDSURFACES 0x887600EE
#define DDERR_NO3D 0x887600EF
#define DDERR_NOALPHAHW 0x887600F0
#define DDERR_NOSTEREOHARDWARE 0x887600F1
#define DDERR_NOSURFACELEFT 0x887600F2
#define DDERR_NOCLIPLIST 0x887600F3
#define DDERR_NOCOLORCONVHW 0x887600F4
#define DDERR_NOCOOPERATIVELEVELSET 0x887600F5
#define DDERR_NOCOLORKEY 0x887600F6
#define DDERR_NOCOLORKEYHW 0x887600F7
#define DDERR_DIRECTDRAWALREADYCREATED 0x887600F8
#define DDERR_NOEMULATION 0x887600F9
#define DDERR_REGIONTOOSMALL 0x887600FA
#define DDERR_TEXTUREUNFILTERABLE 0x887600FB
#define DDERR_D3DNOTINITIALIZED 0x887600FC
#define DDERR_D3DINVALIDOBJECT 0x887600FD
#define DDERR_VIDEONOTACTIVE 0x887600FE
#define DDERR_NOMONITORINFORMATION 0x887600FF
#define DDERR_NODRIVERSUPPORT 0x88760100
#define DDERR_DEVICEDOESNTOWNSURFACE 0x88760101
#define DDERR_UNKNOWN 0x88760102
#define DDERR_NOTFOUND 0x88760103
#define DDERR_THREADIDISMATCH 0x88760104
#define DDERR_TEXTUREADDRESSNOTAVAILABLE 0x88760105
#define DDERR_INVALIDPIXELFORMAT 0x88760106
#define DDERR_NODIRECTDRAWSUPPORT 0x88760107
#define DDERR_NOEXCLUSIVEMODE 0x88760108
#define DDERR_NOFLIPHW 0x88760109
#define DDERR_NOGDI 0x8876010A
#define DDERR_NOMIRRORHW 0x8876010B
#define DDERR_NOOVERLAYHW 0x8876010C
#define DDERR_OVERLAPPINGRECTS 0x8876010D
#define DDERR_NORASTEROPHW 0x8876010E
#define DDERR_NOROTATIONHW 0x8876010F
#define DDERR_NOSTRETCHHW 0x88760110
#define DDERR_NOT4BITCOLOR 0x88760111
#define DDERR_NOT4BITCOLORINDEX 0x88760112
#define DDERR_NOT8BITCOLOR 0x88760113
#define DDERR_NOTEXTUREHW 0x88760114
#define DDERR_NOVSYNCHW 0x88760115
#define DDERR_NOZBUFFERHW 0x88760116
#define DDERR_NOZOVERLAYHW 0x88760117
#define DDERR_NOT3D 0x88760118
#define DDERR_NOSOUNDDRV 0x88760119
#define DDERR_OUTOFCAPS 0x8876011A
#define DDERR_OUTOFMEMORY 0x8007000E
#define DDERR_ACCESSDENIED 0x80070005
#define DDERR_ACCESSDENIEDBUFFER 0x8876011B
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
#define FAR
#define PASCAL
#define STDMETHODCALLTYPE
#define _ASSERTE(x) ((x) ? (void)0 : abort())
#define STRSAFE_E_INVALID_LENGTH 0x800700CF
static inline DWORD GetProcessId(HANDLE hProcess) { return 0; }
static inline BOOL EnumProcessModules(HANDLE hProcess, HMODULE* hMods, DWORD cb, DWORD* cbNeeded) { if(cbNeeded) *cbNeeded = 0; return FALSE; }
static inline DWORD GetModuleFileNameExA(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize) { if(lpFilename) *lpFilename = 0; return 0; }
typedef struct { LPVOID lpBaseOfDll; DWORD SizeOfImage; DWORD EntryPoint; } MODULEINFO;
static inline DWORD GetFileVersionInfoSizeA(const char* lpFileName, DWORD* lplpBuffer) { return 0; }
static inline BOOL GetFileVersionInfoA(const char* lpFileName, DWORD dwHandle, DWORD dwLen, LPVOID lpData) { return FALSE; }
typedef struct { DWORD dwSignature; DWORD dwStrucVersion; DWORD dwFileVersionMS; DWORD dwFileVersionLS; DWORD dwProductVersionMS; DWORD dwProductVersionLS; DWORD dwFileFlagsMask; DWORD dwFileFlags; DWORD dwFileOS; DWORD dwFileType; DWORD dwFileSubtype; DWORD dwFileDateMS; DWORD dwFileDateLS; } VS_FIXEDFILEINFO;
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
#define GetModuleFileName GetModuleFileNameA
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
#define DispatchMessage DispatchMessageA
static inline BOOL TranslateMessageA(const MSG* lpMsg) { return TRUE; }
#define TranslateMessage TranslateMessageA
static inline int TranslateAcceleratorA(HWND hWnd, HACCEL hAccTable, MSG* lpMsg) { return 0; }
static inline HWND GetParent(HWND hWnd) { return nullptr; }
static inline BOOL IsChild(HWND hWndParent, HWND hWnd) { return FALSE; }
#define WM_NCLBUTTONDBLCLK 0x00A3
static inline HANDLE CreateEventA(void* lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName) { return (HANDLE)1; }
static inline HANDLE CreateEventW(void* lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName) { return (HANDLE)1; }
#define CreateEvent CreateEventA
static inline BOOL PostThreadMessageA(DWORD dwThreadId, UINT Msg, WPARAM wParam, LPARAM lParam) { return FALSE; }
#define PostThreadMessage PostThreadMessageA
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
#define SWP_NOOWNERZORDER 0x0200
#define SWP_NOCOPYBITS 0x0100
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
#define MK_LBUTTON 0x01
#define MK_RBUTTON 0x02
#define MK_MBUTTON 0x10
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
#define WM_INITDIALOG 0x0110
#define WM_TIMER 0x0113
#define IDOK 1
#define IDCANCEL 2
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
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PLUS 0xBB
#define VK_OEM_4 0xDB
#define VK_OEM_6 0xDD
#define VK_OEM_PIPE 0xDC
#define VK_OEM_1 0xBA
#define VK_OEM_7 0xDE
#define VK_OEM_3 0xC0
#define VK_OEM_COMMA 0xBC
#define VK_DECIMAL 0x6E
#define VK_DIVIDE 0xF6
#define VK_NUMPAD0 0x60
#define VK_NUMPAD1 0x61
#define VK_NUMPAD2 0x62
#define VK_NUMPAD3 0x63
#define VK_NUMPAD4 0x64
#define VK_NUMPAD5 0x65
#define VK_NUMPAD6 0x66
#define VK_NUMPAD7 0x67
#define VK_NUMPAD8 0x68
#define VK_NUMPAD9 0x69
#define VK_OEM_PERIOD 0xBE
#define VK_DELETE 0x2E
#define VK_SNAPSHOT 0x2C
#define VK_LSHIFT 0xA0
#define VK_RSHIFT 0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_LMENU 0xA4
#define VK_RMENU 0xA5
#define VK_UP 0x26
#define VK_DOWN 0x28
#define VK_LEFT 0x25
#define VK_RIGHT 0x27
#define VK_INSERT 0x2D
#define VK_HOME 0x24
#define VK_END 0x23
#define VK_PAGEUP 0x21
#define VK_PAGEDOWN 0x22
#define VK_NUMLOCK 0x90
#define VK_CAPITAL 0x14
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
#define VK_MULTIPLY 0x6A
#define VK_SUBTRACT 0x6D
#define VK_ADD 0x6B
#define VK_PAUSE 0x13
#define VK_LBUTTON 0x01
#define VK_RBUTTON 0x02
#define VK_MBUTTON 0x03
#define VK_XBUTTON1 0x05
#define VK_XBUTTON2 0x06
#define SDL_SCANCODE_LAST SDL_SCANCODE_COUNT
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
#if !defined(__arm64__) && !defined(__aarch64__)
#include <emmintrin.h>
#endif
#endif

// DirectInput types
typedef struct { LONG lX; LONG lY; LONG lZ; LONG lRx; LONG lRy; LONG lRz; LONG rglSlider[2]; DWORD rgdwButton[32]; DWORD rgbButtons[32]; LONG lVX; LONG lVY; LONG lVZ; LONG lVRx; LONG lVRy; LONG lVRz; LONG lVFRx; LONG lVFRy; LONG lVFRz; LONG lVFRa; LONG lVFRb; LONG lVFRc; LONG lAS; LONG lARS; DWORD rglAxis[16]; DWORD rgdwPOV[16]; DWORD rglVAxis[16]; DWORD rgVAS; DWORD rgdwVPOV[16]; DWORD rglVPOV[16]; } DIJOYSTATE2;
typedef struct { DWORD dwSize; DWORD dwFlags; DWORD dwDevType; DWORD dwAxes; DWORD dwButtons; DWORD dwPOVs; DWORD dwFFSamplePeriod; DWORD dwFsUpdates; DWORD dwTotalInputs; DWORD dwSID; DWORD dwVendorID; DWORD dwVersion; } DIDATAFORMAT;
typedef struct { DWORD dwSize; DWORD dwFlags; DWORD dwDevType; DWORD dwAxes; DWORD dwButtons; DWORD dwPOVs; DWORD dwFFSamplePeriod; DWORD dwFsUpdates; DWORD dwTotalInputs; DWORD dwSID; DWORD dwVendorID; DWORD dwVersion; TCHAR tszProductName[128]; } DIDEVICEINSTANCE;

#include <SDL3/SDL.h>

// SDL GameController renames (SDL3 API changes) - placed after SDL3 include to override compat macros
#define SDL_GetNumGameControllers() SDL_GetNumGamepads()
#define SDL_GameControllerOpen SDL_OpenGamepad
#define SDL_GameControllerClose SDL_CloseGamepad
#define SDL_GameControllerGetAxis SDL_GamepadGetAxis
#define SDL_GameControllerGetButton SDL_GamepadGetButton
#define SDL_GameControllerName SDL_GetGamepadName
#define SDL_GameControllerMappingForIndex SDL_GamepadMappingForIndex
#define SDL_GameControllerFromInstanceID SDL_GamepadFromInstanceID
#define SDL_GameControllerEventState SDL_GamepadEventState
#define SDL_IsGameController SDL_IsGamepad
#define SDL_GameControllerGetJoystick SDL_GetGamepadJoystick

// SDL3 function aliases that Orbiter code uses directly
#define SDL_GamepadName SDL_GetGamepadName
#define SDL_GamepadGetJoystick SDL_GetGamepadJoystick

#define SDL_GetGameControllerInstanceID SDL_GetGamepadInstanceID
#define SDL_GameControllerGetDeviceInstanceID SDL_GetGamepadDeviceInstanceID

typedef struct SDL_Joystick SDL_Joystick;
typedef struct { int X; int Y; } COORD;
typedef struct { int left; int top; int right; int bottom; } SMALL_RECT;
typedef struct _CONSOLE_SCREEN_BUFFER_INFO { DWORD cbSize; COORD dwSize; COORD dwCursorPosition; WORD  wAttributes; SMALL_RECT srWindow; COORD dwMaximumWindowSize; } CONSOLE_SCREEN_BUFFER_INFO, *PCONSOLE_SCREEN_BUFFER_INFO;
typedef struct { int cx; int cy; } SIZE;
typedef struct { int id; int x; int y; int cx; int cy; int type; int flags; const char* text; void* hwnd; } DLGCTRL;
typedef struct { DWORD style; DWORD dwExtendedStyle; int cxdlg; int cydlg; WORD cdit; short x; short y; short cx; short cy; LPCSTR lpszName; LPCSTR lpszMenuName; WORD class_name; WORD origin; short width; short height; } DLGTEMPLATE;

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
typedef struct { short x; short y; } POINTS;

typedef unsigned char UINT8;
typedef void* LPMSG;
typedef void* HTREEITEM;
typedef long long LONGLONG;
typedef size_t SIZE_T;
typedef LRESULT (CALLBACK *FARPROC)(HWND, UINT, WPARAM, LPARAM);
typedef void* LPSECURITY_ATTRIBUTES;
typedef DWORD (*LPTHREAD_START_ROUTINE)(LPVOID);
typedef DWORD* LPDWORD;
typedef struct { HRESULT GetDeviceState(DWORD, LPVOID) { return 0; } HRESULT Acquire() { return 0; } HRESULT GetDeviceData(DWORD, DIDEVICEOBJECTDATA*, LPDWORD, DWORD) { return 0; } } *LPDIRECTINPUTDEVICE8;
#define ATTACH_PARENT_PROCESS 0xFFFFFFFF



// HTMLHelp functions
// HWND WINAPI HtmlHelpA(HWND hwndCaller, LPCSTR pszFile, UINT uCommand, DWORD_PTR dwData) { return 0; }

// HTMLHelp macros
#define HH_DISPLAY_TOPIC 0x0000



// Win32 API stubs
BOOL GetCursorPos(POINT* p) { *p = {0,0}; return TRUE; }
BOOL ScreenToClient(HWND, POINT*) { return TRUE; }
BOOL SetCursorPos(int, int) { return TRUE; }
HWND GetDesktopWindow() { return 0; }
BOOL AllocConsole() {}
BOOL AttachConsole(DWORD) {}
void sprintf_s(char* d, size_t, const char* f, ...) {}
HWND GetConsoleWindow() { return 0; }
void SetConsoleTitle(const char*) {}
HMENU GetSystemMenu(HWND, BOOL) { return nullptr; }
#define MF_BYCOMMAND 0x00000000L
#define STD_OUTPUT_HANDLE (-11)
#define WAIT_TIMEOUT 258L
DWORD WaitForSingleObject(HANDLE, DWORD) { return 0; }
void CloseHandle(HANDLE) {}
void ReleaseMutex(HANDLE) {}
#define STD_INPUT_HANDLE (-10)
#define STD_ERROR_HANDLE (-12)
SIZE_T lstrlenA(LPCSTR s) { return strlen(s); }
#define lstrlen lstrlenA
void strcpy_s(char* d, size_t, const char* s) { strcpy(d,s); }
#define D3DVAL(x) ((float)(x))
FARPROC GetProcAddress(HMODULE, const char*) { return 0; }
HANDLE GetStdHandle(int) { return 0; }
BOOL DeleteMenu(HMENU, UINT, UINT) { return TRUE; }
typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(LPVOID);
static inline HANDLE CreateThread(LPSECURITY_ATTRIBUTES sa, SIZE_T stack_size, LPTHREAD_START_ROUTINE start_addr, LPVOID param, DWORD create_flags, LPDWORD thread_id) { return (HANDLE)1; }
#define CreateThread(sa,stack_size,start_addr,param,create_flags,thread_id) CreateThread(sa,stack_size,start_addr,param,(create_flags)==NULL?0:(create_flags),(thread_id))
BOOL SetConsoleTextAttribute(HANDLE, WORD) { return TRUE; }
#define CreateMutex CreateMutexA
#define CreateDialogA(hInst, lpTemplateName, hWndParent, lpDialogFunc) CreateDialogParamA(hInst, lpTemplateName, hWndParent, lpDialogFunc, 0)
#define ReadConsole ReadConsoleA
#define WriteConsole WriteConsoleA
#define MAKEINTRESOURCE(x) ((LPSTR)((ULONG_PTR)((WORD)(x))))
#define ENABLE_LINE_INPUT 0x0002
#define ENABLE_ECHO_INPUT 0x0004
#define ENABLE_PROCESSED_INPUT 0x0001
#define FOREGROUND_RED 0x0004
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_INTENSITY 0x0008

// Console I/O and window management stubs
BOOL SetConsoleMode(HANDLE hConsoleHandle, DWORD dwMode) { return TRUE; }
BOOL GetConsoleMode(HANDLE hConsoleHandle, DWORD* lpMode) { if(lpMode) *lpMode = 0; return TRUE; }
BOOL ReadConsoleA(HANDLE, void*, DWORD, DWORD*, void*) { return FALSE; }
BOOL WriteConsoleA(HANDLE, const void*, DWORD, DWORD*, void*) { return TRUE; }
BOOL GetConsoleScreenBufferInfo(HANDLE, void*) { return TRUE; }
BOOL SetConsoleCursorPosition(HANDLE, COORD) { return TRUE; }
BOOL SetConsoleTitleA(const char*) { return TRUE; }
HANDLE CreateMutexA(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR) { return (HANDLE)1; }
HWND CreateDialogParamA(HINSTANCE, LPSTR, HWND, DLGPROC, LPARAM) { return nullptr; }
BOOL UpdateWindow(HWND) { return TRUE; }
BOOL KillTimer(HWND, UINT_PTR) { return TRUE; }
BOOL TerminateThread(HANDLE, DWORD) { return FALSE; }
BOOL DrawIcon(HDC, int, int, HICON) { return FALSE; }
HGDIOBJ GetStockObject(int type) {
    if (type == 5) return (HBRUSH)5;
    if (type == 8) return (HPEN)0;
    return nullptr;
}
HBRUSH CreateSolidBrush(COLORREF) { return (HBRUSH)1; }
HDC GetDC(HWND) { return (HDC)1; }
int ReleaseDC(HWND, HDC) { return 0; }
HWND GetActiveWindow() { return 0; }
HWND GetFocus() { return 0; }
BOOL InvalidateRect(HWND, const RECT*, BOOL) { return TRUE; }
BOOL RedrawWindow(HWND, const RECT*, HRGN, UINT) { return TRUE; }
UINT GetWindowLongPtrA(HWND, int) { return 0; }
LONG_PTR SetWindowLongPtrA(HWND, int, LONG_PTR) { return 0; }
#define SetWindowLongPtr SetWindowLongPtrA
#define SWP_FRAMECHANGED 0x0001
BOOL IsWindow(HWND) { return FALSE; }
UINT_PTR SetTimer(HWND, UINT_PTR, UINT, void*) { return 1; }
HMENU GetMenu(HWND) { return nullptr; }
BOOL AdjustWindowRectEx(LPRECT, DWORD, BOOL, DWORD) { return TRUE; }
LONG SetWindowLongA(HWND, int, LONG) { return 0; }
HICON LoadIconA(HINSTANCE, LPCSTR) { return (HICON)1; }
#define LoadIcon LoadIconA
HCURSOR LoadCursorA(HINSTANCE, LPCSTR) { return (HCURSOR)1; }
HFONT GetStockObjectA(int) { return (HFONT)0; }
#define SM_CYSCREEN 1
static inline BOOL StretchBlt(HDC,int,int,int,int,HDC,int,int,int,int,UINT) { return TRUE; }
static inline HBRUSH GetSysColorBrush(int) { return (HBRUSH)1; }
#define PBM_SETRANGE 0x0201
#define MAKELPARAM(w,l) ((LPARAM)(((WORD)(w))|((DWORD)((WORD)(l)))<<16))
#define TRANSPARENT 1
#define WM_CTLCOLORDLG 0x0137
#define PBM_SETPOS 0x0203
#define RDW_UPDATENOW 0x0100
#define RDW_ALLCHILDREN 0x0040
#define TVE_EXPAND 0x0002
#define TVE_COLLAPSE 0x0001
#define TV_FIRST 0x1100
#define TVM_FIRST TV_FIRST
#define TVGN_CARET 0x0009
#define TVM_DELETEITEM (TVM_FIRST + 0x0007)
#define TVI_ROOT ((HTREEITEM)-1)
#define TVM_GETNEXTITEM (TVM_FIRST + 0x0008)
#define TVGN_NEXT 0x0001
#define TVM_GETITEM (TVM_FIRST + 0x000D)
#define TVGN_CHILD 0x0004
#define TVIF_SELECTEDIMAGE 0x0020
#define TVGN_NEXT 0x0001
#define GetSystemMetrics(int) 0
#define TreeView_Expand(HWND,HTREEITEM,UINT) {}
#define TreeView_SetCheckState(HWND,HTREEITEM,BOOL) {}
#define TVIS_STATEIMAGEMASK 0xF000
#define TreeView_GetCheckState(HWND,HTREEITEM) 0
#define MB_ICONWARNING 0x00000030L
#define NM_CUSTOMDRAW ((UINT)(-2300000012))
#define GWLP_HWNDPARENT (-8)
typedef struct { UINT cbSize; UINT style; LRESULT (*lpfnWndProc)(HWND, UINT, WPARAM, LPARAM); int cbClsExtra; int cbWndExtra; HINSTANCE hInstance; HICON hIcon; HCURSOR hCursor; HBRUSH hbrBackground; LPCSTR lpszMenuName; LPCSTR lpszClassName; HICON hIconSm; } WNDCLASSEX;
static inline ATOM RegisterClassExA(const WNDCLASSEX*) { return 1; }
#define RegisterClassEx RegisterClassExA
#define TreeView_SetItemState(HWND,HTREEITEM,uState,uMask) {}
#define LoadResourceA(H,HRC) ((HGLOBAL)1)
#define LoadResource(H,HRC) ((HGLOBAL)1)
#define LockResource(HGLOBAL) ((LPVOID)1)
#define _splitpath(s,drive,dir,name,ext) {}
#define CB_FINDSTRING 0x014C
#define CB_GETLBTEXT 0x0148
#define CB_FINDSTRINGEXACT 0x0158
#define LBN_SELCHANGE 0x0001
#define LB_GETSEL 0x0187
#define LB_RESETCONTENT 0x0184
#define LB_SETSEL 0x0185
#define SB_LINEDOWN 0x0001
#define SB_LINEUP 0x0000
#define SB_PAGEDOWN 0x0003
#define SB_PAGEUP 0x0002
#define CBN_SELCHANGE 0x0005
#define WS_SIZEBOX 0x00040000L
#define SW_SHOWNOACTIVATE 7
#define WM_MOVE 0x0003
#define WM_APP 0x8000
#define FindFirstChangeNotificationA(LPSTR,BOOL,DWORD) ((HANDLE)1)
#define FindFirstChangeNotification FindFirstChangeNotificationA
static inline char* _fullpath(char* buf, const char* path, size_t maxlen) { return realpath(path, buf); }
#define WS_HSCROLL 0x00010000L
#define WS_THICKFRAME 0x00040000L
#define MAKELONG(w,l) ((LONG)(((WORD)(w))|((DWORD)((WORD)(l)))<<16))
static inline HDC GetWindowDC(HWND) { return (HDC)1; }
static inline BOOL BitBlt(HDC,int,int,int,int,HDC,int,int,UINT) { return TRUE; }
#define ImageList_Destroy(HIMAGELIST) TRUE
#define TVI_FIRST ((HTREEITEM)-3)
#define TVGN_PREVIOUS 0x0003
static inline HTREEITEM TreeView_GetParent(HWND, HTREEITEM) { return 0; }
#define MB_ICONQUESTION 0x00000020L
#define MB_ICONINFORMATION 0x00000040L
#define FILE_NOTIFY_CHANGE_FILE_NAME 0x00000001
#define FILE_NOTIFY_CHANGE_DIR_NAME 0x00000002
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
static inline HANDLE FindNextChangeNotification(HANDLE) { return (HANDLE)1; }
static inline BOOL FindCloseChangeNotification(HANDLE) { return TRUE; }
static inline HTREEITEM TreeView_SelectItem(HWND, HTREEITEM) { return 0; }
HGDIOBJ SelectObject(HDC, HGDIOBJ) { return nullptr; }
int GetClientRect(HWND, RECT*) { return 0; }
LRESULT DefDlgProcA(HWND, UINT, WPARAM, LPARAM) { return 0; }
int SendDlgItemMessageA(HWND, int, UINT, WPARAM, LPARAM) { return 0; }
BOOL IsDialogMessageA(HWND, void*) { return FALSE; }
HFONT CreateFontA(int, int, int, int, int, BOOL, BOOL, BOOL, UINT, UINT, UINT, UINT, UINT, LPCSTR) { return (HFONT)1; }
HRGN CreateRectRgn(int, int, int, int) { return (HRGN)1; }
int GetTextExtentPoint32A(HDC, LPCSTR, int, SIZE*) { return 0; }
UINT GetMenuItemCount(HMENU) { return 0; }
int GetMenuItemID(HMENU, int) { return 0; }
HGDIOBJ CreateFontIndirectA(const void*) { return (HGDIOBJ)1; }
BOOL GetWindowRect(HWND, RECT*) { return TRUE; }

HINSTANCE LoadLibraryA(LPCSTR) { return (HINSTANCE)1; }
BOOL FreeLibrary(HINSTANCE) { return TRUE; }
BOOL SetConsoleCtrlHandler(void*, BOOL) { return TRUE; }

static inline UINT timeBeginPeriod(UINT uMilliseconds) { return 1; }
static inline BOOL PostMessageA(HWND, UINT, WPARAM, LPARAM) { return FALSE; }
static inline BOOL PostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return PostMessageA(hWnd, Msg, wParam, lParam); }
static inline BOOL InitCommonControls() { return TRUE; }
static inline BOOL GetClassInfoA(HINSTANCE, LPCSTR, WNDCLASS*) { return TRUE; }
static inline LONG RegOpenKeyExA(HKEY, LPCSTR, DWORD, DWORD, HKEY*) { return 0; }
#define RegOpenKeyEx RegOpenKeyExA
static inline LONG RegCloseKey(HKEY) { return 0; }
static inline HMODULE LoadLibraryExA(LPCSTR, HANDLE, DWORD) { return (HMODULE)1; }
#define LoadLibraryEx LoadLibraryExA
static inline HCURSOR SetCursor(HCURSOR) { return 0; }
#define _chdir chdir
static inline int _execl(const char* path, const char* arg, ...) { return 0; }
#define PM_NOREMOVE 0x0000
#define WM_QUIT 0x0012
static inline BOOL PeekMessageA(LPMSG, HWND, UINT, UINT, UINT) { return FALSE; }
static inline BOOL GetMessageA(LPMSG, HWND, UINT, UINT) { return FALSE; }
#define HKEY_CURRENT_USER ((HKEY)0x80000001u)
static inline int ShowCursor(BOOL) { return 1; }
static inline BOOL SetCapture(HWND) { return TRUE; }
static inline BOOL ClientToScreen(HWND, POINT*) { return TRUE; }
static inline BOOL ClipCursor(const RECT*) { return TRUE; }
static inline BOOL ReleaseCapture() { return TRUE; }
#define DIERR_NOTACQUIRED (-2128964351)
#define DIERR_INPUTLOST (-2128964350)
#define SUCCEEDED(x) ((HRESULT)(x) >= 0)
#define DIK_LSHIFT 0xA0
#define WM_ACTIVATE 0x0006
#define WA_INACTIVE 0
#define WM_CHAR 0x0102
static inline DWORD GetWindowThreadProcessId(HWND, LPDWORD) { return 0; }
static inline HWND SetFocus(HWND) { return 0; }
#define SDL_SYSWMEVENT 0
typedef struct { POINT ptReserved; POINT ptMaxSize; POINT ptMaxPosition; POINT ptMinTrackSize; POINT ptMaxTrackSize; } MINMAXINFO;
#define WM_POWERBROADCAST 0x0218
#define PBT_APMQUERYSUSPEND 0x0000
#define PBT_APMRESUMESUSPEND 0x0003
#define SC_MONITORPOWER 0xF170
static inline LRESULT SendMessageA(HWND, UINT, WPARAM, LPARAM) { return 0; }
static inline LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return SendMessageA(hWnd, Msg, wParam, lParam); }
#define HTCLIENT 0x0001
#define WM_NOTIFY 0x004E
#define GWL_STYLE (-16)
#define TVS_DISABLEDRAGDROP 0x0800
#define TVS_SHOWSELALWAYS 0x0008
#define TVS_NOTOOLTIPS 0x0080
#define WS_BORDER 0x00800000L
#define WS_TABSTOP 0x00010000L
#define DefWindowProc DefWindowProcA
static inline BOOL SystemParametersInfoA(UINT, UINT, PVOID, UINT) { return FALSE; }
#define SystemParametersInfo SystemParametersInfoA
#define KEY_QUERY_VALUE 0x0001
#define ERROR_SUCCESS 0
#define SW_MAXIMIZE 3
#define SPI_GETFONTSMOOTHING 0x004B
#define SPI_SETFONTSMOOTHING 0x004C
#define SPIF_SENDCHANGE 0x0002
static inline void timeEndPeriod(UINT uMilliseconds) { }
#define LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR 0x00000100
#define LOAD_LIBRARY_SEARCH_DEFAULT_DIRS 0x00000800
#define LOAD_LIBRARY_AS_DATAFILE 0x00000040
static inline UINT LoadStringA(HINSTANCE, UINT, LPSTR, int) { return 0; }
#define LoadString LoadStringA
static inline DWORD GetLastError() { return 0; }
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define LANG_NEUTRAL 0x00
#define SUBLANG_DEFAULT 0x01
static inline DWORD timeGetTime() { return (DWORD)SDL_GetTicks(); }
static inline UINT SetTextAlign(HDC hDC, UINT fMode) { return 0; }
static inline BOOL MoveToEx(HDC hDC, int X, int Y, LPPOINT lpPoint) { if(lpPoint) { lpPoint->x = 0; lpPoint->y = 0; } return TRUE; }
static inline BOOL LineTo(HDC hDC, int X, int Y) { return TRUE; }
#define FormatMessage FormatMessageA
static inline HANDLE GetCurrentProcess() { return (HANDLE)-1; }
static inline BOOL GetProcessMemoryInfo(HANDLE, PPROCESS_MEMORY_COUNTERS pPMC, DWORD cb) { if(pPMC) { pPMC->WorkingSetSize = 0; pPMC->PeakWorkingSetSize = 0; } return TRUE; }
static inline int _fseeki64(FILE* stream, long long offset, int whence) { return fseeko(stream, (off_t)offset, whence); }
static inline HANDLE OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) { (void)dwDesiredAccess; (void)bInheritHandle; (void)dwProcessId; return (HANDLE)1; }
static inline BOOL GetModuleInformation(HANDLE, HMODULE, void*, DWORD) { return TRUE; }
#define GetFileVersionInfoSize GetFileVersionInfoSizeA
#define GetFileVersionInfo GetFileVersionInfoA

// Additional Win32 stubs
static inline DWORD GetCurrentDirectoryA(DWORD nBufferLength, LPSTR lpBuffer) {
    if (lpBuffer) {
        char* cwd = getcwd(nullptr, 0);
        if (cwd) {
            size_t len = strlen(cwd);
            if (len < nBufferLength) {
                strcpy(lpBuffer, cwd);
                free(cwd);
                return len;
            }
            free(cwd);
        }
        if (!lpBuffer[0]) lpBuffer[0] = '/';
        return 0;
    }
    return 0;
}
#define GetCurrentDirectory GetCurrentDirectoryA

static inline BOOL SetCurrentDirectoryA(LPCSTR lpPathName) {
    return (chdir(lpPathName) == 0);
}

static inline int _chmod(const char* pathname, int mode) { return chmod(pathname, mode); }
static inline BOOL SetWindowTextA(HWND, LPCSTR) { return TRUE; }
#define HIMAGELIST HANDLE
#define ILC_COLOR8 0x00000020L
static inline HIMAGELIST ImageList_Create(int, int, int, int, int) { return (HIMAGELIST)1; }
static inline int ImageList_Add(HIMAGELIST, HBITMAP, COLORREF) { return 0; }
#define TVM_SETIMAGELIST (TVM_FIRST + 0x0019)
#define TVSIL_NORMAL 0
#define SW_HIDE 0
#define SW_SHOW 5
#define BM_SETCHECK 0x00F0
#define BST_CHECKED 1
#define BST_UNCHECKED 0
#define IDHELP 4
#define WM_SHOWWINDOW 0x001F
#define WM_CTLCOLORSTATIC 0x0138
#define SIZE_RESTORED 0
#define SIZE_MINIMIZED 1
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#define ETDT_ENABLE 1
#define IsDialogMessage IsDialogMessageA
#define EnableWindow(HWND,BOOL) TRUE
#define IsIconic(HWND) FALSE
#define PostQuitMessage(int) {}
#define BLACK_BRUSH 4
#define LoadImage LoadImageA
#define IMAGE_BITMAP 0
static inline HBITMAP LoadImageA(HINSTANCE, LPCSTR, UINT, int, int, UINT) { return (HBITMAP)1; }
static inline BOOL DeleteObject(HGDIOBJ) { return TRUE; }
#define GetWindowLongPtr GetWindowLongPtrA
#define SendDlgItemMessage SendDlgItemMessageA
#define ShellExecute ShellExecuteA
#define FindResource FindResourceA
#define TVIF_HANDLE 0x0010
#define TVIF_CHILDREN 0x0080
#define TVN_SELCHANGED (-455)
#define NM_DBLCLK 3
typedef struct { int mask; HTREEITEM hItem; UINT state; UINT stateMask; LPSTR pszText; int cchTextMax; int iImage; int iSelectedImage; int cChildren; LPARAM lParam; } TV_ITEM;
typedef struct { NMHDR hdr; UINT action; HTREEITEM hItem; LPARAM lParam; TV_ITEM itemNew; } NM_TREEVIEW;
typedef NM_TREEVIEW* LPNMTREEVIEW;
#define TVITEM TV_ITEM
#define GetConsoleProcessList(a,b) 1
#define CreateDialogParam CreateDialogParamA
#define DWLP_USER 8
#define HWND_BOTTOM ((HWND)1)
#define LB_ADDSTRING 0x0180
#define SW_SHOWNORMAL 1
#define CB_RESETCONTENT 0x014B
#define CB_ADDSTRING 0x0143
#define CB_SETCURSEL 0x014E
#define BM_GETCHECK 0x0F0E
#define CB_GETCURSEL 0x0147
#define CB_ERR (-1)
#define BN_CLICKED 0
#define MB_ICONEXCLAMATION 0x00000030L
static inline int GetWindowTextA(HWND, LPSTR, int) { return 0; }
#define GetWindowText GetWindowTextA
#define EnableThemeDialogTexture(HWND,UINT) S_OK
static inline DWORD SetTextColor(HDC, COLORREF) { return 0; }
static inline DWORD SetBkColor(HDC, COLORREF) { return 0; }
static inline int SetBkMode(HDC, int) { return 0; }
#define DialogBoxParamA(hInst,lpTemplateName,hWndParent,lpDialogFunc,LPARAM) 0
#define FindResourceA(h,lpName,lpType) ((HRSRC)1)
static inline HDC CreateCompatibleDC(HDC) { return (HDC)1; }
static inline BOOL DeleteDC(HDC) { return TRUE; }
#define SRCCOPY 0x00CC0020
#define COLOR_3DFACE 15
#define NULL_PEN 8
static inline BOOL Rectangle(HDC, int, int, int, int) { return TRUE; }
#define PS_SOLID 0
#define NULL_BRUSH 5
static inline HGDIOBJ CreatePen(int fnPenStyle, int cWidth, COLORREF crColor) { (void)fnPenStyle; (void)cWidth; (void)crColor; return (HGDIOBJ)1; }
static inline BOOL TextOutA(HDC, int, int, LPCSTR, int) { return TRUE; }
#define TextOut TextOutA
typedef MINMAXINFO* LPMINMAXINFO;
#define DialogBoxParam DialogBoxParamA
#define EndDialog(hwnd,retval) {}
#define TVI_LAST ((HTREEITEM)-2)
#define TVI_SORT ((HTREEITEM)-5)
typedef struct { HTREEITEM hInsertAfter; HTREEITEM hItem; HTREEITEM hParent; UINT format; TV_ITEM item; LPARAM lParam; } TV_INSERTSTRUCT;
static inline BOOL TreeView_DeleteItem(HWND, HTREEITEM) { return TRUE; }
static inline void TreeView_DeleteAllItems(HWND) {}
static inline HTREEITEM TreeView_InsertItem(HWND, const TV_INSERTSTRUCT*) { return 0; }
static inline HTREEITEM TreeView_GetItem(HWND, const TV_ITEM*) { return 0; }
static inline HTREEITEM TreeView_GetNextSibling(HWND, HTREEITEM) { return 0; }
static inline HTREEITEM TreeView_GetChild(HWND, HTREEITEM) { return 0; }
static inline HTREEITEM TreeView_GetRoot(HWND) { return 0; }
static inline HTREEITEM TreeView_GetSelection(HWND) { return 0; }
typedef void* HGDIOBJ;
#define CP_UTF8 65001
static inline int MultiByteToWideChar(UINT CodePage, UINT dwFlags, const char* lpMultiByteStr, int cbMultiByte, wchar_t* lpWideCharStr, int cchWideChar) { return 0; }
static inline HBITMAP LoadBitmapA(HINSTANCE, LPCSTR) { return (HBITMAP)1; }
#define LoadBitmap LoadBitmapA
#define COLORREF ULONG

// DirectDraw types
typedef struct { unsigned long cbSize; unsigned long dwFlags; unsigned long dwRGBBitCount; unsigned long dwRBitMask; unsigned long dwGBitMask; unsigned long dwBBitMask; unsigned long dwRGBAlphaBitMask; } DDPIXELFORMAT;
typedef struct { unsigned short x; unsigned short y; } SIZEL;
typedef SIZEL* PSIZEL;
typedef struct { unsigned long cbSize; unsigned long dwFlags; unsigned long dwFillColor; unsigned long dwReserved1; unsigned long dwFillColorOpaque; } DDBLTFX;

// GDI bitmap type
typedef struct { unsigned char bmType; unsigned char bmWidth; unsigned char bmHeight; unsigned char bmWidthEx; unsigned long bmHeightEx; unsigned long bmWidthBytes; unsigned short bmPlanes; unsigned short bmBitsPixel; void* bmBits; } BITMAP;

// GDI GetObject stub
static inline int GetObjectA(HGDIOBJ h, int c, void* d) { (void)h; (void)c; (void)d; return 0; }
static inline int GetObjectW(HGDIOBJ h, int c, void* d) { (void)h; (void)c; (void)d; return 0; }
#define GetObject GetObjectA

typedef unsigned char* LPBYTE;
#endif // PLATFORM_SDL_H
