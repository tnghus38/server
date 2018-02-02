#include "Transform.h"

Transform::Transform()
{
	D3DXMatrixIdentity(&_worldMat);				// ���� ��Ʈ���� ������ķ� �ʱ�ȭ

	_worldPosition.x = _localPosition.x = 0.f;	// ���� position�� ���� position ��� ��������
	_worldPosition.y = _localPosition.y = 0.f;
	_worldPosition.z = _localPosition.z = 0.f;

	D3DXQuaternionIdentity(&_worldRotation);	// Rotation�� ���� ���ʹϾ����� �ʱ�ȭ
	D3DXQuaternionIdentity(&_localRotation);

	parent = NULL;								// �θ� ������Ʈ�� NULL
	_childTransform.clear();					// �ڽ� ������Ʈ�� ����ݴϴ�.
}

Transform::~Transform()
{
	for (int i = 0; i < _childTransform.size(); ++i)
	{
		if (_childTransform[i] != NULL)
		{
			delete _childTransform[i];
		}
	}

	_childTransform.clear();
}

void Transform::AddChild(Transform* obj)
{
	_childTransform.push_back(obj);
	obj->parent = this;
}

void Transform::UpdateWorldMatrix()
{
	// �ڽ��� ���� ��Ʈ���� -----------------------------

	// �ʱ�ȭ
	D3DXMatrixIdentity(&_localMat);

	// Scale
	D3DXMatrixScaling(&_localMat, _localScale.x, _localScale.y, _localScale.z);
	
	// Rotation : Quaternion
	D3DXQUATERNION orientation;
	D3DXQuaternionRotationMatrix(&orientation, &_localMat);
	
	// �� ���� ȸ������ ��������
	float yaw = D3DXToRadian(localRotationEuler.y);
	float pitch = D3DXToRadian(localRotationEuler.x);
	float roll = D3DXToRadian(localRotationEuler.z);

	// ���� ȸ�� ���ʹϾ��� ���ϰ�
	D3DXQuaternionRotationYawPitchRoll(&_localRotation, yaw, pitch, roll);
	D3DXQuaternionMultiply(&_localRotation, &orientation, &_localRotation);

	// Rotation ���ʹϾ��� ����ȭ
	D3DXQuaternionNormalize(&_localRotation, &_localRotation);

	// ȸ�� ���� ������ ����� ��´�.
	D3DXMatrixRotationQuaternion(&_localMat, &_localRotation);

	// ȸ���� ���¿����� �� ���� ���͸� ������
	_xAxis = D3DXVECTOR3(_localMat(0, 0), _localMat(1, 0), _localMat(2, 0));
	_yAxis = D3DXVECTOR3(_localMat(0, 1), _localMat(1, 1), _localMat(2, 1));
	_zAxis = D3DXVECTOR3(_localMat(0, 2), _localMat(1, 2), _localMat(2, 2));
	_viewDir = _zAxis;

	// �׸��� Position�� ����
	_localMat(3, 0) = -D3DXVec3Dot(&_xAxis, &_localPosition);
	_localMat(3, 1) = -D3DXVec3Dot(&_yAxis, &_localPosition);
	_localMat(3, 2) = -D3DXVec3Dot(&_zAxis, &_localPosition);

	// ���� ���� ��Ʈ���� ���ϱ�
	D3DXMATRIX parentWorld;

	if (parent != NULL)
	{
		parentWorld = parent->_worldMat;
	}
	else
	{
		D3DXMatrixIdentity(&parentWorld);
	}

	_worldMat = parentWorld * _localMat;
	_worldPosition.x = _worldMat(3, 0);
	_worldPosition.y = _worldMat(3, 1);
	_worldPosition.z = _worldMat(3, 2);

}

void Transform::UpdateFrame()
{
	UpdateWorldMatrix();

	for (int i = 0; i < _childTransform.size(); ++i)
	{
		_childTransform[i]->UpdateFrame();
	}
}

void Transform::Rotate(float xAngle, float yAngle, float zAngle)
{
	localRotationEuler.x += xAngle;
	if (localRotationEuler.x > 360.0f)
		localRotationEuler.x -= 360.0f;
	localRotationEuler.y += yAngle;
	if (localRotationEuler.y > 360.0f)
		localRotationEuler.y -= 360.0f;
	localRotationEuler.z += zAngle;
	if (localRotationEuler.z > 360.0f)
		localRotationEuler.z -= 360.0f;
}

void Transform::Translate(float xPos, float yPos, float zPos)
{
	_localPosition.x = xPos;
	_localPosition.y = yPos;
	_localPosition.z = zPos;
}

void Transform::Scale(float xScale, float yScale, float zScale)
{
	_localScale.x = xScale;
	_localScale.y = yScale;
	_localScale.z = zScale;
}
