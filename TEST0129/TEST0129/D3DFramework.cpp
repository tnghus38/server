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
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION); //�ý����� ���� �� ���̷�Ʈ �������� �������ڴ� 

	if (d3d9 == NULL)  // �ý����� ���������� ������ 
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dPP;
	ZeroMemory(&d3dPP, sizeof(d3dPP));
	d3dPP.Windowed = TRUE;
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD; //����ۿ� ����Ʈ���۸� �ٲ��ٶ� �ɼ�  
											  //DISCARD�� �ø�����̳� ī�ǹ������ �Ҷ� ����Ʈ���ۿ� �����ִ�(���� ������)�� ���� �ϴ� �۾��� ����۾��̶� �̸� �������� �ʰ� �۾��Ҷ����¹��  
	d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;//window�϶� OS�� ����� ���� ���󰣴� 

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);//������ �����Ǵ� �Ӽ������� ������� 
	int vertexProcessing;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) //D3DDEVCAPS_HWTRANSFORMANDLIGHT�̱���� �������� Ȯ�� 
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING; //�ϵ忡�� ���ؽ� ó���� �Ҽ������� 
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING; // �ϵ忡�� ���ؽ� ó���� ���Ҷ� ����Ʈ����� ó�� 

	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, //�ϵ���� ������ ��
		hWnd, // ���� ������ �ڵ� 
		vertexProcessing, // ���ؽ� ó�� ��� 
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
	//��         ����0~1   

	// Render ����
	if (SUCCEEDED(d3dDevice9->BeginScene()))
	{
		// Render �ϱ�

		// Render ��
		d3dDevice9->EndScene();
	}

	// Backbuffer�� FrontBuffer�� Swap
	d3dDevice9->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

HRESULT D3DFramework::Release()
{
	return S_OK;
}
