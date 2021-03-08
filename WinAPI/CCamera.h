#pragma once

class CObject;
class CCamera
{
	SINGLETON(CCamera)
private:
	Vector3 m_vLook;			// 카메라가 현재 보고있는 위치
	CObject* m_pTargetObject;	// 카메라 타겟 오브젝트
	float m_fSpeed;				// 카메라 속도

	Vector3 m_vDiff;			// 카메라와 LookAt 위치와 메인 윈도우 중심위치 차이

public:
	void Update();

public:
	Vector3 GetRenderPosition(Vector3 _vRealPosition) { return _vRealPosition - m_vDiff; }
	void Move();

public:
	void SetLookAt(const Vector3& _vLook) { m_vLook = _vLook; }
};

