#pragma once
#include "CObject.h"

class CTexture;
class CCamera : CObject
{
protected:
	Vector3 m_vLook;			// 카메라가 현재 보고있는 위치
	CObject* m_pTargetObject;	// 카메라 타겟 오브젝트
	float m_fSpeed;				// 카메라 속도

	Vector3 m_vDiff;			// 카메라와 LookAt 위치와 메인 윈도우 중심위치 차이

public:
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC _hDC);

public:
	Vector3 GetRenderPosition(Vector3 _vRealPosition) { return _vRealPosition - m_vDiff; }
	Vector2 GetRenderPosition(Vector2 _vRealPosition) { return _vRealPosition - m_vDiff; }
	Vector3 GetScreenToWorldPosition(Vector3 _vScreenPosition) { return _vScreenPosition + m_vDiff; }
	Vector2 GetScreenToWorldPosition(Vector2 _vScreenPosition) { return _vScreenPosition + m_vDiff; }
	void SetMainCamera();

public:
	void SetLookAt(const Vector3& _vLook) { m_vLook = _vLook; }
	void SetLookAt(const Vector2& _vLook) { m_vLook = _vLook; }

	void SetTargetObject(CObject* _pTargetObject) { m_pTargetObject = _pTargetObject; }

public:
	CLONE(CCamera);
public:
	CCamera() = delete;
	CCamera(E_GroupType _eGroupType);
	virtual ~CCamera() override;
};