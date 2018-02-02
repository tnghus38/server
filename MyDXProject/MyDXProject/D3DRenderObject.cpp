#include "D3DRenderObject.h"

D3DRenderObject::D3DRenderObject(LPDIRECT3DDEVICE9 _device)
{
	device = _device;
}


D3DRenderObject::~D3DRenderObject()
{
}

void D3DRenderObject::SetEffect(LPD3DXEFFECT _effect)
{
	effect = _effect;
}