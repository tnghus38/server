#include "Transform.h"

Transform::Transform()
{
	D3DXMatrixIdentity(&worldMat);						// ���� ��Ʈ���� ������ķ� �ʱ�ȭ

	localScale.x = localScale.y = localScale.z = 1.0f;	// Scale �ʱ�ȭ;

	localRotationEuler.x = 0.0f;						// localRotation �� �ʱ�ȭ;
	localRotationEuler.y = 0.0f;
	localRotationEuler.z = 0.0f;

	worldPosition.x = localPosition.x = 0.f;			// ���� position�� ���� position ��� ��������
	worldPosition.y = localPosition.y = 0.f;
	worldPosition.z = localPosition.z = 0.f;

	D3DXQuaternionIdentity(&worldRotation);				// Rotation�� ���� ���ʹϾ����� �ʱ�ȭ
	D3DXQuaternionIdentity(&localRotation);

	parent = NULL;										// �θ� ������Ʈ�� NULL
	_childTransforms.clear();							// �ڽ� ������Ʈ�� ����ݴϴ�.
}

Transform::~Transform()
{
	for (unsigned int i = 0; i < _childTransforms.size(); ++i)
	{
		if (_childTransforms[i] != NULL)
		{
			delete _childTransforms[i];
		}
	}

	_childTransforms.clear();
}

void Transform::AddChild(Transform* obj)
{
	_childTransforms.push_back(obj);
	obj->parent = this;
}

void Transform::UpdateWorldMatrix()
{
	// �ڽ��� ���� ��Ʈ���� -----------------------------

	// �ʱ�ȭ
	D3DXMatrixIdentity(&_localMat);

	// Scale
	D3DXMatrixScaling(&_localMat, localScale.x, localScale.y, localScale.z);
	
	// Rotation : Quaternion
	D3DXQUATERNION orientation;
	D3DXQuaternionRotationMatrix(&orientation, &_localMat);
	
	// �� ���� ȸ������ ��������
	float yaw = D3DXToRadian(localRotationEuler.y);
	float pitch = D3DXToRadian(localRotationEuler.x);
	float roll = D3DXToRadian(localRotationEuler.z);

	// ���� ȸ�� ���ʹϾ��� ���ϰ�
	D3DXQuaternionRotationYawPitchRoll(&localRotation, yaw, pitch, roll);
	D3DXQuaternionMultiply(&localRotation, &orientation, &localRotation);

	// Rotation ���ʹϾ��� ����ȭ
	D3DXQuaternionNormalize(&localRotation, &localRotation);

	// ȸ�� ���� ������ ����� ��´�.
	D3DXMatrixRotationQuaternion(&_localMat, &localRotation);

	// ȸ���� ���¿����� �� ���� ���͸� ������
	_xAxis = D3DXVECTOR3(_localMat(0, 0), _localMat(1, 0), _localMat(2, 0));
	_yAxis = D3DXVECTOR3(_localMat(0, 1), _localMat(1, 1), _localMat(2, 1));
	_zAxis = D3DXVECTOR3(_localMat(0, 2), _localMat(1, 2), _localMat(2, 2));
	_viewDir = _zAxis;

	// �׸��� Position�� ����
	_localMat(3, 0) = localPosition.x;
	_localMat(3, 1) = localPosition.y;
	_localMat(3, 2) = localPosition.z;

	// ���� ���� ��Ʈ���� ���ϱ�
	D3DXMATRIX parentWorld;

	if (parent != NULL)
	{
		parentWorld = parent->worldMat;
	}
	else
	{
		D3DXMatrixIdentity(&parentWorld);
	}

	worldMat = _localMat * parentWorld;
	worldPosition.x = worldMat(3, 0);
	worldPosition.y = worldMat(3, 1);
	worldPosition.z = worldMat(3, 2);
}

void Transform::UpdateFrame(float fElapsedTime)
{
	UpdateWorldMatrix();

	for (unsigned int i = 0; i < _childTransforms.size(); ++i)
	{
		_childTransforms[i]->UpdateFrame(fElapsedTime);
	}
}

void Transform::Rotate(float xAngle, float yAngle, float zAngle)
{
	localRotationEuler.x += xAngle;
	if (localRotationEuler.x >= 360.0f)
		localRotationEuler.x -= 360.0f;
	else if (localRotationEuler.x < 0.0f)
		localRotationEuler.x += 360.0f;

	localRotationEuler.y += yAngle;
	if (localRotationEuler.y >= 360.0f)
		localRotationEuler.y -= 360.0f;
	else if (localRotationEuler.y < 0.0f)
		localRotationEuler.y += 360.0f;

	localRotationEuler.z += zAngle;
	if (localRotationEuler.z > 360.0f)
		localRotationEuler.z -= 360.0f;
	else if (localRotationEuler.z < 0.0f)
		localRotationEuler.z += 360.0f;
}

void Transform::Translate(float xPos, float yPos, float zPos)
{
	localPosition.x = xPos;
	localPosition.y = yPos;
	localPosition.z = zPos;
}

void Transform::Scale(float xScale, float yScale, float zScale)
{
	localScale.x = xScale;
	localScale.y = yScale;
	localScale.z = zScale;
}
