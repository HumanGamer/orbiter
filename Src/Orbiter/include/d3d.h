// Copyright (c) Martin Schweiger
// Licensed under the MIT License
//
// Cross-platform compatibility layer for d3d.h on macOS/Linux

#pragma once

#include "platform_sdl.h"

class IDirect3DDevice7;

// Direct3D types (stubs)
typedef void*           LPDIRECT3D7;

class IDirect3DDevice7 {
public:
    HRESULT SetRenderState(DWORD State, DWORD Value) { return D3D_OK; }
    HRESULT GetRenderState(DWORD State, DWORD* pValue) { if(pValue) *pValue = 0; return D3D_OK; }
    HRESULT DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, DWORD FVF, const void* pVertexStreamZeroData, UINT VertexCount, const void* pIndexData) { return D3D_OK; }
    HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, const void* pVertexData, UINT VertexCount, const void* pIndexData, UINT IndexCount) { return D3D_OK; }
    HRESULT SetTexture(DWORD Stage, void* pTexture) { (void)Stage; (void)pTexture; return D3D_OK; }
};
typedef IDirect3DDevice7* LPDIRECT3DDEVICE7;
typedef void*           LPDIRECT3D8;
typedef void*           LPDIRECT3DDEVICE8;
typedef void*           LPDIRECT3DVERTEXBUFFER8;
typedef void*           LPDIRECT3DINDEXBUFFER8;

#define D3D_OK              0
#define D3DERR_WRONGSTATE   0x887608E4
#define D3DERR_NOTAVAILABLE 0x887608BC

#ifndef __D3DCOLOR_DEFINED__
#define __D3DCOLOR_DEFINED__
typedef struct { float r, g, b, a; } D3DCOLOR;
#endif

#ifndef __D3DVERTEX_DEFINED__
#define __D3DVERTEX_DEFINED__
typedef struct {
    FLOAT x, y, z, rhw;
    DWORD color;
    FLOAT tu, tv;
    FLOAT nx, ny, nz;
} D3DVERTEX;
#endif

#ifndef __D3DMATRIX_DEFINED__
#define __D3DMATRIX_DEFINED__
typedef struct {
    FLOAT _11, _12, _13, _14;
    FLOAT _21, _22, _23, _24;
    FLOAT _31, _32, _33, _34;
    FLOAT _41, _42, _43, _44;
} D3DMATRIX;
#endif

#define D3DFVF_XYZ          0x001
#define D3DFVF_RHW          0x002
#define D3DFVF_NORMAL       0x010
#define D3DFVF_PSIZE        0x020
#define D3DFVF_DIFFUSE      0x040
#define D3DFVF_SPECULAR     0x080
#define D3DFVF_TEX1         0x100
#define D3DFVF_TEX2         0x200
#define D3DFVF_TEX3         0x400
#define D3DFVF_TEX4         0x800
#define D3DFVF_TEXCOORDSIZE2(idx)  (0 << (idx * 4))
#define D3DFVF_TEXCOORDSIZE3(idx)  (1 << (idx * 4))
#define D3DFVF_TEXCOORDSIZE4(idx)  (2 << (idx * 4))
#define D3DFVF_VERTEX       (D3DFVF_XYZ | D3DFVF_RHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// Direct3D primitive types
#define D3DPT_POINTLIST     0
#define D3DPT_LINELIST      1
#define D3DPT_LINESTRIP     2
#define D3DPT_TRIANGLELIST  4
#define D3DPT_TRIANGLESTRIP 5
#define D3DPT_TRIANGLEFAN   6
typedef DWORD D3DPRIMITIVETYPE;

// Direct3D render states
#define D3DRENDERSTATE_ALPHABLENDENABLE         30
#define D3DRENDERSTATE_AMBIENT                  4
#define D3DRENDERSTATE_SRCBLEND                 14
#define D3DRENDERSTATE_DESTBLEND                15

// Direct3D blend factors
#define D3DBLEND_ZERO           1
#define D3DBLEND_ONE            2
#define D3DBLEND_SRCCOLOR       3
#define D3DBLEND_INVSRCCOLOR    4
#define D3DBLEND_SRCALPHA       5
#define D3DBLEND_INVSRCALPHA    6
