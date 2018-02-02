#pragma once
#include <vector>
#include <d3dx9.h>

using namespace std;

class Transform
{
protected:
	D3DXVECTOR3					_xAxis;					// 현재 회전 상태에서의 X축 벡터
	D3DXVECTOR3					_yAxis;					// 현재 회전 상태에서의 X축 벡터
	D3DXVECTOR3					_zAxis;					// 현재 회전 상태에서의 X축 벡터
	D3DXVECTOR3					_viewDir;				// 바라보고 있는 방향 최종적으로 _zAxis와 같겠지만, 특정 물체나 타겟을 바라보기 위한 벡터 계산은 이것으로 한다.

	// Matrices
	D3DXMATRIX					_localMat;
	D3DXMATRIX					_worldMat;				// 최종 계산된 WorldMatrix
														// this->worldMat = _parent->worldMat * this->_localMat

	// Child Object Array
	vector<Transform*>		_childTransform;		// 자식 RenderObject들

public:													
	// Scale
	D3DXVECTOR3					_localScale;			// Scale은 로컬만 있으면 된다.

	// Rotation
	D3DXQUATERNION				_localRotation;			// 부모 RenderObject 기준 최종 Rotation
	D3DXQUATERNION				_worldRotation;			// 원점 기준 Rotation

	// Position
	D3DXVECTOR3					_localPosition;			// 부모 RenderObject 기준 Position
	D3DXVECTOR3					_worldPosition;			// 원점 기준 Position

	Transform*				parent;						// 부모 RenderObject

protected:
	void UpdateWorldMatrix();

public:
	D3DXVECTOR3					localRotationEuler;			// 현재 localRotation을 결정하는 각 축에 해당하는 Euler 회전각 (라디안이 아니다);

	Transform();
	virtual ~Transform();

	virtual void UpdateFrame();

	void Rotate(float xAngle, float yAngle, float zAngle);
	void Translate(float xPos, float y, float z);
	void Scale(float xScale, float yScale, float zScale);

	void AddChild(Transform* obj);
};