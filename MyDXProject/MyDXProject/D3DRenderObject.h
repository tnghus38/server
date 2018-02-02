#pragma once
#include <list>
#include "Transform.h"
class Transform;

class D3DRenderObject : public Transform
{
protected:
	LPDIRECT3DDEVICE9 device;
	LPD3DXEFFECT effect = NULL;

public:
	D3DRenderObject(LPDIRECT3DDEVICE9 _device);
	virtual ~D3DRenderObject();

	void SetEffect(LPD3DXEFFECT _effect);
	virtual HRESULT Reset() { return S_OK; }
	virtual HRESULT Init() { return S_OK; }
	virtual HRESULT Render() { return S_OK; }
};