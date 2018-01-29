#pragma once

#include<d3d9.h>
#include<d3dx9.h>
//ΩÃ±€≈Ê¿∏∑Œ∏∏µÎ 
class D3DFramework
{
	static D3DFramework* instance;

	LPDIRECT3D9 d3d9=NULL;
	LPDIRECT3DDEVICE9 d3dDevice9=NULL;
	HWND hWnd = NULL;
	D3DFramework();
public:
	static D3DFramework* Instance()
	{
		if (instance == NULL)
		{
			instance = new D3DFramework();
		}
		return instance;
	}
	~D3DFramework();
	HRESULT Init(HWND hWnd);
	HRESULT updateFrame();
	HRESULT Render();
	HRESULT Release();
};