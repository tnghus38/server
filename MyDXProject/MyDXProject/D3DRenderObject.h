#pragma once
#include "Transform.h"
#include "D3DObject.h"
#include "Enums.h"

class D3DShaderEffect;

class D3DRenderObject : public D3DObject
{
protected:
	D3DShaderEffect* _shaderEffect;
	DWORD _FVF;

	LPD3DXMESH          g_pMesh = NULL; // Our mesh object in sysmem
	D3DMATERIAL9*       g_pMeshMaterials = NULL; // Materials for our mesh
	LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL; // Textures for our mesh

public: 
	Transform transform;
	int renderQueue;

public:
	D3DRenderObject(LPDIRECT3DDEVICE9 _device, DWORD _fvf);
	virtual ~D3DRenderObject();

	void SetShader(D3DShaderEffect* _effect);

	virtual HRESULT UpdateFrame(float fElapsedTime);
	virtual HRESULT Render() { return S_OK; };
	virtual HRESULT Release() { return S_OK; }
};