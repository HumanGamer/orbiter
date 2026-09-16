# OpenOrbiter Build System

## Prerequisites

### Windows
- Visual Studio 2019+ with CMake tools
- DirectX SDK (June 2010) for D3D9 client
- SDL3 and SDL3_shadercross via vcpkg: `vcpkg install sdl3 sdl3-shadercross`

### macOS
- Xcode Command Line Tools
- Homebrew: `brew install sdl3`
- CMake via homebrew: `brew install cmake ninja`

### Linux
- System package manager (SDL3 + SDL3_shadercross must be installed)
- Ninja build system: `apt install ninja-build` or equivalent

## Building SDLGPUClient (Cross-Platform)

This is the recommended build target for macOS and Linux. It uses SDL_gpu for rendering instead of DirectX.

### macOS ARM64 Build
```bash
cmake -S . -B out/build/macos-arm64-debug \
  -DCMAKE_TOOLCHAIN_FILE=$(brew --prefix)/share/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DORBITER_BUILD_D3D9CLIENT=OFF \
  -DORBITER_BUILD_SDLGPUCLIENT=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -G Ninja

cmake --build out/build/macos-arm64-debug --config Debug
```

### Linux x64 Build
```bash
cmake -S . -B out/build/linux-x64-debug \
  -DORBITER_BUILD_D3D9CLIENT=OFF \
  -DORBITER_BUILD_SDLGPUCLIENT=ON \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -G Ninja

cmake --build out/build/linux-x64-debug --config Debug
```

### Windows with SDLGPUClient
```bash
cmake -S . -B out/build/windows-sdlgpu \
  -DCMAKE_TOOLCHAIN_FILE=$(vcpkg root)/scripts/buildsystems/vcpkg.cmake \
  -DORBITER_BUILD_D3D9CLIENT=OFF \
  -DORBITER_BUILD_SDLGPUCLIENT=ON \
  -G "Visual Studio 16"

cmake --build out/build/windows-sdlgpu --config Release
```

## Building D3D9 Client (Windows Only)
```bash
cmake -S . -B out/build/windows-d3d9 \
  -DCMAKE_TOOLCHAIN_FILE=$(vcpkg root)/scripts/buildsystems/vcpkg.cmake \
  -G "Visual Studio 16"

cmake --build out/build/windows-d3d9 --config Release
```

## Using CMakePresets.json

Orbiter provides CMake presets for all platforms:

```bash
# List available presets
cmake --list-presets

# Configure macOS ARM64 Debug
cmake -S . -B out/build --preset=macos-arm64-debug

# Build with presets
cmake --build out/build --config Release
```

## Runtime Dependencies

SDLGPUClient builds require the following runtime libraries on the target machine:
- `sdl3.dll` (Windows) or `libSDL3.dylib` (macOS) or `libSDL3.so` (Linux)
- `shadercross.dll/so/dylib` if using shader cross-compilation features

On macOS, install via homebrew which copies to `/opt/homebrew/lib`.
On Linux, the system SDL3 library provides these.
On Windows, copy from your vcpkg triplet folder (e.g., `vcpkg/installed/x64-windows/bin`).

## Platform-Specific Notes

### macOS
- Requires Xcode 14+ targeting macOS 13.0+ deployment target
- SDL3 must be installed via homebrew (`brew install sdl3`)
- Signing codesigning required for full functionality on Gatekeeper-enabled systems

### Linux
- `ORBITER_BUILD_D3D9CLIENT` is disabled by default (via linux-winegcc preset) since DirectX 9 requires WINE compatibility layer
- Ensure `SDL3-devel` and `SDL3_shadercross-devel` packages are installed via package manager
- X11 or Wayland support required for window rendering

### Windows
- DirectX SDK (June 2010) must be installed if building D3D9 client
- For SDLGPUClient builds, only SDL3 + shadercross needed - no DirectX dependency
- LargeAddressAware linker flag is set by default for x86 builds
