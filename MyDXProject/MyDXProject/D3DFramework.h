#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <vector>
#include <list>

#include "D3DUtil.h"
#include "D3DRenderObject.h"
#include "D3DShaderEffect.h"

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


const D3DXVECTOR3 CAMERA_ACCELERATION(8.0f, 8.0f, 8.0f);
const float       CAMERA_FOVX = 90.0f;
const D3DXVECTOR3 CAMERA_POS(0.0f, 0.0f, -10.0f);
const float       CAMERA_SPEED_ROTATION = 0.2f;
const float       CAMERA_SPEED_FLIGHT_YAW = 100.0f;
const D3DXVECTOR3 CAMERA_VELOCITY(2.0f, 2.0f, 2.0f);
const float       CAMERA_ZFAR = 100.0f;
const float       CAMERA_ZNEAR = 0.1f;

using namespace std;

class Camera;

class D3DFramework
{
private:
	// 싱글턴 인스턴스
	static D3DFramework* instance;

	// D3D9 객체 포인터
	LPDIRECT3D9						pD3D9 = NULL;

	// D3DDevice9 포인터
	LPDIRECT3DDEVICE9				pD3DDevice9 = NULL;

	// Window 핸들러
	HWND							hWnd = NULL;

	// 프로그램 타이틀 이름
	LPCTSTR							strWindowTitle = NULL;

	// 카메라 클래스 포인터
	Camera*							camera = NULL;

	D3DPRESENT_PARAMETERS			d3dpp;

	DWORD							msaaSamples;
	DWORD							maxAnisotrophy;

	list<D3DRenderObject*>			renderObjects;

	// Shader Effect 컨테이너
	map<LPCWSTR, D3DShaderEffect*>	shaders;
	vector<LPCWSTR>					shaderFileNames;

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

	// FullScreen;
	bool	isFullScreen = false;
	bool	enableVerticalSync = true;

	// Window width and height
	int windowWidth;
	int windowHeight;
	
	LPDIRECT3DDEVICE9 GetD3DDevice() { return pD3DDevice9; }

	void ToggleFullScreen();
	bool isDeviceValid();
	bool ResetDevice();

	void SetupRenderOrder();

	// 사용할 모든 Shader 로드
	HRESULT CreateShaderEffects();

	// 렌더 오브젝트들 관련
	// HRESULT LoadFirstScene(); 
	HRESULT AddRenderObject(D3DRenderObject* renderObj);

	// 입력 처리
	void ProcessUserInput();

	// 카메라 갱신
	void GetCameraMoveDirection(D3DXVECTOR3 &direction);
	void UpdateCamera(float fElapsedTime);

	HRESULT Init(HWND hWnd, LPCTSTR lpszClass, int screenWidth, int screenHeight);
	HRESULT UpdateFrame(float fElapsedTime);
	HRESULT Render();
	HRESULT Release();

	HRESULT UpdateRenderObjects(float fElapsedTime);
	HRESULT DrawRenderObjects();

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

