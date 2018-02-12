#include "D3DRenderObject.h"

D3DRenderObject::D3DRenderObject(LPDIRECT3DDEVICE9 device, DWORD fvf)
{
	_device = device;
	_FVF = fvf;

	transform.renderObject = this;
}

D3DRenderObject::~D3DRenderObject()
{
}

void D3DRenderObject::SetShader(D3DShaderEffect* effect)
{
	_shaderEffect = effect;
}

HRESULT D3DRenderObject::UpdateFrame(float fElapsedTime)
{
	transform.UpdateFrame(fElapsedTime);
	return S_OK;
}