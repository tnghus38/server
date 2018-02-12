#pragma once

#include <d3dx9.h>
#include "D3DUtil.h"
class D3DObject
{
protected:
	LPDIRECT3DDEVICE9 _device;

public:
	D3DObject();
	virtual ~D3DObject();

	virtual HRESULT Init() { return S_OK; }
	virtual HRESULT Reset() { return S_OK; }
	virtual HRESULT Release() { return S_OK; }
};

