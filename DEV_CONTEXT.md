# SDL3 Port - Development Context

## Status: ACTIVE - Infrastructure phase complete, transitioning to source code modifications

## Progress Tracker ✅
- [x] Root CMakeLists.txt: Added SDL3 + SDL3_shadercross find_package + ORBITER_BUILD_SDLGPUCLIENT option
- [x] OVP/CMakeLists.txt: Conditional SDLGPUClient subdirectory (already in commit)
- [x] Extern/stb/CMakeLists.txt: stb_image FetchContent (already in commit)
- [x] SdlBridge.cpp: Cleaned up broken implementation, fixed brace matching and syntax
- [x] Src/Orbiter/CMakeLists.txt: Added SdlBridge sources, conditional SDL3 + stb linking
- [x] CMakePresets.json: Added macOS ARM64 + Linux x64 build profiles with SDLGPUCLIENT support

## TODO - NEXT PRIORITY
### Orbiter.cpp (Src/Orbiter/Orbiter.cpp)
1. Add `#include "SDLBridge.h"` at top
2. Add `#ifdef ORBITER_BUILD_SDLGPUCLIENT` guards around Win32-specific code:
   - `WinMain()` → `SDL_main()` entry point
   - `HWND hRenderWnd` → `SDL_Window* sdlWindow`
   - `MSG msg` + PeekMessage/GetMessage → SdlBridge::PumpWindowEvents()
3. Replace `SetCurrentDirectory()`, `GetCurrentDirectory()`, `InitCommonControls()`, etc. with SDL equivalents
4. Run loop changes: replace `TranslateMessage/DispatchMessage` with SdlBridge event processing

### Input system files
1. `Input.cpp` - DInput class: Keep existing API (`PollJoystick`, `SetJoystickProperties`) add SDL branch in `Create()` and `Destroy()`
2. `Di7frame.h/cpp` - CDIFramework7: Guard with `#ifdef ORBITER_BUILD_D3D9CLIENT`, add `SDLInputBridge` class behind it for SDLGPUCLIENT builds
3. Create `OVP/Input/SDLInput.cpp` - joystick enumeration via SDL_GameController, key scanning via SDL_GetKeyState(), proper deadzone/saturation mapping

### GraphicsAPI.cpp (Src/Orbiter/GraphicsAPI.cpp)
- Strip out all WIC/GDI includes and decode/surface creation code
- Add `#include <stb_image.h>` + stb decode stubs in `clbkCreateTexture()` and surface blitting paths
- Replace GDI SetPixel/BitBlt with SDL surface manipulation

### Other files requiring modification
- `DialogWin.cpp` - Dialog window creation: wrap Win32 CreateDialog + AddSDLWindow() for SdlBridge integration
- `DlgCtrl_Sdl.h` stub: already present, implement in DlgCtrl module
- `keymap.cfg` reader (Keymap.cpp): unchanged, still uses VK codes

## Architecture Decisions

### Graphics Client
- New abstract graphics client at `OVP/SDLGPUClient/`
- Allowing any backend via SDL_gpu + runtime HLSL compilation using `SDL_ShaderCross`.
- Replace Win32 `WinMain()` with `SDL_main(int argc, char* argv[])` and translate PeekMessage/GetMessage to SDL event loops.

### Input Mapping (from DInput)
1. DirectInput COM initialization → SDL_GameControllerInit + SDL_JoystickAttach (`OVP/Input/Input.cpp`)
2. Replace Poll() event loop with SDL_PollEvent polling on each game frame (`DInput/Di7frame.h/cpp`)
3. Use SDL_KeyMap scancode→VK code to maintain `keymap.cfg` compatibility
4. All input code paths must be functional and identical between D3D9 + sdlgpu graphics clients.

### Image Loading (WIC replacement)
1. WIC/GDI usage in `GraphicsAPI.cpp` removed completely → replaced by `stb_image` for all decoding/surface blitting. (`Orbitersdk/include/Bitmap.h`)
2. stb_image loaded into `Extern/stb/` via Git submodule or CMake FetchContent.

### CMake Changes
- Root CMakeLists.txt:
  * Added SDL3 + SDL3_shadercross conditionals with find_package fallbacks.
  * Conditional ORBITER_BUILD_D3D9CLIENT boolean option defaulting to ON.
- OVP/SDLGPUClient/CMakeLists.txt: New target linking SDL3, ShaderCross + GLSLc → HLSL pipeline at runtime.
- Src/Orbiter CMakeLists.txt: Added SdlBridge sources + conditional SDL3 + stb_image linking.

### Platform Support via CMakePresets.json
- macOS ARM64: ORBITER_BUILD_D3D9CLIENT=OFF, ORBITER_BUILD_SDLGPUCLIENT=ON, clang++ compiler
- Linux x64: Already had D3D9CLIENT disabled in linux-winegcc-base preset
