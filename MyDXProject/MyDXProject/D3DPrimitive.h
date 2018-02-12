#pragma once
#include "D3DRenderObject.h"

class D3DPrimitive : public D3DRenderObject
{
public:
	enum PrimitiveType
	{
		Triangle,
		Quad,
		Cube,
		Plane,
		Mesh
	};

protected:
	LPDIRECT3DVERTEXBUFFER9			pD3DVertextBuffer;
	LPDIRECT3DINDEXBUFFER9			pD3DIndexBuffer;
	LPDIRECT3DTEXTURE9				pD3DTexture;
	PrimitiveType					primitiveType;
	LPDIRECT3DVERTEXDECLARATION9	pDecl;	// 정점선언

public:
	D3DPrimitive(LPDIRECT3DDEVICE9 _device, DWORD _fvf, PrimitiveType type);
	~D3DPrimitive();

	HRESULT Init();
	HRESULT Render();
	HRESULT Release();

private:
	HRESULT SetupTriangle();
	HRESULT SetupQuad();
	HRESULT SetupCube();
	HRESULT SetupPlane();
	HRESULT SetupMesh();

	HRESULT DrawTriangle();
	HRESULT DrawQuad();
	HRESULT DrawCube();
	HRESULT DrawPlane();
	HRESULT DrawMesh();
};