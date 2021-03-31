#pragma once

enum class E_CamEffect {
	FADE_IN,
	FADE_OUT,
	NONE,
};

class CObject;
class CTexture;
class CCamera
{
	SINGLETON(CCamera)
private:
	Vector3 m_vLook;			// ī�޶� ���� �����ִ� ��ġ
	CObject* m_pTargetObject;	// ī�޶� Ÿ�� ������Ʈ
	float m_fSpeed;				// ī�޶� �ӵ�

	Vector3 m_vDiff;			// ī�޶�� LookAt ��ġ�� ���� ������ �߽���ġ ����
	
	// Camera Effect
	CTexture* m_pVeil;			// ī�޶� ȿ�� �ؽ��� (for fade in, out)
	E_CamEffect m_eCurEffectState;	// ���� ī�޶� ����Ʈ
	float m_fEndEffectTime;		// ȿ�� ���� �ð�
	float m_fCurEffectTime;		// ���� ȿ�� ���� �ð�
	BYTE m_Alpha;				// Veil texture alpha
	float m_fRatio;

public:
	void Init();
	void Update();
	void Render(HDC _hDC);

public:
	Vector3 GetRenderPosition(Vector3 _vRealPosition) { return _vRealPosition - m_vDiff; }
	Vector2 GetRenderPosition(Vector2 _vRealPosition) { return _vRealPosition - m_vDiff; }
	Vector3 GetScreenToWorldPosition(Vector3 _vScreenPosition) { return _vScreenPosition + m_vDiff; }
	Vector2 GetScreenToWorldPosition(Vector2 _vScreenPosition) { return _vScreenPosition + m_vDiff; }
	void Move();
	void UpdateEffect();
	void SetEffect(E_CamEffect _eCamEffect, float _fTime);

public:
	void SetLookAt(const Vector3& _vLook) { m_vLook = _vLook; }
	void SetLookAt(const Vector2& _vLook) { m_vLook = _vLook; }
};