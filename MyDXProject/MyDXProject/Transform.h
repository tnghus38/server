#pragma once
#include <vector>
#include <d3dx9.h>

using namespace std;

class Transform
{
protected:
	D3DXVECTOR3					_xAxis;					// ���� ȸ�� ���¿����� X�� ����
	D3DXVECTOR3					_yAxis;					// ���� ȸ�� ���¿����� X�� ����
	D3DXVECTOR3					_zAxis;					// ���� ȸ�� ���¿����� X�� ����
	D3DXVECTOR3					_viewDir;				// �ٶ󺸰� �ִ� ���� ���������� _zAxis�� ��������, Ư�� ��ü�� Ÿ���� �ٶ󺸱� ���� ���� ����� �̰����� �Ѵ�.

	// Matrices
	D3DXMATRIX					_localMat;
	D3DXMATRIX					_worldMat;				// ���� ���� WorldMatrix
														// this->worldMat = _parent->worldMat * this->_localMat

	// Child Object Array
	vector<Transform*>		_childTransform;		// �ڽ� RenderObject��

public:													
	// Scale
	D3DXVECTOR3					_localScale;			// Scale�� ���ø� ������ �ȴ�.

	// Rotation
	D3DXQUATERNION				_localRotation;			// �θ� RenderObject ���� ���� Rotation
	D3DXQUATERNION				_worldRotation;			// ���� ���� Rotation

	// Position
	D3DXVECTOR3					_localPosition;			// �θ� RenderObject ���� Position
	D3DXVECTOR3					_worldPosition;			// ���� ���� Position

	Transform*				parent;						// �θ� RenderObject

protected:
	void UpdateWorldMatrix();

public:
	D3DXVECTOR3					localRotationEuler;			// ���� localRotation�� �����ϴ� �� �࿡ �ش��ϴ� Euler ȸ���� (������ �ƴϴ�);

	Transform();
	virtual ~Transform();

	virtual void UpdateFrame();

	void Rotate(float xAngle, float yAngle, float zAngle);
	void Translate(float xPos, float y, float z);
	void Scale(float xScale, float yScale, float zScale);

	void AddChild(Transform* obj);
};