#pragma once
#include "CObject.h"

class CTexture;
class CCamera : CObject
{
protected:
	Vector3 m_vLook;			// ī�޶� ���� �����ִ� ��ġ
	CObject* m_pTargetObject;	// ī�޶� Ÿ�� ������Ʈ
	float m_fSpeed;				// ī�޶� �ӵ�

	Vector3 m_vDiff;			// ī�޶�� LookAt ��ġ�� ���� ������ �߽���ġ ����

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