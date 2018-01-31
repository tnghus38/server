#pragma once

#include <vector>
#include <d3dx9.h>

using namespace std;

class D3DRenderObject
{
	// 원점 기준
	D3DXVECTOR3 _worldPosition; 
	D3DXQUATERNION _worldRotation;

	//항상 일정 
	D3DXVECTOR3 _localScale;
	//부모기준
	D3DXVECTOR3 _localPosition;
	D3DXQUATERNION _localRotation;

	D3DXMATRIX _localMat;

	//최종계산 메트릭스 
	D3DXMATRIX _worldMat;


	vector<LPD3DXEFFECT> _effects; // 로드한 셰이더를 벡터에 담아두고 이름으로 찾아씀 
public:

	D3DRenderObject * _ParentObj;//부모 
	vector<D3DRenderObject*> _ChildObj;//자식 

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