#pragma once

#include <vector>
#include <d3dx9.h>

using namespace std;

class D3DRenderObject
{
	// ���� ����
	D3DXVECTOR3 _worldPosition; 
	D3DXQUATERNION _worldRotation;

	//�׻� ���� 
	D3DXVECTOR3 _localScale;
	//�θ����
	D3DXVECTOR3 _localPosition;
	D3DXQUATERNION _localRotation;

	D3DXMATRIX _localMat;

	//������� ��Ʈ���� 
	D3DXMATRIX _worldMat;


	vector<LPD3DXEFFECT> _effects; // �ε��� ���̴��� ���Ϳ� ��Ƶΰ� �̸����� ã�ƾ� 
public:

	D3DRenderObject * _ParentObj;//�θ� 
	vector<D3DRenderObject*> _ChildObj;//�ڽ� 

	D3DRenderObject();
	virtual ~D3DRenderObject();

	virtual void AttachNewRenderObject();

	void AttachNewRenderObject(D3DRenderObject * Obj);
	virtual void Init() {};
	virtual void UpdateFrame() {};
	virtual void Render() {};
	virtual void Release();
protected:
	virtual void UpdateWorldMatrix();
};