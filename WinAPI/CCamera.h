#pragma once

class CObject;
class CCamera
{
	SINGLETON(CCamera)
private:
	Vector3 m_vLook;			// ī�޶� ���� �����ִ� ��ġ
	CObject* m_pTargetObject;	// ī�޶� Ÿ�� ������Ʈ
	float m_fSpeed;				// ī�޶� �ӵ�

	Vector3 m_vDiff;			// ī�޶�� LookAt ��ġ�� ���� ������ �߽���ġ ����

public:
	void Update();

public:
	Vector3 GetRenderPosition(Vector3 _vRealPosition) { return _vRealPosition - m_vDiff; }
	void Move();

public:
	void SetLookAt(const Vector3& _vLook) { m_vLook = _vLook; }
};

