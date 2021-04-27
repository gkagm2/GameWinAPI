#pragma once
#include "CCamera.h"

enum class E_CamEffect {
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tCamEffect {
	E_CamEffect eEffect; // 현재 카메라 이펙트
	float fEndTime; // 효과 만료 시간
	float fCurTime; // 효과 진행 시간
};

class CCamera2D : public CCamera
{
private:
	// Camera Effect
	CTexture* m_pVeil;			// 카메라 효과 텍스쳐 (for fade in, out)
	queue<tCamEffect> m_queEffect; // 진행될 카메라 이펙트들
	BYTE m_Alpha;				// Veil texture alpha
	float m_fRatio;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void Move();
	void MoveVehicle();

	void UpdateEffect();
	void AddEffect(E_CamEffect _eCamEffect, float _fEndTime);

public:
	//CLONE(CCamera_Tool);

public:
	CCamera2D() = delete;
	CCamera2D(E_GroupType _eGroupType);
	virtual ~CCamera2D() override;
};