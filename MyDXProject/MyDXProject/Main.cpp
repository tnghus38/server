#include <windows.h>
#include "D3DFramework.h"
#include "D3DPrimitive.h"
#include "input.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MyDXProject");

int windowWidth;
int windowHeight;
bool hasFocus;
bool isFullScreen;

float GetElapsedTimeInSeconds();

D3DPrimitive* triangle;
D3DPrimitive* triangleChild;
D3DPrimitive* quad;
D3DPrimitive* cube;
void InitD3DRenderObjects()
{
	// 삼각형 1 등록
	DWORD primitiveFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	triangle = new D3DPrimitive(
		D3DFramework::Instance()->GetD3DDevice(), 
		primitiveFVF, 
		D3DPrimitive::PrimitiveType::Triangle);
	D3DFramework::Instance()->AddRenderObject(triangle);
	triangle->Init();
	triangle->renderQueue = RenderObject::Transparent;

	// 삼각형 2 등록
	triangleChild = new D3DPrimitive(
		D3DFramework::Instance()->GetD3DDevice(),
		primitiveFVF,
		D3DPrimitive::PrimitiveType::Mesh);
	D3DFramework::Instance()->AddRenderObject(triangleChild);
	triangleChild->Init();
	triangleChild->renderQueue = RenderObject::Transparent;
	//쿼드
	quad = new D3DPrimitive(
		D3DFramework::Instance()->GetD3DDevice(),
		primitiveFVF,
		D3DPrimitive::PrimitiveType::Quad);
	D3DFramework::Instance()->AddRenderObject(quad);
	quad->Init();
	quad->renderQueue = RenderObject::Transparent;

	triangleChild->transform.AddChild(&quad->transform);
	triangleChild->transform.Translate(-2.5f, 1.0f, 0.0f);
	quad->transform.Translate(1.0f, 0.0f, 2.0f);
	//큐브
	cube = new D3DPrimitive(
		D3DFramework::Instance()->GetD3DDevice(),
		primitiveFVF,
		D3DPrimitive::PrimitiveType::Cube);
	D3DFramework::Instance()->AddRenderObject(cube);
	cube->Init();
	cube->renderQueue = RenderObject::Transparent;

	cube->transform.Translate(3.0f, 1.0f, 2.0f);
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow )

{
	HWND		hWnd;
	MSG			msg;
	WNDCLASS	WndClass;

	ZeroMemory(&WndClass, sizeof(WNDCLASS));

	g_hInst	= hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	DWORD wndExStyle = WS_EX_OVERLAPPEDWINDOW;
	DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;

	hWnd = CreateWindowEx(wndExStyle, WndClass.lpszClassName, lpszClass,
		wndStyle, 0, 0, 0, 0, 0, 0, WndClass.hInstance, 0);

	if (hWnd == NULL) {
		MessageBox(NULL, TEXT("can't create window"), TEXT("error"), MB_ICONERROR | MB_OK);
		return -1;
	}

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int halfScreenWidth = screenWidth / 2;
	int halfScreenHeight = screenHeight / 2;
	int left = (screenWidth - halfScreenWidth) / 2;
	int top = (screenHeight - halfScreenHeight) / 2;
	RECT rc = { 0 };

	SetRect(&rc, left, top, left + halfScreenWidth, top + halfScreenHeight);
	AdjustWindowRectEx(&rc, wndStyle, FALSE, wndExStyle);
	MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

	GetClientRect(hWnd, &rc);
	windowWidth = rc.right - rc.left;
	windowHeight = rc.bottom - rc.top;

	if (FAILED(D3DFramework::Instance()->Init(hWnd, lpszClass, windowWidth, windowHeight)))
	{
		return -1;
	}

	// 렌더 오브젝트를 초기화 한다. 
	InitD3DRenderObjects();

	ShowWindow(hWnd, nCmdShow);

	while (true)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		if (hasFocus)
		{
			float deltaTime = GetElapsedTimeInSeconds();

			triangle->transform.Rotate(0.0f, deltaTime * 50.0f, 0.0f);
			triangleChild->transform.Rotate(0.0f, deltaTime * 50.0f, 0.0f);
			cube ->transform.Rotate(deltaTime * 50.0f, deltaTime * 50.0f, deltaTime * 50.0f);
			D3DFramework::Instance()->UpdateFrame(deltaTime);

			if (D3DFramework::Instance()->isDeviceValid())
				D3DFramework::Instance()->Render();
		}
		else
		{
			WaitMessage();
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	Keyboard::instance().handleMsg(hWnd, iMessage, wParam, lParam);

	switch (iMessage)
	{
	case WM_ACTIVATE:
		switch (wParam)
		{
		default:
			break;

		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			hasFocus = true;
			break;

		case WA_INACTIVE:
			if (D3DFramework::Instance()->isFullScreen)
				ShowWindow(hWnd, SW_MINIMIZE);
			hasFocus = false;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		D3DFramework::Instance()->windowWidth = windowWidth = static_cast<int>(LOWORD(lParam));
		D3DFramework::Instance()->windowHeight = windowHeight = static_cast<int>(HIWORD(lParam));
		break;

	default:
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

float GetElapsedTimeInSeconds()
{
	// Returns the elapsed time (in seconds) since the last time this function
	// was called. This elaborate setup is to guard against large spikes in
	// the time returned by QueryPerformanceCounter().

	static const int MAX_SAMPLE_COUNT = 50;

	static float frameTimes[MAX_SAMPLE_COUNT];
	static float timeScale = 0.0f;
	static float actualElapsedTimeSec = 0.0f;
	static INT64 freq = 0;
	static INT64 lastTime = 0;
	static int sampleCount = 0;
	static bool initialized = false;

	INT64 time = 0;
	float elapsedTimeSec = 0.0f;

	if (!initialized)
	{
		initialized = true;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&lastTime));
		timeScale = 1.0f / freq;
	}

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
	elapsedTimeSec = (time - lastTime) * timeScale;
	lastTime = time;

	if (fabsf(elapsedTimeSec - actualElapsedTimeSec) < 1.0f)
	{
		memmove(&frameTimes[1], frameTimes, sizeof(frameTimes) - sizeof(frameTimes[0]));
		frameTimes[0] = elapsedTimeSec;

		if (sampleCount < MAX_SAMPLE_COUNT)
			++sampleCount;
	}

	actualElapsedTimeSec = 0.0f;

	for (int i = 0; i < sampleCount; ++i)
		actualElapsedTimeSec += frameTimes[i];

	if (sampleCount > 0)
		actualElapsedTimeSec /= sampleCount;

	return actualElapsedTimeSec;
}