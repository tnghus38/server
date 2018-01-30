#pragma once

#include<d3d9.h>
#include<d3dx9.h>
//�̱������θ��� 
class D3DFramework
{
	static D3DFramework* instance;

	LPDIRECT3D9 d3d9=NULL;
	LPDIRECT3DDEVICE9 d3dDevice9=NULL;
	HWND hWnd = NULL;
	LPCTSTR strWindowTitle = NULL;
	//Camera * camera = NULL;
	D3DPRESENT_PARAMETERS  d3dpp;

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
	HRESULT updateFrame(float time);
	HRESULT Render();
	HRESULT Release();

	//�����Ž��� 
	HRESULT DisplayErrorMsg(HRESULT hr, DWORD dwType);

	bool isDevicevalid();
	bool ResetDevice();
};