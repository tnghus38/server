#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include "D3DFramework.h"

#define MAX_LOADSTRING 100

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("LJH");
float GetElapsedTimeInSeconds();//시간 

bool hasFocus;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HWND hWnd;
	WNDCLASS WndClass;
	MSG msg;

	ZeroMemory(&WndClass, sizeof(WNDCLASS));

	WndClass.cbClsExtra = 0;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;

	WndClass.cbWndExtra = 0;
	WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;

	RegisterClass(&WndClass);

	DWORD wndExStyle = WS_EX_OVERLAPPEDWINDOW;
	DWORD wndStyle= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPCHILDREN;
	hWnd = CreateWindowEx(wndExStyle, WndClass.lpszClassName, lpszClass, wndStyle, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	D3DFramework::Instance()->Init(hWnd);
	HACCEL hAccelTable = LoadAccelerators(hInstance, WndClass.lpszClassName);

	while (true)
	{
		//메시지가없을때호출  //겟메시지는 메식지가 없을때를 처리못함  
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) == false)
		{
			if (msg.message == WM_QUIT)
				break;

			/*TranslateAccelerator: 입력한 키가 액셀러레이터에 등록되어 있는 경우 WM_KEYDOWN 또는 WM_SYSKEYDOWN
			                        메세지를 WM_COMMAND 또는 WM_SYSCOMMAND 메세지로 변환한다.*/
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (msg.message == WM_QUIT)
			break;

		if (hasFocus)
		{
			D3DFramework::Instance()->updateFrame(GetElapsedTimeInSeconds());

			if (D3DFramework::Instance()->isDevicevalid())
				D3DFramework::Instance()->Render();
		}
		else
		{
			WaitMessage();
		}
	}
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			EndPaint(hWnd, &ps);
			break;
	}
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
				ShowWindow(hWnd, SW_MINIMIZE);
			hasFocus = false;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
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