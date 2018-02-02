#include "D3DFramework.h"
#include "Camera.h"

D3DFramework* D3DFramework::instance = NULL;

D3DFramework::D3DFramework()
{
}

D3DFramework::~D3DFramework()
{
}

bool D3DFramework::isDeviceValid()
{
	HRESULT hr = pD3DDevice->TestCooperativeLevel();

	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICENOTRESET)
			return ResetDevice();
	}

	return true;
}

bool D3DFramework::ResetDevice()
{
	// Shader Effect OnLostDevice()

	// Font OnLostDevice()

	// Device Reset()
	if (FAILED(pD3DDevice->Reset(&d3dpp)))
		return false;

	// Font OnResetDevice()

	// Shader Effect OnResetDevice()

	return true;
}

HRESULT D3DFramework::Init(HWND _hWnd, LPCTSTR lpszClass)
{
	hWnd = _hWnd;
	strWindowTitle = lpszClass;

	// Direct 3D 사용
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D9 == NULL)
		return DisplayErrorMsg(D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT);

	HRESULT hr = 0;

	// 디스플레이 모드 조사
	D3DDISPLAYMODE d3ddm;
	if (FAILED(hr = pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return E_FAIL;

	if (FAILED(hr))
	{
		pD3D9->Release();
		pD3D9 = 0;
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

	if (FAILED(hr = pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
		&d3dpp, &pD3DDevice)))
	{
		if (FAILED(hr = pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice)))
		{
			pD3DDevice->Release();
			pD3DDevice = 0;
			return DisplayErrorMsg(D3DAPPERR_NULLREFDEVICE, MSGERR_APPMUSTEXIT);
		}
	}

	D3DCAPS9 caps;

	// Prefer anisotropic texture filtering if it's supported.
	if (SUCCEEDED(pD3DDevice->GetDeviceCaps(&caps)))
	{
		if (caps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY)
			maxAnisotrophy = caps.MaxAnisotropy;
		else
			maxAnisotrophy = 1;
	}

	camera = new Camera();

	return S_OK;
}

HRESULT D3DFramework::UpdateFrame(float fElapsedTime)
{
	// 변경되는 데이터들을 처리합니다.
	return S_OK;
}

HRESULT D3DFramework::Render()
{
	
		// 이 공간에 모든 렌더링 오브젝트를 Draw합니다.
		D3DPrimitive* a= new D3DPrimitive(pD3DDevice, D3DPrimitive::PrimitiveType::tiger);
		if (SUCCEEDED(a->Init()))
		{
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					a->Render();
			}
		}
		
		return S_OK;
}

HRESULT D3DFramework::Release()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT D3DFramework::DisplayErrorMsg(HRESULT hr, DWORD dwType)
{
	static bool s_bFatalErrorReported = false;
	TCHAR strMsg[512];

	// If a fatal error message has already been reported, the app
	// is already shutting down, so don't show more error messages.
	if (s_bFatalErrorReported)
		return hr;

	switch (hr)
	{
	case D3DAPPERR_NODIRECT3D:
		wcscpy(strMsg, TEXT("Could not initialize Direct3D. You may\n")
			TEXT("want to check that the latest version of\n")
			TEXT("DirectX is correctly installed on your\n")
			TEXT("system.  Also make sure that this program\n")
			TEXT("was compiled with header files that match\n")
			TEXT("the installed DirectX DLLs."));
		break;

	case D3DAPPERR_NOCOMPATIBLEDEVICES:
		wcscpy(strMsg, TEXT("Could not find any compatible Direct3D\n")
			TEXT("devices."));
		break;

	case D3DAPPERR_NOWINDOWABLEDEVICES:
		wcscpy(strMsg, TEXT("This sample cannot run in a desktop\n")
			TEXT("window with the current display settings.\n")
			TEXT("Please change your desktop settings to a\n")
			TEXT("16- or 32-bit display mode and re-run this\n")
			TEXT("sample."));
		break;

	case D3DAPPERR_NOHARDWAREDEVICE:
		wcscpy(strMsg, TEXT("No hardware-accelerated Direct3D devices\n")
			TEXT("were found."));
		break;

	case D3DAPPERR_HALNOTCOMPATIBLE:
		wcscpy(strMsg, TEXT("This sample requires functionality that is\n")
			TEXT("not available on your Direct3D hardware\n")
			TEXT("accelerator."));
		break;

	case D3DAPPERR_NOWINDOWEDHAL:
		wcscpy(strMsg, TEXT("Your Direct3D hardware accelerator cannot\n")
			TEXT("render into a window.\n")
			TEXT("Press F2 while the app is running to see a\n")
			TEXT("list of available devices and modes."));
		break;

	case D3DAPPERR_NODESKTOPHAL:
		wcscpy(strMsg, TEXT("Your Direct3D hardware accelerator cannot\n")
			TEXT("render into a window with the current\n")
			TEXT("desktop display settings.\n")
			TEXT("Press F2 while the app is running to see a\n")
			TEXT("list of available devices and modes."));
		break;

	case D3DAPPERR_NOHALTHISMODE:
		wcscpy(strMsg, TEXT("This sample requires functionality that is\n")
			TEXT("not available on your Direct3D hardware\n")
			TEXT("accelerator with the current desktop display\n")
			TEXT("settings.\n")
			TEXT("Press F2 while the app is running to see a\n")
			TEXT("list of available devices and modes."));
		break;

	case D3DAPPERR_MEDIANOTFOUND:
	case HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND):
		wcscpy(strMsg, TEXT("Could not load required media."));
		break;

	case D3DAPPERR_RESETFAILED:
		wcscpy(strMsg, TEXT("Could not reset the Direct3D device."));
		break;

	case D3DAPPERR_NONZEROREFCOUNT:
		wcscpy(strMsg, TEXT("A D3D object has a non-zero reference\n")
			TEXT("count (meaning things were not properly\n")
			TEXT("cleaned up)."));
		break;

	case D3DAPPERR_NULLREFDEVICE:
		wcscpy(strMsg, TEXT("Warning: Nothing will be rendered.\n")
			TEXT("The reference rendering device was selected, but your\n")
			TEXT("computer only has a reduced-functionality reference device\n")
			TEXT("installed.  Install the DirectX SDK to get the full\n")
			TEXT("reference device.\n"));
		break;

	case E_OUTOFMEMORY:
		wcscpy(strMsg, TEXT("Not enough memory."));
		break;

	case D3DERR_OUTOFVIDEOMEMORY:
		wcscpy(strMsg, TEXT("Not enough video memory."));
		break;

	default:
		wcscpy(strMsg, TEXT("Generic application error. Enable\n")
			TEXT("debug output for detailed information."));
	}

	if (MSGERR_APPMUSTEXIT == dwType)
	{
		s_bFatalErrorReported = true;
		wcscat(strMsg, TEXT("\n\nThis sample will now exit."));
		MessageBox(NULL, strMsg, strWindowTitle, MB_ICONERROR | MB_OK);

		// Close the window, which shuts down the app
		if (hWnd)
			SendMessage(hWnd, WM_CLOSE, 0, 0);
	}
	else
	{
		if (MSGWARN_SWITCHEDTOREF == dwType)
			wcscat(strMsg, TEXT("\n\nSwitching to the reference rasterizer,\n")
				TEXT("a software device that implements the entire\n")
				TEXT("Direct3D feature set, but runs very slowly."));
		MessageBox(NULL, strMsg, strWindowTitle, MB_ICONWARNING | MB_OK);
	}

	return E_FAIL;
}

void D3DFramework::ChooseBestMSAAMode(D3DFORMAT backBufferFmt, D3DFORMAT depthStencilFmt, BOOL windowed, D3DMULTISAMPLE_TYPE &type, DWORD &qualityLevels, DWORD &samplesPerPixel)
{
	bool supported = false;

	struct MSAAMode
	{
		D3DMULTISAMPLE_TYPE type;
		DWORD samples;
	}
	multsamplingTypes[15] =
	{
		{ D3DMULTISAMPLE_16_SAMPLES,  16 },
		{ D3DMULTISAMPLE_15_SAMPLES,  15 },
		{ D3DMULTISAMPLE_14_SAMPLES,  14 },
		{ D3DMULTISAMPLE_13_SAMPLES,  13 },
		{ D3DMULTISAMPLE_12_SAMPLES,  12 },
		{ D3DMULTISAMPLE_11_SAMPLES,  11 },
		{ D3DMULTISAMPLE_10_SAMPLES,  10 },
		{ D3DMULTISAMPLE_9_SAMPLES,   9 },
		{ D3DMULTISAMPLE_8_SAMPLES,   8 },
		{ D3DMULTISAMPLE_7_SAMPLES,   7 },
		{ D3DMULTISAMPLE_6_SAMPLES,   6 },
		{ D3DMULTISAMPLE_5_SAMPLES,   5 },
		{ D3DMULTISAMPLE_4_SAMPLES,   4 },
		{ D3DMULTISAMPLE_3_SAMPLES,   3 },
		{ D3DMULTISAMPLE_2_SAMPLES,   2 }
	};

	for (int i = 0; i < 15; ++i)
	{
		type = multsamplingTypes[i].type;

		supported = MSAAModeSupported(type, backBufferFmt, depthStencilFmt,
			windowed, qualityLevels);

		if (supported)
		{
			samplesPerPixel = multsamplingTypes[i].samples;
			return;
		}
	}

	type = D3DMULTISAMPLE_NONE;
	qualityLevels = 0;
	samplesPerPixel = 1;
}

bool D3DFramework::MSAAModeSupported(D3DMULTISAMPLE_TYPE type, D3DFORMAT backBufferFmt,
	D3DFORMAT depthStencilFmt, BOOL windowed,
	DWORD &qualityLevels)
{
	DWORD backBufferQualityLevels = 0;
	DWORD depthStencilQualityLevels = 0;

	HRESULT hr = pD3D9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, backBufferFmt, windowed, type,
		&backBufferQualityLevels);

	if (SUCCEEDED(hr))
	{
		hr = pD3D9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, depthStencilFmt, windowed, type,
			&depthStencilQualityLevels);

		if (SUCCEEDED(hr))
		{
			if (backBufferQualityLevels == depthStencilQualityLevels)
			{
				// The valid range is between zero and one less than the level
				// returned by IDirect3D9::CheckDeviceMultiSampleType().

				if (backBufferQualityLevels > 0)
					qualityLevels = backBufferQualityLevels - 1;
				else
					qualityLevels = backBufferQualityLevels;

				return true;
			}
		}
	}

	return false;
}