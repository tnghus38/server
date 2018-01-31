#pragma once
#include "D3DRenderObject.h"

class BoxObject : public D3DRenderObject
{

public:
	BoxObject();
	~BoxObject();
	void AttachNewRenderObject();

	
	void Init();
	void UpdateFrame();
	void Render();
	void UpdateWorldMatrix();
	void Release();
};