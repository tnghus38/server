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

	//�������̵� ���൵ �������� ������� 
	for (int i = 0; i < _ChildObj.size(); ++i)
	{
		if (_ChildObj[i] != NULL)
			_ChildObj[i]->Release();
	}
}

void D3DRenderObject::UpdateWorldMatrix()
{
	//�θ� ��Ʈ���� 
	D3DXMATRIX parentWorld;
	
	//�ڱ��ڽ� ��Ʈ���� 
	 //�����ð� 

	if (_ParentObj != NULL)
	{
		parentWorld = _ParentObj->_worldMat; //�θ� ������ �θ��� ���� ��Ʈ���� �־��� 
	}
	else
	{
		D3DXMatrixIdentity(&parentWorld);//�θ𰡾����� ������� (����� �״��) �־��� 
	}
	
	//������ ��Ʈ����= �θ� ���� ��Ʈ���� * ������ ��Ʈ���� 
	_worldMat = parentWorld * _localMat;
}
