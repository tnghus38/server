#include <windows.h>
#include "D3DFramework.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MyDXProject");

int windowWidth;
int windowHeight;
bool hasFocus;
bool isFullScreen;

float GetElapsedTimeInSeconds();

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
	DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
		WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	hWnd = CreateWindowEx(wndExStyle, WndClass.lpszClassName, lpszClass,
		wndStyle, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) {
		MessageBox(NULL, TEXT("can't create window"), TEXT("error"), MB_ICONERROR | MB_OK);
		return -1;
	}

	if (FAILED(D3DFramework::Instance()->Init(hWnd, lpszClass)))
	{
		return -1;
	}

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
			D3DFramework::Instance()->UpdateFrame(GetElapsedTimeInSeconds());

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
			if (isFullScreen)
				ShowWindow(hWnd, SW_MINIMIZE);
			hasFocus = false;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		windowWidth = static_cast<int>(LOWORD(lParam));
		windowHeight = static_cast<int>(HIWORD(lParam));
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