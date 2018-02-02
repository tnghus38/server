#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include "D3DPrimitive.h"
//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d

using namespace std;

class Camera;

class D3DFramework
{
private:
	static D3DFramework* instance;

	LPDIRECT3D9					pD3D9 = NULL;
	LPDIRECT3DDEVICE9			pD3DDevice = NULL;
	HWND						hWnd = NULL;
	LPCTSTR						strWindowTitle = NULL;
	Camera*						camera = NULL;

	D3DPRESENT_PARAMETERS		d3dpp;

	DWORD						msaaSamples;
	DWORD						maxAnisotrophy;

	map<string, LPD3DXEFFECT>	shaders;

public:
	static D3DFramework* Instance()
	{
		if (instance == NULL)
		{
			instance = new D3DFramework();
		}
		return instance;
	}

	D3DFramework();
	~D3DFramework();

	bool isDeviceValid();
	bool ResetDevice();

	HRESULT Init(HWND hWnd, LPCTSTR lpszClass);
	HRESULT UpdateFrame(float fElapsedTime);
	HRESULT Render();
	HRESULT Release();

	// Camera Instance
	Camera* GetCamera() { return camera; }

	// DisplayError 
	HRESULT DisplayErrorMsg(HRESULT hr, DWORD dwType);

	bool MSAAModeSupported (
		D3DMULTISAMPLE_TYPE type, 
		D3DFORMAT backBufferFmt,
		D3DFORMAT depthStencilFmt, 
		BOOL windowed,
		DWORD &qualityLevels
	);

	void ChooseBestMSAAMode (
		D3DFORMAT backBufferFmt, 
		D3DFORMAT depthStencilFmt,
		BOOL windowed, 
		D3DMULTISAMPLE_TYPE &type,
		DWORD &qualityLevels, 
		DWORD &samplesPerPixel
	);
};

