// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ====================================================================================
// File: Di7frame.h
// Desc: Class to manage DirectInput OR SDL environment objects
// ====================================================================================

#ifndef DI7FRAME_H
#define DI7FRAME_H
#define STRICT 1
#include <windows.h>
#ifdef HAS_DINPUT
#include <dinput.h>
#include <d3d.h>
#endif

#ifdef ORBITER_BUILD_SDLGPUCLIENT
class CDIFramework7 {
	SDL_GameController* sdl_ctrl = nullptr;
	int joy_count = 0;
public:
	CDIFramework7() { SDL_joy_count = SDL_GetNumGameControllers(); }
	~CDIFramework7() { SDL_joy_cleanup(); }

	HRESULT Create(HINSTANCE) { 
		SDL_InitSubsystem(SDL_INIT_GAMECONTROLLER); 
		return S_OK; 
	}
	VOID Destroy() { SDL_joy_cleanup(); }
	VOID GetJoysticks(DIDEVICEINSTANCE**, DWORD*) {}
	DWORD NumJoysticks() const { return joy_count; }

	HRESULT CreateDevice(HWND, LPDIRECTINPUT8, LPDIRECTINPUTDEVICE8&, GUID, const DIDATAFORMAT*, DWORD) {
		return S_FALSE;
	}
	HRESULT CreateKbdDevice(HWND) { return S_OK; }
	HRESULT CreateMouseDevice(HWND) { return S_OK; }
	HRESULT CreateJoyDevice(HWND,	DWORD idx = 0) { 
		joy_count = SDL_GetNumGameControllers();
		if (idx < joy_count) sdl_ctrl = SDL_OpenGameController(idx);
		return S_OK; 
	}
	void DestroyJoyDevice() { if(sdl_ctrl){SDL_CloseGameController(sdl_ctrl); sdl_ctrl=nullptr;} }
	void DestroyDevices() { DestroyJoyDevice(); }

	LPDIRECTINPUTDEVICE8 GetKbdDevice() { return nullptr; }
	LPDIRECTINPUTDEVICE8 GetMouseDevice() { return nullptr; }
	LPDIRECTINPUTDEVICE8 GetJoyDevice() { return nullptr; }

private:
	int SDL_joy_count = 0;
	void SDL_joy_cleanup() {};
};
#else // ORBITER_BUILD_SDLGPUCLIENT
//-----------------------------------------------------------------------------
// Name: CDIFramework7
// Desc: The DirectInput framework class for DX7. Maintains the DI devices
//-----------------------------------------------------------------------------
class CDIFramework7
{
	LPDIRECTINPUT8       m_pDI;             // DInput object
	LPDIRECTINPUTDEVICE8 m_pdidKbdDevice;   // keyboard device
	LPDIRECTINPUTDEVICE8 m_pdidMouseDevice; // mouse device
	LPDIRECTINPUTDEVICE8 m_pdidJoyDevice;   // joystick device
	GUID                 m_guidJoystick;    // GUID for the joystick
	BOOL                 m_bUseKbd;
	BOOL                 m_bUseJoy;

	struct JLIST {
		DIDEVICEINSTANCE*    descJoy;     // list of enumerated joystick devices
		DWORD                nJoy;        // number of enumerated joysticks
	} jList;

	static BOOL CALLBACK EnumJoysticksCallback (LPCDIDEVICEINSTANCE pInst,
		VOID* pvContext);

public:
	CDIFramework7();
	~CDIFramework7();

	HRESULT Create (HINSTANCE hInst);

	VOID Destroy ();

	VOID GetJoysticks (DIDEVICEINSTANCE **dev, DWORD *pdwCount);

	DWORD NumJoysticks () const { return jList.nJoy; }

	HRESULT CreateDevice (HWND hWnd, LPDIRECTINPUT8 pDI,
		LPDIRECTINPUTDEVICE8 pDIDevice, GUID guidDevice, const DIDATAFORMAT *pdidDataFormat,
		DWORD dwFlags);

	HRESULT CreateKbdDevice (HWND hWnd);
	HRESULT CreateMouseDevice (HWND hWnd);
	HRESULT CreateJoyDevice (HWND hWnd, DWORD idx = 0);

	void DestroyJoyDevice();
	void DestroyDevices();

	inline LPDIRECTINPUTDEVICE8 GetKbdDevice() { return m_pdidKbdDevice; }
	inline LPDIRECTINPUTDEVICE8 GetMouseDevice() { return m_pdidMouseDevice; }
	inline LPDIRECTINPUTDEVICE8 GetJoyDevice() { return m_pdidJoyDevice; }
};
#endif // !ORBITER_BUILD_SDLGPUCLIENT

#endif // !DI7FRAME_H
