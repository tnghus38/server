#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <tchar.h>

#include "D3DPrimitive.h"

using namespace std;

typedef struct {
	FLOAT x, y, z;    // 스크린좌표에서 위치
	FLOAT tu, tv;
	DWORD color;
} CUSTOMVERTEX;

D3DPrimitive::D3DPrimitive(LPDIRECT3DDEVICE9 _device, PrimitiveType type) : D3DRenderObject(_device)
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
	switch (primitiveType)
	{
	case Triangle: {
		// 정점선언 오브젝트 생성
		D3DVERTEXELEMENT9 decl[] =
		{
			{ 0, 0,D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,0 },
			{ 0,12,D3DDECLTYPE_FLOAT2,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,0 },
			{ 0,20,D3DDECLTYPE_D3DCOLOR,D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,	0 },
			D3DDECL_END()
		};

		if (FAILED(hr = device->CreateVertexDeclaration(
			decl, &pDecl)))
			return DXTRACE_ERR(TEXT("CreateVertexDeclaration"), hr);


		if (FAILED(SetupTriangle()))
			return E_FAIL;
	}
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
	case tiger:
		if (FAILED(SetupTiger()))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT D3DPrimitive::Render()
{
	 // Clear the backbuffer and the zbuffer
	device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB( 0, 0, 255 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED(device->BeginScene() ) )
    {
        // Setup the world, view, and projection matrices
		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
		device->SetTransform(D3DTS_WORLD, &matWorld);

		D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		device->SetTransform(D3DTS_VIEW, &matView);

		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		device->SetTransform(D3DTS_PROJECTION, &matProj);

        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
            // Set the material and texture for this subset
			device->SetMaterial( &g_pMeshMaterials[i] );
			device->SetTexture( 0, g_pMeshTextures[i] );

            // Draw the mesh subset
            g_pMesh->DrawSubset( i );
        }

        // End the scene
		device->EndScene();
    }

    // Present the backbuffer contents to the display
	device->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}


HRESULT D3DPrimitive::SetupTriangle()
{
	CUSTOMVERTEX vertices[] = {
		//   x,     y,      z,  
		{  0.0f,  sqrtf(3.0f) * 0.5f, 0, 0.5f, 1.0f, D3DCOLOR_RGBA(255,   0,   0, 255) },
		{  1.0f, -sqrtf(3.0f) * 0.5f, 0, 1.0f, 0.0f, D3DCOLOR_RGBA(  0, 255,   0, 255) },
		{ -1.0f, -sqrtf(3.0f) * 0.5f, 0, 0.0f, 0.0f, D3DCOLOR_RGBA(  0,   0, 255, 255) },
	};
	return S_OK;
}

HRESULT D3DPrimitive::SetupQuad()
{
	return S_OK;
}

HRESULT D3DPrimitive::SetupCube()
{
	return S_OK;
}

HRESULT D3DPrimitive::SetupPlane()
{
	
	return S_OK;
}

HRESULT D3DPrimitive::SetupTiger()
{
	LPD3DXBUFFER pD3DXMtrlBuffer;
	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX(L"Tiger.x", D3DXMESH_SYSTEMMEM,
		device, NULL,
		&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
		&g_pMesh)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX(L"..\\Tiger.x", D3DXMESH_SYSTEMMEM,
			device, NULL,
			&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
			&g_pMesh)))
		{
			MessageBox(NULL, L"Could not find tiger.x", L"Meshes.exe", MB_OK);
			return E_FAIL;
		}

	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
	if (g_pMeshMaterials == NULL)
		return E_OUTOFMEMORY;
	g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
	if (g_pMeshTextures == NULL)
		return E_OUTOFMEMORY;

	for (DWORD i = 0; i < g_dwNumMaterials; i++)
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
			if (FAILED(D3DXCreateTextureFromFileA(device,
				d3dxMaterials[i].pTextureFilename,
				&g_pMeshTextures[i])))
			{
				// If texture is not in current folder, try parent folder
				const CHAR* strPrefix = "..\\";
				CHAR strTexture[MAX_PATH];
				strcpy_s(strTexture, MAX_PATH, strPrefix);
				strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFileA(device,
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
