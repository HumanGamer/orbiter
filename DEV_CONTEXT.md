# SDL3 PORT PROJECT CONTEXT - ORBITER 21.x → SDL_gpu Cross-Platform

## MACHINE CONFIG
- M3 MacBook Air, macOS Tahoe (Apple Silicon)
- SDL3 3.4.12 installed via Homebrew: /opt/homebrew/Cellar/sdl3/
- SDL3_shadercross 3.0.0 via pkg-config: /usr/local/lib/pkgconfig/sdl3-shadercross.pc
- stb_image in /Users/mattparizeau/Documents/OpenCode/orbiter/Extern/stb/

## PROJECT STRUCTURE (relevant directories)
```
/Users/mattparizeau/Documents/OpenCode/orbiter/
  CMakeLists.txt                          # Root CMake - add SDL3 find_package + SDLGPUCLIENT option
  OVP/CMakeLists.txt                      # Add SDLGPUClient subdirectory conditionally
  OVP/D3D9Client/                         # Original D3D9 client (68 cpp, 70+ h) - KEEP FOR NOW
  OVP/SDLGPUClient/                       # NEW: SDL_gpu based graphics client
    CMakeLists.txt
    include/gcCore.h, gpuTypes.h, gpuPipeline.h, gpuSurface.h, gpuBackend.h
    src/gpuBackend.cpp, gpuSurfaces.cpp, gpuSketchpad.cpp, gpuMesh.cpp, gpuParticle.cpp
    src/pipeline/shader_compiler.cpp/h, shaderCross.h
    gcCore.cpp, OapiExtension.cpp         # Bridge between Orbiter core and SDL_gpu
    shaders/*.hlsl                        # HLSL shaders (compiled at runtime via shadercross)
  Src/Orbiter/                            # CORE CHANGES HERE  
    CMakeLists.txt                        # Add SdlBridge sources + SDL3/stb links
    include/SdlBridge.h, include/gcCore.h
    Orbiter.cpp                           # Port WinMain → SDL_main
    GraphicsAPI.cpp                       # Remove WIC/GDI, replace with stb_image + SDL surfaces
    Input.h/cpp                           # Replace DirectInput → SDL_GameController/SDL_Keyboard
    Di7frame.h/cpp                        # Remove COM/DirectInput, use SDL joystick enumerate
  Extern/stb/CMakeLists.txt               # FetchContent for stb_image (single header)
```

## KEY FILES TO MODIFY (already done or in progress):
1. ✅ Extern/CMakeLists.txt → add_subdirectory(stb)
2. ✅ Extern/stb/CMakeLists.txt → FetchContent + stb image library target
3. ✅ CMakeLists.txt → find_package(SDL3 REQUIRED) + find_package(SDL3_shadercross REQUIRED) + SDLGPU_CLIENT option
4. ✅ OVP/SDLGPUClient/* → All source files created (gpuBackend, gpuSurfaces, gpuSketchpad, gpuMesh, shader_cross, gcCore)
5. ⏳ Src/Orbiter/CMakeLists.txt → Add SdlBridge, replace dinput8/lib with SDL3::SDL3 + stb
6. ⏳ Src/Orbiter/Orbiter.cpp → WinMain() → int main(int argc, char* argv[])
7. ⏳ Src/Orbiter/Run function → Replace PeekMessage/GetMessage loop with SDL_PollEvent
8. ⏳ Input.cpp/Di7frame.h/cpp → DirectInput → SDL_GameController + SDL_GetKeyboardState
9. ⏳ GraphicsAPI.cpp → Remove IWICImagingFactory, WriteImageDataToFile replaced with stbi_write
10. 🔲 DlgCtrl/* → Need platform-specific bridge (see below)

## ARCHITECTURE DECISIONS
- Entry point: `int main(int argc, char* argv[])` with SDL_Init() before anything else
- Windowing: SDL_CreateWindow for render window + dialog popups
- Dialog system: 2 approaches needed:
  a) Simple dialog windows using SDL_CreatePopupWindow or child windows
  b) Message translation layer so existing DlgCtrl code works without major changes
- Input: SDL_GetKeyboardState() directly, SDL_SetTextInputRect for text input boxes  
- Joystick: SDL_GameController + SDL_Joystick enums (replace DIDevice enum loop)
- Image loading: stb_image for texture loading (load .png/.jpg/.bmp from memory or file)
- Render pipeline: SDL_gpu with shadercross runtime HLSL→Metal/DXIL/GLSL compilation
- Shader targets: Metal on macOS, DXC/DXIL on Windows, GLSL/SPIR-V on Linux
- ImGui: Keep existing imgui files - SDL3 backend needed (imgui_impl_sdl3.cpp/h)
- stb_image_stbi_write for WriteImageDataToFile (bmp/png/jpg support)

## CRITICAL INTERFACE CONTRACT (DO NOT BREAK)
The graphics client DLL must export these exact signatures (from D3D9Client interface):
```cpp
extern "C" DLLEXPORT void* oapiGetGraphicsClient() { // Must exist in SDLGPUClient export
    return &g_sdlgpu_instance;  // oapi::GraphicsClient derived instance
}

// The base class interface in Orbitersdk/include/GraphicsAPI.h:
class GraphicsClient : public Module {
  SURFHANDLE clbkCreateSurface(DWORD w, DWORD h, SURFHANDLE) { }
  bool         clbkReleaseSurface(SURFHANDLE surf) { return false; }
  Sketchpad*   clbkGetSketchpad(SURFHANDLE surf) { return NULL; }
  void         clbkRenderScene() = 0;      // MUST IMPLEMENT
  bool         clbkDisplayFrame() { return false; }
  void         clbkCloseSession(bool fastclose) {}
  void         clbkDestroyRenderWindow(bool) {}
  // ... plus many more virtuals for surfaces, textures, fonts, pens, brushes
};

// The D3D9Client has ~60 virtual methods to implement. 
// Key ones: clbkCreateSurface (via SDL_GPUTexture), clbkGetSketchpad (2D primitives),
//           clbkLoadTexture (texture from file), clbkBlt/blit operations,
//           clbkRenderScene (main render loop).
```

## CMAKE DETAIL
Root CMakeLists changes:
- After all existing find_package() calls, add:
  ```cmake
  find_package(SDL3 REQUIRED) 
  find_package(SDL3_shadercross REQUIRED)
  
  option(ORBITER_BUILD_SDLGPUCLIENT "Build SDL GPU client" OFF)
  if(ORBITER_BUILD_SDLGPUCLIENT)
      add_subdirectory(OVP/SDLGPUClient)
  endif()
  ```

OVP/CMakeLists.txt changes:
- Add: `if(ORBITER_BUILD_SDLGPUCLIENT) add_subdirectory(SDLGPUClient) endif()`

Src/Orbiter/CMakeList.txt changes:
- Add SdlBridge.cpp to SOURCES
- Replace dinput8.lib, dxguid.lib with SDL3::SDL3  
- Add stb::image dependency
- Remove `#pragma comment(linker,"manifestdependenc...")` (no longer needed)
- Add imgui_impl_sdl3.cpp (replace imgui_impl_win32.cpp for the bridge)

## SHADER CROSS USAGE (runtime compilation)
```cpp
// Initialize backend (once at startup):
SDL_shadercross_InitDXCompiler();    // Windows
SDL_shadercross_InitMetal();         // macOS
SDL_shadercross_InitGLSLCompilers(); // Linux

// Compile a vertex shader:
SDL_GPUCompiledShader* compiled = SDL_shadercross_CompileHLSLtoMSL(
    hlsl_source, strlen(hlsl_source), 0);

SDL_GPUShaderCreateInfo shaderCI{};
shaderCI.code = (uint8_t*)compiled->bytecode;
shaderCI.code_size = compiled->bytecode_size;
shaderCI.entry_point = "main"; // or appropriate entry point name
shaderCI.stage = SDL_GPU_SHADERSTAGE_VERTEX;
shaderCI.format = SDL_GPU_SHADERFORMAT_MSL;

SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderCI);
SDL_free(compiled->bytecode);
SDL_free(compiled);
```

## INPUT MAPPING (DirectInput → SDL)
VK code mapping for keyboard:
- SDL_SCANCODE_A→VK_A, SDL_SCANCODE_SPACE→VK_SPACE, etc.
- Use SDL_GetScancodeName() + manual lookup table for unmapped keys
- Modifier tracking via SDL_GetModState()

Joystick/Game controller:
- Old: DirectInput8 CreateDevice enumeration → GUID mapping to axes (roll,pitch,yaw,ruddle,throttle)
- New: SDL_GameControllerAddMapping() + SDL_GameControllerGetAxis() for standard mappings
- Fallback: raw axis mapping via SDL_JoystickGetAxis() for non-standard controllers

SDL_GameController mapping names used in Orbiter keymap.cfg:
- "axis 0" = roll left/right (+= right, -= left) - X axis
- "axis 1" = pitch backward/forward (+= back, -= forward) - Y axis (reversed from normal)
- "axis 2" = yaw left/right (+= right, -= left) - Z or RZ axis
- "throttle" = throttle level (0 to 1)

## WINDOW/HWND SUBSTITUTIONS (Win32 → SDL)
HWND hWnd:       SDL_Window* window      - Replace all HWND uses with SDL_Window pointers  
WNDCLASS:        No equivalent            - Window class is implicit in SDL  
CreateWindow:    SDL_CreateWindow()     - Creates window + renderer context
PeekMessage:     SDL_PollEvent()         - Poll for pending events
SendMessage:     Direct function call   - Send message via callback to dialog
WM_KEYDOWN/WPARAM wParam/LPARAM lParam → SDL_KeyboardEvent instead
WM_MOUSEFIRST→SDL_MouseButtonEvent/SDL_MouseMotionEvent

## DIALOG SYSTEM APPROACH (most complex part)
Orbiter uses Win32 dialogs (IDD_DIALOG templates, DLGTEMPLATE, DialogBoxIndirectParam).
The MFC-like control library (DlgCtrl) depends on CreateWindow/CreateDialog.

Bridge strategy:
1. Replace WNDCLASS #32770 (Dialog class) with SDL child/sheet windows
2. Controls (button, static text, edit box, listbox) rendered in an overlay/draw loop
3. DialogProc → event dispatch from SDL events to control callbacks
4. Modal dialogs: SDL_CreatePopupWindow with blocking event processing
5. Use imgui's existing Dear ImGui for property pages / debug UI if needed

NOTE: This is a LOT of work (~50 dialog templates). For the initial port, focus on:
- Launchpad dialog (main menu) using simple window + custom controls
- Save/Load scenario dialogs  
- The rest can remain stubbed or use a simpler "click options" workflow until full bridge

Alternatively: Port a minimal DlgCtrl subsystem that maps Win32 control messages to SDL events. 

## BUILD COMMANDS (after all changes complete)
```bash
# macOS build
cmake -B out/macos-debug \
  -DCMAKE_BUILD_TYPE=Debug \
  -DORBITER_BUILD_SDLGPUCLIENT=ON \
  -DORBITER_BUILD_D3D9CLIENT=OFF \
  -S .

cmake --build out/macos-debug -j12

# Linux build (x86_64)
cmake -B out/linux-debug \
  -DCMAKE_BUILD_TYPE=Debug \
  -DORBITER_BUILD_SDLGPUCLIENT=ON \
  -DORBITER_BUILD_D3D9CLIENT=OFF \
  -S .

# Windows ARM64 via Parallels VM
# Requires Visual Studio Build Tools + VCPKG or manual SDL3 install
cmake -B out/win-arm64-debug -G "Visual Studio 17 2022" -A ARM64 ...
```

## IMMEDIATE NEXT STEPS
1. ✅ SdlBridge.h created → now create SdlBridge.cpp implementing all the bridge functions
2. Create src/SdlBridge.cpp (event translation, modal dialogs, keyboard state tracking)  
3. Update Orbiter.cpp: WinMain→main(), add SDL_init() and event loop
4. Update OVP/SDLGPUClient/src/gcCore.cpp → fix class name (SDLGPU_graphicsClient was wrong)
5. Create src/OapiExtension.cpp with RegisterGraphicsClient entry point for the plugin DLL
6. Update all CMakeLists.txt to wire everything together
7. Create platform build presets in CMakePresets.json
8. Fix all references and compile
