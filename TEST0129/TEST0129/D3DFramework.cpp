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

	D3DPRESENT_PARAMETERS d3dPP;
	ZeroMemory(&d3dPP, sizeof(d3dPP));
	d3dPP.Windowed = TRUE;
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD; //백버퍼와 프론트버퍼를 바꿔줄때 옵션  
											  //DISCARD는 플립방식이나 카피방식으로 할때 프론트버퍼에 원래있던(이전 프레임)을 보장 하는 작업이 비싼작업이라 이를 보장하지 않고 작업할때쓰는방식  
	d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;//window일땐 OS에 백버퍼 모델을 따라간다 

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
		&d3dPP, // 
		&d3dDevice9)))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT D3DFramework::updateFrame()
{
	return S_OK;
}

HRESULT D3DFramework::Render()
{
	if (d3dDevice9 == NULL)
		return E_FAIL;

	d3dDevice9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 0), 1.0f, 0);
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
