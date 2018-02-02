#include "Transform.h"

Transform::Transform()
{
	D3DXMatrixIdentity(&_worldMat);				// 월드 매트릭스 단위행렬로 초기화

	_worldPosition.x = _localPosition.x = 0.f;	// 월드 position과 로컬 position 모두 원점으로
	_worldPosition.y = _localPosition.y = 0.f;
	_worldPosition.z = _localPosition.z = 0.f;

	D3DXQuaternionIdentity(&_worldRotation);	// Rotation도 단위 쿼터니언으로 초기화
	D3DXQuaternionIdentity(&_localRotation);

	parent = NULL;								// 부모 오브젝트는 NULL
	_childTransform.clear();					// 자식 오브젝트도 비워줍니다.
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
	// 자신의 로컬 매트릭스 -----------------------------

	// 초기화
	D3DXMatrixIdentity(&_localMat);

	// Scale
	D3DXMatrixScaling(&_localMat, _localScale.x, _localScale.y, _localScale.z);
	
	// Rotation : Quaternion
	D3DXQUATERNION orientation;
	D3DXQuaternionRotationMatrix(&orientation, &_localMat);
	
	// 각 축의 회전값을 라디안으로
	float yaw = D3DXToRadian(localRotationEuler.y);
	float pitch = D3DXToRadian(localRotationEuler.x);
	float roll = D3DXToRadian(localRotationEuler.z);

	// 최종 회전 쿼터니언을 구하고
	D3DXQuaternionRotationYawPitchRoll(&_localRotation, yaw, pitch, roll);
	D3DXQuaternionMultiply(&_localRotation, &orientation, &_localRotation);

	// Rotation 쿼터니언의 정규화
	D3DXQuaternionNormalize(&_localRotation, &_localRotation);

	// 회전 까지 구해진 행렬을 얻는다.
	D3DXMatrixRotationQuaternion(&_localMat, &_localRotation);

	// 회전된 상태에서의 각 축의 벡터를 구하자
	_xAxis = D3DXVECTOR3(_localMat(0, 0), _localMat(1, 0), _localMat(2, 0));
	_yAxis = D3DXVECTOR3(_localMat(0, 1), _localMat(1, 1), _localMat(2, 1));
	_zAxis = D3DXVECTOR3(_localMat(0, 2), _localMat(1, 2), _localMat(2, 2));
	_viewDir = _zAxis;

	// 그리고 Position을 적용
	_localMat(3, 0) = -D3DXVec3Dot(&_xAxis, &_localPosition);
	_localMat(3, 1) = -D3DXVec3Dot(&_yAxis, &_localPosition);
	_localMat(3, 2) = -D3DXVec3Dot(&_zAxis, &_localPosition);

	// 최종 월드 매트릭스 구하기
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
