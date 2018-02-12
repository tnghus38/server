#include <windows.h>
#include <dxerr.h>
#include <tchar.h>

#include "D3DPrimitive.h"

using namespace std;

typedef struct {
	FLOAT x, y, z;
	DWORD color;
} PRIMITIVE_VERTEX;

D3DPrimitive::D3DPrimitive(LPDIRECT3DDEVICE9 _device, DWORD _fvf, PrimitiveType type) 
	: D3DRenderObject(_device, _fvf)
{
	// 기본 도형 타입
	primitiveType = type;
}

D3DPrimitive::~D3DPrimitive()
{
}

HRESULT D3DPrimitive::Init()
{
	HRESULT hr;

	// 정점선언 오브젝트 생성
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0,D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,0 },
		{ 0,12,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,	0 },
		D3DDECL_END()
	};

	if (FAILED(hr = _device->CreateVertexDeclaration(
		decl, &pDecl)))
		return DXTRACE_ERR( TEXT("CreateVertexDeclaration"), hr);

	switch (primitiveType)
	{
	case Triangle:

		if (FAILED(SetupTriangle()))
			return E_FAIL;
		break;
	case Quad:
		if (FAILED(SetupQuad()))
			return E_FAIL;
		break;
	case Cube:
		if (FAILED(SetupCube()))
			return E_FAIL;
		break;
	case Plane:
		if (FAILED(SetupPlane()))
			return E_FAIL;
		break;
	case Mesh:

		if (FAILED(SetupMesh()))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT D3DPrimitive::Render()
{

	// RenderState
	_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// Transform
	_device->SetTransform(D3DTS_WORLD, &(transform.worldMat));

	// Vertex Buffer 및 Vertex정의 세팅
	_device->SetVertexDeclaration(pDecl);

	switch (primitiveType)
	{
	case Triangle:

		_device->SetStreamSource(0, pD3DVertextBuffer, 0, sizeof(PRIMITIVE_VERTEX));
		if (FAILED(DrawTriangle()))
			return E_FAIL;
		break;
	case Quad:

		_device->SetStreamSource(0, pD3DVertextBuffer, 0, sizeof(PRIMITIVE_VERTEX));
		if (FAILED(DrawQuad()))
			return E_FAIL;
		break;
	case Cube:

		_device->SetStreamSource(0, pD3DVertextBuffer, 0, sizeof(PRIMITIVE_VERTEX));
		if (FAILED(DrawCube()))
			return E_FAIL;
		break;
	case Plane:

		_device->SetStreamSource(0, pD3DVertextBuffer, 0, sizeof(PRIMITIVE_VERTEX));
		if (FAILED(DrawPlane()))
			return E_FAIL;
		break;
	case Mesh:
		if (FAILED(DrawMesh()))
			return E_FAIL;
		break;

	}

	D3DRenderObject::Render();

	return S_OK;
}

HRESULT D3DPrimitive::SetupTriangle()
{
	PRIMITIVE_VERTEX vertices[] = {
		//   x,     y,      z,  
		{  0.0f,  sqrtf(3.0f) * 0.5f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 0.5f) },
		{  1.0f, -sqrtf(3.0f) * 0.5f,	0, D3DCOLOR_COLORVALUE(0.0f, 1.0f, 0.0f, 0.5f) },
		{ -1.0f, -sqrtf(3.0f) * 0.5f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
	};

	if (_device != NULL)
	{
		// 정점 버퍼 생성
		if (FAILED(_device->CreateVertexBuffer(
			3 * sizeof(PRIMITIVE_VERTEX),
			0, _FVF,
			D3DPOOL_DEFAULT,
			&pD3DVertextBuffer, NULL)))
		{
			return E_FAIL;
		}

		// 정점 버퍼에 정보 저장
		VOID* pVertices;
		if (FAILED(pD3DVertextBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return E_FAIL;
		memcpy(pVertices, vertices, sizeof(vertices));
		pD3DVertextBuffer->Unlock();
	}
	return S_OK;
}

HRESULT D3DPrimitive::SetupQuad()
{
	PRIMITIVE_VERTEX vertices[] = {
		//   x,     y,      z,  
		{ 1.0f,  1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 0.5f) },
		{ 1.0f, -1.0f,	0, D3DCOLOR_COLORVALUE(0.0f, 1.0f, 0.0f, 0.5f) },
		{ -1.0f, 1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
		{ -1.0f, -1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
	};

	if (_device != NULL)
	{
		// 정점 버퍼 생성
		if (FAILED(_device->CreateVertexBuffer(
			4 * sizeof(PRIMITIVE_VERTEX),
			0, _FVF,
			D3DPOOL_DEFAULT,
			&pD3DVertextBuffer, NULL)))
		{
			return E_FAIL;
		}

		// 정점 버퍼에 정보 저장
		VOID* pVertices;
		if (FAILED(pD3DVertextBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return E_FAIL;
		memcpy(pVertices, vertices, sizeof(vertices));
		pD3DVertextBuffer->Unlock();
	}
	return S_OK;
}

HRESULT D3DPrimitive::SetupCube()
{
	PRIMITIVE_VERTEX vertices[] = {
		//   x,     y,      z,  
		{ -1.0f, -1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
	{ -1.0f, 1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },

	{ 1.0f, -1.0f,	0, D3DCOLOR_COLORVALUE(0.0f, 1.0f, 0.0f, 0.5f) },
	{ 1.0f,  1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 0.5f) },
	{ 1.0f, -1.0f,	-2, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },

	{ 1.0f, 1.0f,	-2, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 0.5f) },
	{ -1.0f, -1.0f,	-2, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 0.5f) },
	{ -1.0f, 1.0f,	-2, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },

	{ -1.0f, -1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
	{ -1.0f, 1.0f,	-2, D3DCOLOR_COLORVALUE(0.0f, 1.0f, 1.0f, 0.5f) },
	{ -1.0f, 1.0f,	0, D3DCOLOR_COLORVALUE(0.5f, 0.5f, 1.0f, 0.5f) },

	{ 1.0f, 1.0f,	-2, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 0.5f) },
	{ 1.0f,  1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 0.5f) },
	{ -1.0f,  -1.0f,	-2, D3DCOLOR_COLORVALUE(0.0f, 1.0f, 0.0f, 0.5f) },
	{ -1.0f,  -1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
	{ 1.0f,  -1.0f,	-2, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 0.5f) },
	{ 1.0f,  -1.0f,	0, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 0.5f) },

	};

	if (_device != NULL)
	{
		// 정점 버퍼 생성
		if (FAILED(_device->CreateVertexBuffer(
			6 * sizeof(PRIMITIVE_VERTEX),
			0, _FVF,
			D3DPOOL_DEFAULT,
			&pD3DVertextBuffer, NULL)))
		{
			return E_FAIL;
		}

		// 정점 버퍼에 정보 저장
		VOID* pVertices;
		if (FAILED(pD3DVertextBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return E_FAIL;
		memcpy(pVertices, vertices, sizeof(vertices));
		pD3DVertextBuffer->Unlock();
	}
	return S_OK;
}

HRESULT D3DPrimitive::SetupPlane()
{
	PRIMITIVE_VERTEX vertices[] = {
		//   x,     y,      z,  
		{ 1.0f,  1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 0.5f) },
	{ 1.0f, -1.0f,	0, D3DCOLOR_COLORVALUE(0.0f, 1.0f, 0.0f, 0.5f) },
	{ -1.0f, 1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
	{ -1.0f, -1.0f,	0, D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 0.5f) },
	};

	if (_device != NULL)
	{
		// 정점 버퍼 생성
		if (FAILED(_device->CreateVertexBuffer(
			4 * sizeof(PRIMITIVE_VERTEX),
			0, _FVF,
			D3DPOOL_DEFAULT,
			&pD3DVertextBuffer, NULL)))
		{
			return E_FAIL;
		}

		// 정점 버퍼에 정보 저장
		VOID* pVertices;
		if (FAILED(pD3DVertextBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return E_FAIL;
		memcpy(pVertices, vertices, sizeof(vertices));
		pD3DVertextBuffer->Unlock();
	}
	return S_OK;
}

HRESULT D3DPrimitive::SetupMesh()
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX(L"Tiger.x", D3DXMESH_SYSTEMMEM,
		_device, NULL,
		&pD3DXMtrlBuffer, NULL, &_FVF,
		&g_pMesh)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX(L"..\\Tiger.x", D3DXMESH_SYSTEMMEM,
			_device, NULL,
			&pD3DXMtrlBuffer, NULL, &_FVF,
			&g_pMesh)))
		{
			MessageBox(NULL, L"Could not find tiger.x", L"Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pMeshMaterials = new D3DMATERIAL9[_FVF];
	if (g_pMeshMaterials == NULL)
		return E_OUTOFMEMORY;
	g_pMeshTextures = new LPDIRECT3DTEXTURE9[_FVF];
	if (g_pMeshTextures == NULL)
		return E_OUTOFMEMORY;

	for (DWORD i = 0; i < _FVF; i++)
	{
		// Copy the material
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

		g_pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFileA(_device,
				d3dxMaterials[i].pTextureFilename,
				&g_pMeshTextures[i])))
			{
				// If texture is not in current folder, try parent folder
				const CHAR* strPrefix = "..";
					CHAR strTexture[MAX_PATH];
				strcpy_s(strTexture, MAX_PATH, strPrefix);
				strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFileA(_device,
					strTexture,
					&g_pMeshTextures[i])))
				{
					MessageBox(NULL, L"Could not find texture map", L"Meshes.exe", MB_OK);
				}
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();

	return S_OK;
}

HRESULT D3DPrimitive::DrawTriangle()
{
	//알파 블렌딩 
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//뒷면 컬링   D3DCULL_CCW가 디폴트 
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	return S_OK;;
}

HRESULT D3DPrimitive::DrawQuad()
{
	//알파 블렌딩 
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//뒷면 컬링   D3DCULL_CCW가 디폴트 
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	return S_OK;
}

HRESULT D3DPrimitive::DrawCube()
{ 
	//알파 블렌딩 
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//뒷면 컬링   D3DCULL_CCW가 디폴트 
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,15);
	return S_OK;
}

HRESULT D3DPrimitive::DrawPlane()
{
	//알파 블렌딩 
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//뒷면 컬링   D3DCULL_CCW가 디폴트 
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	return S_OK;
}

HRESULT D3DPrimitive::DrawMesh()
{ 

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		
		for (DWORD i = 0; i < _FVF; i++)
		{
			// Set the material and texture for this subset
			_device->SetMaterial(&g_pMeshMaterials[i]);
			_device->SetTexture(0, g_pMeshTextures[i]);

			// Draw the mesh subset
			g_pMesh->DrawSubset(i);
		}

		
	return S_OK;
}

HRESULT D3DPrimitive::Release()
{
	SAFE_RELEASE(pD3DVertextBuffer);
	return S_OK;
}