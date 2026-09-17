// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for d3dtypes.h on macOS/Linux

#pragma once

#ifdef ORBITER_BUILD_SDLGPUCLIENT

#include "platform_sdl.h"

// Direct3D types
typedef FLOAT           D3DVALUE;
typedef D3DVALUE        *LPD3DVALUE;

#ifndef __D3DCOLOR_DEFINED__
#define __D3DCOLOR_DEFINED__
typedef struct { float r, g, b, a; } D3DCOLOR, *LPD3DCOLOR;
#endif

#ifndef __D3DCOLORVALUE_DEFINED__
typedef struct {
    D3DVALUE r, g, b, a;
} D3DCOLORVALUE, *LPD3DCOLORVALUE;
#define __D3DCOLORVALUE_DEFINED__
#endif


#ifndef __D3DMATRIX_DEFINED__
typedef struct {
    D3DVALUE mat[4][4];
} D3DMATRIX, *LPD3DMATRIX;
#endif

typedef struct {
    D3DVECTOR vCenter;
    D3DVALUE  vRadius;
} D3DRECT;

// D3D7 types
class IDirect3DVertexBuffer7 {
public:
    virtual ~IDirect3DVertexBuffer7() {}
    virtual HRESULT Release() { return S_OK; }
};
#ifndef __IDIRECT3DDEVICE7_CLASS__
#ifndef __IDIRECT3DDEVICE7_CLASS__
class IDirect3DDevice7 {
public:
    virtual ~IDirect3DDevice7() {}
#endif
    virtual HRESULT STDMETHODCALLTYPE SetRenderState(DWORD State, DWORD dwValue) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE GetRenderState(DWORD State, DWORD *pValue) { if(pValue) *pValue = 0; return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, void* pTexture) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawPrimitive(UINT PrimitiveType, UINT VertexFormat, const void* pVertices, UINT VertexCount, const void* pIndices) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(UINT PrimitiveType, UINT VertexFormat, const void* pVertices, UINT VertexCount, const void* pIndices, UINT IndexCount, UINT Flags) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetFVF(DWORD VertexFormat) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE SetStreamSource(UINT StreamNum, void* pDataStream, UINT DataStride) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE Clear(DWORD Count, const void* pRects, DWORD Flags, DWORD Color, float Z, DWORD Stencil) { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE BeginScene() { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE EndScene() { return S_OK; }
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwCaps;
    DWORD dwCaps2;
    DWORD dwCKeyCaps;
    DWORD dwDDCaps;
    DWORD dwHWCaps;
    DWORD dwFXCaps;
    DWORD dwFXHWCaps;
    DWORD dwFXActiveCaps;
    DWORD dwMaxSecBufSize;
    DWORD dwMaxBuffers;
    DWORD dwMaxVCPUs;
    DWORD dwMaxVideoMem;
    DWORD dwDeviceVidMem;
    DWORD dwSystemVidMem;
    DWORD dwMaxZBufferWidth;
    DWORD dwMaxZBufferHeight;
    DWORD dwMaxOffScrWidth;
    DWORD dwMaxOffScrHeight;
    DWORD dwMinOverlayWidth;
    DWORD dwMinOverlayHeight;
    DWORD dwMaxOverlayWidth;
    DWORD dwMaxOverlayHeight;
    DWORD dwStrideAlignCaps;
} DDCAPS;

typedef struct {
    DWORD dwSize;
    DWORD dwFlags;
    DDCAPS Caps;
    GUID DeviceGUID;
    DWORD DriverVersion;
    DWORD DXVersion;
    DWORD dwReserved;
    DWORD dwVertexProcessingCaps;
    DWORD dwNumSimultaneousRasters;
    DWORD dwVertexShaderVersion;
    DWORD dwMaxVertexShaderConst;
    DWORD dwPixelShaderVersion;
    DWORD dwMaxPixelShaderValue;
} D3DDEVICEDESC7;

typedef struct {
    D3DVALUE          rf, rg, rb, rh;
    D3DCOLOR          diffuse;
    D3DCOLOR          specular;
    D3DVALUE          dist;
    DWORD             specularTex;
    D3DCOLOR          ambient;
    D3DVALUE          power;
    D3DCOLOR          emissive;
} D3DMATERIAL7, *LPD3DMATERIAL7;

#endif // ORBITER_BUILD_SDLGPUCLIENT
#endif
