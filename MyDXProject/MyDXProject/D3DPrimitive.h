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
		tiger
	};

protected:
	LPDIRECT3DVERTEXBUFFER9			pD3DVertextBuffer;
	LPDIRECT3DINDEXBUFFER9			pD3DIndexBuffer;
	LPDIRECT3DTEXTURE9				pD3DTexture;
	PrimitiveType					primitiveType;
	LPDIRECT3DVERTEXDECLARATION9	pDecl;	// 정점선언
	VOID* vpVertices;

public:
	D3DPrimitive(LPDIRECT3DDEVICE9 _device, PrimitiveType type);
	~D3DPrimitive();

	HRESULT Init();
	HRESULT Render();


private:
	HRESULT SetupTriangle();
	HRESULT SetupQuad();
	HRESULT SetupCube();
	HRESULT SetupPlane();
	HRESULT SetupTiger();
	

	DWORD               g_dwNumMaterials = 0L;
	LPD3DXMESH          g_pMesh = NULL;
	D3DMATERIAL9*       g_pMeshMaterials = NULL;
	
	//렌더오브젝트로 가자
	LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL;
	
};