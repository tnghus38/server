#include <vector>
#include "D3DFramework.h"
#include "Camera.h"
#include "input.h"

D3DFramework* D3DFramework::instance = NULL;

D3DFramework::D3DFramework()
{
	shaderFileNames.push_back(TEXT("hlsl.fx"));
}

D3DFramework::~D3DFramework()
{
	Release();
}

void D3DFramework::ToggleFullScreen()
{
	isFullScreen = !isFullScreen;

	ResetDevice();
}

bool D3DFramework::isDeviceValid()
{
	HRESULT hr = pD3DDevice9->TestCooperativeLevel();

	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICENOTRESET)
			return ResetDevice();
	}

	return true;
}

bool D3DFramework::ResetDevice()
{
	// Device Reset 

	static DWORD savedExStyle;
	static DWORD savedStyle;
	static RECT rcSaved;

	if (isFullScreen)
	{
		// Moving to full screen mode.

		savedExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
		savedStyle = GetWindowLong(hWnd, GWL_STYLE);
		GetWindowRect(hWnd, &rcSaved);

		SetWindowLong(hWnd, GWL_EXSTYLE, 0);
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		windowWidth = GetSystemMetrics(SM_CXSCREEN);
		windowHeight = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0,
			windowWidth, windowHeight, SWP_SHOWWINDOW);

		// Update presentation parameters.

		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = windowWidth;
		d3dpp.BackBufferHeight = windowHeight;

		if (enableVerticalSync)
		{
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_DEFAULT;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		else
		{
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_IMMEDIATE;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}
	}
	else
	{
		// Moving back to windowed mode.

		SetWindowLong(hWnd, GWL_EXSTYLE, savedExStyle);
		SetWindowLong(hWnd, GWL_STYLE, savedStyle);
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		windowWidth = rcSaved.right - rcSaved.left;
		windowHeight = rcSaved.bottom - rcSaved.top;

		SetWindowPos(hWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
			windowWidth, windowHeight, SWP_SHOWWINDOW);

		// Update presentation parameters.

		d3dpp.Windowed = TRUE;
		d3dpp.BackBufferWidth = windowWidth;
		d3dpp.BackBufferHeight = windowHeight;
		d3dpp.FullScreen_RefreshRateInHz = 0;

		if (enableVerticalSync)
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		else
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// Viewport has changed in size. Rebuild the camera's projection matrix.
	camera->perspective(CAMERA_FOVX,
		static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
		CAMERA_ZNEAR, CAMERA_ZFAR);

	// Shader Effect OnLostDevice()

	// Font OnLostDevice()

	// Device Reset()
	if (FAILED(pD3DDevice9->Reset(&d3dpp)))
		return false;

	// Font OnResetDevice()

	// Shader Effect OnResetDevice()

	// RenderObjects OnResetDevice();
	for (list<D3DRenderObject*>::iterator iter = renderObjects.begin(); iter != renderObjects.end(); iter++)
	{
		(*iter)->Reset();
	}

	return true;
}

bool Comp(const D3DRenderObject* pObject1, const D3DRenderObject* pObject2)
{
	if (pObject1->renderQueue < pObject2->renderQueue)
	{
		return TRUE;
	}
	else
	{
		if (pObject1->renderQueue != (int)(RenderObject::AlphaTest) && pObject1->renderQueue != (int)(RenderObject::Transparent))
			return FALSE;

		if (pObject1->renderQueue == pObject2->renderQueue)

		{
			D3DXVECTOR3 eyePt = D3DFramework::Instance()->GetCamera()->getPosition();
			D3DXVECTOR3 vectorToEyePt1 = pObject1->transform.worldPosition - eyePt;
			float distance1 = D3DXVec3Length(&vectorToEyePt1);
			D3DXVECTOR3 vectorToEyePt2 = pObject2->transform.worldPosition - eyePt;
			float distance2 = D3DXVec3Length(&vectorToEyePt2);

			if (distance1 > distance2)
				return TRUE;
			else
				return FALSE;
		}
	}
	return false;
}

void D3DFramework::SetupRenderOrder()
{
	renderObjects.sort(Comp);
}

HRESULT D3DFramework::CreateShaderEffects()
{
	HRESULT hr;

	shaders.clear();

	for (unsigned int i = 0; i < shaderFileNames.size(); ++i)
	{
		wstring filename(shaderFileNames[i]);
		int subIndex = filename.find_last_of(TEXT(".") - 1);
		wstring shaderName = filename.substr(0, subIndex);

		D3DShaderEffect* shader = new D3DShaderEffect(pD3DDevice9, (LPCWSTR)filename.c_str());
		if (FAILED(hr = shader->Init()))
			return E_FAIL;

		shaders.insert(pair<LPCWSTR, D3DShaderEffect*>(shaderName.c_str(), shader));
	}

	return S_OK;
}

HRESULT D3DFramework::AddRenderObject(D3DRenderObject * renderObj)
{
	renderObjects.push_back(renderObj);
	return S_OK;
}

void D3DFramework::ProcessUserInput()
{
	Keyboard &keyboard = Keyboard::instance();
	Mouse &mouse = Mouse::instance();

	if (keyboard.keyPressed(Keyboard::KEY_ESCAPE))
		PostMessage(hWnd, WM_CLOSE, 0, 0);

	if (keyboard.keyPressed(Keyboard::KEY_ADD) || keyboard.keyPressed(Keyboard::KEY_NUMPAD_ADD))
	{
		camera->setRotationSpeed(camera->getRotationSpeed() + 0.01f);

		if (camera->getRotationSpeed() > 1.0f)
			camera->setRotationSpeed(1.0f);
	}

	if (keyboard.keyPressed(Keyboard::KEY_MINUS) || keyboard.keyPressed(Keyboard::KEY_NUMPAD_MINUS))
	{
		camera->setRotationSpeed(camera->getRotationSpeed() - 0.01f);

		if (camera->getRotationSpeed() <= 0.0f)
			camera->setRotationSpeed(0.01f);
	}

	if (keyboard.keyPressed(Keyboard::KEY_PERIOD))
	{
		mouse.setWeightModifier(mouse.weightModifier() + 0.1f);

		if (mouse.weightModifier() > 1.0f)
			mouse.setWeightModifier(1.0f);
	}

	if (keyboard.keyPressed(Keyboard::KEY_COMMA))
	{
		mouse.setWeightModifier(mouse.weightModifier() - 0.1f);

		if (mouse.weightModifier() < 0.0f)
			mouse.setWeightModifier(0.0f);
	}

	if (keyboard.keyPressed(Keyboard::KEY_M))
		mouse.smoothMouse(!mouse.isMouseSmoothing());

	if (keyboard.keyDown(Keyboard::KEY_LALT) || keyboard.keyDown(Keyboard::KEY_RALT))
	{
		if (keyboard.keyPressed(Keyboard::KEY_ENTER))
			ToggleFullScreen();
	}

	if (keyboard.keyPressed(Keyboard::KEY_SPACE))
	{
		if (camera->getBehavior() == Camera::CAMERA_BEHAVIOR_FIRST_PERSON)
		{
			camera->setBehavior(Camera::CAMERA_BEHAVIOR_FLIGHT);
		}
		else
		{
			const D3DXVECTOR3 &cameraPos = camera->getPosition();

			camera->setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
			camera->setPosition(cameraPos.x, CAMERA_POS.y, cameraPos.z);
		}
	}
}

void D3DFramework::GetCameraMoveDirection(D3DXVECTOR3 & direction)
{
	static bool moveForwardsPressed = false;
	static bool moveBackwardsPressed = false;
	static bool moveRightPressed = false;
	static bool moveLeftPressed = false;
	static bool moveUpPressed = false;
	static bool moveDownPressed = false;

	D3DXVECTOR3 velocity = camera->getCurrentVelocity();
	Keyboard &keyboard = Keyboard::instance();

	direction.x = direction.y = direction.z = 0.0f;

	if (keyboard.keyDown(Keyboard::KEY_UP) || keyboard.keyDown(Keyboard::KEY_W))
	{
		if (!moveForwardsPressed)
		{
			moveForwardsPressed = true;
			camera->setCurrentVelocity(velocity.x, velocity.y, 0.0f);
		}

		direction.z += 1.0f;
	}
	else
	{
		moveForwardsPressed = false;
	}

	if (keyboard.keyDown(Keyboard::KEY_DOWN) || keyboard.keyDown(Keyboard::KEY_S))
	{
		if (!moveBackwardsPressed)
		{
			moveBackwardsPressed = true;
			camera->setCurrentVelocity(velocity.x, velocity.y, 0.0f);
		}

		direction.z -= 1.0f;
	}
	else
	{
		moveBackwardsPressed = false;
	}

	if (keyboard.keyDown(Keyboard::KEY_RIGHT) || keyboard.keyDown(Keyboard::KEY_D))
	{
		if (!moveRightPressed)
		{
			moveRightPressed = true;
			camera->setCurrentVelocity(0.0f, velocity.y, velocity.z);
		}

		direction.x += 1.0f;
	}
	else
	{
		moveRightPressed = false;
	}

	if (keyboard.keyDown(Keyboard::KEY_LEFT) || keyboard.keyDown(Keyboard::KEY_A))
	{
		if (!moveLeftPressed)
		{
			moveLeftPressed = true;
			camera->setCurrentVelocity(0.0f, velocity.y, velocity.z);
		}

		direction.x -= 1.0f;
	}
	else
	{
		moveLeftPressed = false;
	}

	if (keyboard.keyDown(Keyboard::KEY_PAGEUP))
	{
		if (!moveUpPressed)
		{
			moveUpPressed = true;
			camera->setCurrentVelocity(velocity.x, 0.0f, velocity.z);
		}

		direction.y += 1.0f;
	}
	else
	{
		moveUpPressed = false;
	}

	if (keyboard.keyDown(Keyboard::KEY_PAGEDOWN))
	{
		if (!moveDownPressed)
		{
			moveDownPressed = true;
			camera->setCurrentVelocity(velocity.x, 0.0f, velocity.z);
		}

		direction.y -= 1.0f;
	}
	else
	{
		moveDownPressed = false;
	}
}

void D3DFramework::UpdateCamera(float fElapsedTime)
{
	static bool rollLeftPressed = false;
	static bool rollRightPressed = false;

	float heading = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;
	float rotationSpeed = camera->getRotationSpeed();
	D3DXVECTOR3 direction;
	Mouse &mouse = Mouse::instance();

	GetCameraMoveDirection(direction);

	switch (camera->getBehavior())
	{
	case Camera::CAMERA_BEHAVIOR_FIRST_PERSON:
		pitch = mouse.yPosRelative() * rotationSpeed;
		heading = mouse.xPosRelative() * rotationSpeed;

		if (Keyboard::instance().keyDown(Keyboard::KEY_Q))
		{
			if (!rollLeftPressed)
			{
				rollLeftPressed = true;
			}
		}
		else
		{
			rollLeftPressed = false;
		}

		if (Keyboard::instance().keyDown(Keyboard::KEY_E))
		{
			if (!rollRightPressed)
			{
				rollRightPressed = true;
			}
		}
		else
		{
			rollRightPressed = false;
		}
		camera->rotate(heading, pitch, 0.0f, rollLeftPressed, rollRightPressed);
		break;

	case Camera::CAMERA_BEHAVIOR_FLIGHT:
		heading = direction.x * CAMERA_SPEED_FLIGHT_YAW * fElapsedTime;
		pitch = -mouse.yPosRelative() * rotationSpeed;
		roll = mouse.xPosRelative() * rotationSpeed;

		camera->rotate(heading, pitch, roll);
		direction.x = 0.0f; // ignore yaw motion when updating camera velocity
		break;
	}

	camera->updatePosition(direction, fElapsedTime);
	
	pD3DDevice9->SetTransform(D3DTS_VIEW, &camera->getViewMatrix());
	pD3DDevice9->SetTransform(D3DTS_PROJECTION, &camera->getProjectionMatrix());
}

HRESULT D3DFramework::Init(HWND _hWnd, LPCTSTR lpszClass, int screenWidth, int screenHeight)
{
	hWnd = _hWnd;
	strWindowTitle = lpszClass;
	windowWidth = screenWidth;
	windowHeight = screenHeight;

	// Direct 3D 사용
	if (NULL == (pD3D9 = Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

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
	d3dpp.BackBufferWidth = windowWidth;
	d3dpp.BackBufferHeight = windowHeight;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
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
		&d3dpp, &pD3DDevice9)))
	{
		if (FAILED(hr = pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice9)))
		{
			SAFE_RELEASE(pD3DDevice9);
			return DisplayErrorMsg(D3DAPPERR_NULLREFDEVICE, MSGERR_APPMUSTEXIT);
		}
	}

	D3DCAPS9 caps;

	// Prefer anisotropic texture filtering if it's supported.
	if (SUCCEEDED(pD3DDevice9->GetDeviceCaps(&caps)))
	{
		if (caps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY)
			maxAnisotrophy = caps.MaxAnisotropy;
		else
			maxAnisotrophy = 1;
	}

	// 카메라 생성
	camera = new Camera();

	// Shader Effect 로드
	CreateShaderEffects();

	// 보통은 이 부분에서 SceneManager 같은 것을 초기화 시켜야 할것이다.
	// 일반적인 게임 엔진에서는 이 SceneManager 등록된 Scene을 로드 하여 각각의 RenderObject들을 생성할 것이다
	// 하지만 일단 우리는 여기서는 아무것도 하지 않는다. 지금은 Main.cpp 에서 각각의 렌더 오브젝트를 세팅하는 것으로 하자.

	// 카메라 초기화
	camera->perspective(CAMERA_FOVX,
		static_cast<float>(GetSystemMetrics(SM_CXSCREEN)) / static_cast<float>(GetSystemMetrics(SM_CYSCREEN)),
		CAMERA_ZNEAR, CAMERA_ZFAR);

	camera->setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
	camera->setPosition(CAMERA_POS);
	camera->setAcceleration(CAMERA_ACCELERATION);
	camera->setVelocity(CAMERA_VELOCITY);

	return S_OK;
}

HRESULT D3DFramework::UpdateFrame(float fElapsedTime)
{
	SetupRenderOrder();

	Keyboard::instance().update();
	Mouse::instance().update();

	ProcessUserInput();

	UpdateCamera(fElapsedTime);

	// 변경되는 데이터들을 처리합니다.
	UpdateRenderObjects(fElapsedTime);

	return S_OK;
}

HRESULT D3DFramework::UpdateRenderObjects(float fElapsedTime)
{
	for (list<D3DRenderObject*>::iterator iter = renderObjects.begin(); iter != renderObjects.end(); ++iter)
	{
		if (FAILED((*iter)->UpdateFrame(fElapsedTime)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT D3DFramework::Render()
{
	pD3DDevice9->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100,100,255), 1.0f, 0);

	if (FAILED(pD3DDevice9->BeginScene()))
	{
		return E_FAIL;
	}
	else
	{
		// 이 공간에 모든 렌더링 오브젝트를 Draw합니다.
		DrawRenderObjects();

		pD3DDevice9->EndScene();
		pD3DDevice9->Present(0, 0, 0, 0);
		return S_OK;
	}
}

HRESULT D3DFramework::DrawRenderObjects()
{
	for (list<D3DRenderObject*>::iterator iter = renderObjects.begin(); iter != renderObjects.end(); ++iter)
	{

		(*iter)->Render();
	}
	return S_OK;
}

HRESULT D3DFramework::Release()
{
	for (list<D3DRenderObject*>::iterator iter = renderObjects.begin(); iter != renderObjects.end(); ++iter)
	{
		SAFE_RELEASE(*iter);
	}

	SAFE_RELEASE(pD3DDevice9);
	SAFE_RELEASE(pD3D9);

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