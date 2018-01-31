#include "D3DRenderObject.h"

D3DRenderObject::D3DRenderObject()
{
	D3DXMatrixIdentity(&_worldMat);

	_worldPosition.x = _localPosition.x = 0.f;
	_worldPosition.y = _localPosition.y = 0.f;
	_worldPosition.z = _localPosition.z = 0.f;

	D3DXQuaternionIdentity(&_worldRotation);
	D3DXQuaternionIdentity(&_localRotation);

	_ParentObj = NULL;
	_ChildObj.clear();
	_effects.clear();
}

D3DRenderObject::~D3DRenderObject()
{
	for (int i = 0; i < _ChildObj.size(); ++i)
	{
		if (_ChildObj[i] != NULL)
		{
			_ChildObj[i]->Release();
			delete _ChildObj[i];
		}
	}
	_ChildObj.clear();
}

void D3DRenderObject::AttachNewRenderObject()
{

	D3DRenderObject* child = new D3DRenderObject();
	child->_ParentObj = this;
	_ChildObj.push_back(child);
}
void D3DRenderObject::AttachNewRenderObject(D3DRenderObject* Obj)
{

	Obj->_ParentObj = this;
	_ChildObj.push_back(Obj);
}
void D3DRenderObject::Release()
{

	//오버라이딩 해줘도 공통으로 해줘야함 
	for (int i = 0; i < _ChildObj.size(); ++i)
	{
		if (_ChildObj[i] != NULL)
			_ChildObj[i]->Release();
	}
}

void D3DRenderObject::UpdateWorldMatrix()
{
	//부모 매트릭스 
	D3DXMATRIX parentWorld;
	
	//자기자신 매트릭스 
	 //다음시간 

	if (_ParentObj != NULL)
	{
		parentWorld = _ParentObj->_worldMat; //부모가 있으면 부모의 월드 매트릭를 넣어줌 
	}
	else
	{
		D3DXMatrixIdentity(&parentWorld);//부모가없으면 단위행렬 (연산시 그대로) 넣어줌 
	}
	
	//내월드 매트릭스= 부모 월드 매트릭스 * 내로컬 매트릭스 
	_worldMat = parentWorld * _localMat;
}
