#include "Transform.h"

Transform::Transform()
{
	D3DXMatrixIdentity(&worldMat);						// 월드 매트릭스 단위행렬로 초기화

	localScale.x = localScale.y = localScale.z = 1.0f;	// Scale 초기화;

	localRotationEuler.x = 0.0f;						// localRotation 각 초기화;
	localRotationEuler.y = 0.0f;
	localRotationEuler.z = 0.0f;

	worldPosition.x = localPosition.x = 0.f;			// 월드 position과 로컬 position 모두 원점으로
	worldPosition.y = localPosition.y = 0.f;
	worldPosition.z = localPosition.z = 0.f;

	D3DXQuaternionIdentity(&worldRotation);				// Rotation도 단위 쿼터니언으로 초기화
	D3DXQuaternionIdentity(&localRotation);

	parent = NULL;										// 부모 오브젝트는 NULL
	_childTransforms.clear();							// 자식 오브젝트도 비워줍니다.
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
	// 자신의 로컬 매트릭스 -----------------------------

	// 초기화
	D3DXMatrixIdentity(&_localMat);

	// Scale
	D3DXMatrixScaling(&_localMat, localScale.x, localScale.y, localScale.z);
	
	// Rotation : Quaternion
	D3DXQUATERNION orientation;
	D3DXQuaternionRotationMatrix(&orientation, &_localMat);
	
	// 각 축의 회전값을 라디안으로
	float yaw = D3DXToRadian(localRotationEuler.y);
	float pitch = D3DXToRadian(localRotationEuler.x);
	float roll = D3DXToRadian(localRotationEuler.z);

	// 최종 회전 쿼터니언을 구하고
	D3DXQuaternionRotationYawPitchRoll(&localRotation, yaw, pitch, roll);
	D3DXQuaternionMultiply(&localRotation, &orientation, &localRotation);

	// Rotation 쿼터니언의 정규화
	D3DXQuaternionNormalize(&localRotation, &localRotation);

	// 회전 까지 구해진 행렬을 얻는다.
	D3DXMatrixRotationQuaternion(&_localMat, &localRotation);

	// 회전된 상태에서의 각 축의 벡터를 구하자
	_xAxis = D3DXVECTOR3(_localMat(0, 0), _localMat(1, 0), _localMat(2, 0));
	_yAxis = D3DXVECTOR3(_localMat(0, 1), _localMat(1, 1), _localMat(2, 1));
	_zAxis = D3DXVECTOR3(_localMat(0, 2), _localMat(1, 2), _localMat(2, 2));
	_viewDir = _zAxis;

	// 그리고 Position을 적용
	_localMat(3, 0) = localPosition.x;
	_localMat(3, 1) = localPosition.y;
	_localMat(3, 2) = localPosition.z;

	// 최종 월드 매트릭스 구하기
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
