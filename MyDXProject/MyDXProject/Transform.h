#pragma once
#include <vector>
#include <d3dx9.h>
using namespace std;

class D3DRenderObject;

class Transform
{
protected:
	D3DXVECTOR3					_xAxis;					// ���� ȸ�� ���¿����� X�� ����
	D3DXVECTOR3					_yAxis;					// ���� ȸ�� ���¿����� X�� ����
	D3DXVECTOR3					_zAxis;					// ���� ȸ�� ���¿����� X�� ����
	D3DXVECTOR3					_viewDir;				// �ٶ󺸰� �ִ� ���� ���������� _zAxis�� ��������, Ư�� ��ü�� Ÿ���� �ٶ󺸱� ���� ���� ����� �̰����� �Ѵ�.

	// Matrices
	D3DXMATRIX					_localMat;

	// Child Object Array
	vector<Transform*>			_childTransforms;		// �ڽ� RenderObject��

public:													
	// Scale
	D3DXVECTOR3					localScale;			// Scale�� ���ø� ������ �ȴ�.

	// Rotation
	D3DXQUATERNION				localRotation;			// �θ� RenderObject ���� ���� Rotation
	D3DXQUATERNION				worldRotation;			// ���� ���� Rotation

	// Position
	D3DXVECTOR3					localPosition;			// �θ� RenderObject ���� Position
	D3DXVECTOR3					worldPosition;			// ���� ���� Position

	D3DXMATRIX					worldMat;				// ���� ���� WorldMatrix
														// this->worldMat = _parent->worldMat * this->_localMat

	Transform*					parent;					// �θ� RenderObject

	D3DRenderObject*			renderObject = NULL;

protected:
	void UpdateWorldMatrix();

public:
	D3DXVECTOR3					localRotationEuler;			// ���� localRotation�� �����ϴ� �� �࿡ �ش��ϴ� Euler ȸ���� (������ �ƴϴ�);

	Transform();
	virtual ~Transform();

	virtual void UpdateFrame(float fElapsedTime);

	void Rotate(float xAngle, float yAngle, float zAngle);
	void Translate(float xPos, float y, float z);
	void Scale(float xScale, float yScale, float zScale);

	vector<Transform*> GetChildren() { return _childTransforms; }

	void AddChild(Transform* obj);
};