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
	Vector3 m_vLook;			// 카메라가 현재 보고있는 위치
	CObject* m_pTargetObject;	// 카메라 타겟 오브젝트
	float m_fSpeed;				// 카메라 속도

	Vector3 m_vDiff;			// 카메라와 LookAt 위치와 메인 윈도우 중심위치 차이
	
	// Camera Effect
	CTexture* m_pVeil;			// 카메라 효과 텍스쳐 (for fade in, out)
	E_CamEffect m_eCurEffectState;	// 현재 카메라 이펙트
	float m_fEndEffectTime;		// 효과 만료 시간
	float m_fCurEffectTime;		// 현재 효과 진행 시간
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