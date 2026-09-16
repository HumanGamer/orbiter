// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for zmouse.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

// ZMouse stub - Windows mouse handling
class ZMouse {
public:
    ZMouse() {}
    ~ZMouse() {}
    void Create() {}
    void Destroy() {}
    bool GetState(int& x, int& y, int& buttons) { x = 0; y = 0; buttons = 0; return true; }
    void SetPos(int x, int y) {}
    void Clip(int x1, int y1, int x2, int y2) {}
    void Uncip() {}
};

#endif // ORBITER_BUILD_SDLGPUCLIENT
