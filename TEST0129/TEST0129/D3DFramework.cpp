#include "D3DFramework.h"

D3DFramework* D3DFramework::instance = NULL;

D3DFramework::D3DFramework()
{
}

D3DFramework::~D3DFramework()
{
}

HRESULT D3DFramework::Init(HWND hWnd)
{

	if (d3d9 == NULL)
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION); //시스템을 현재 깔린 다이랙트 버전으로 가져오겠다 

	if (d3d9 == NULL)  // 시스템을 못가져오면 못만듬 
		return E_FAIL;


	HRESULT hr = 0;

	// 디스플레이 모드 조사
	D3DDISPLAYMODE d3ddm;
	if (FAILED(hr = d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return E_FAIL;

	if (FAILED(hr))
	{
		d3d9->Release();
		d3d9 = 0;
		return false;
	}
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferWidth = 0;
	d3dpp.BackBufferHeight = 0;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;

	// VSync Enabled
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//// VSync Disabled
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if (FAILED(hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
		&d3dpp, &d3dDevice9)))
	{
		if (FAILED(hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice9)))
		{
			d3dDevice9->Release();
			d3dDevice9 = 0;
			//return DisplayErrorMsg(D3DAPPERR_NULLREFDEVICE, MSGERR_APPMUSTEXIT);
		}
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);//렌더링 지원되는 속성정보를 가지고옴 
	int vertexProcessing;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) //D3DDEVCAPS_HWTRANSFORMANDLIGHT이기능이 가능한지 확인 
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING; //하드에서 버텍스 처리를 할수있을때 
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING; // 하드에서 버텍스 처리를 못할때 소프트웨어에서 처리 

	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, //하드웨어 가속을 씀
		hWnd, // 현재 윈도우 핸들 
		vertexProcessing, // 버텍스 처리 방식 
		&d3dpp, // 
		&d3dDevice9)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT D3DFramework::updateFrame(float time)
{
	return S_OK;
}

HRESULT D3DFramework::Render()
{
	if (d3dDevice9 == NULL)
		return E_FAIL;

	d3dDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);
	//색         깊이0~1   

	// Render 시작
	if (SUCCEEDED(d3dDevice9->BeginScene()))
	{
		// Render 하기

		// Render 끝
		d3dDevice9->EndScene();
	}

	// Backbuffer와 FrontBuffer의 Swap
	d3dDevice9->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

HRESULT D3DFramework::Release()
{
	return S_OK;
}

HRESULT D3DFramework::DisplayErrorMsg(HRESULT hr, DWORD dwType)
{
	return E_NOTIMPL;
}

bool D3DFramework::isDevicevalid()
{
	HRESULT hr = d3dDevice9->TestCooperativeLevel();

	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICENOTRESET)
			return ResetDevice();
	}
	return true;
}

bool D3DFramework::ResetDevice()
{
	//shader Effect OnLostDevice()

	//Font OnLostDevice()

	//Device Reset()
	if (FAILED(d3dDevice9->Reset(&d3dpp)))
	{
		return false;
	}

	//Font OnResetDevice()

	//shader Effect OnResetDevice()


	return true;
}
