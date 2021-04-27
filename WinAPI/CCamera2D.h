#pragma once
#include "CCamera.h"

enum class E_CamEffect {
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct tCamEffect {
	E_CamEffect eEffect; // ���� ī�޶� ����Ʈ
	float fEndTime; // ȿ�� ���� �ð�
	float fCurTime; // ȿ�� ���� �ð�
};

class CCamera2D : public CCamera
{
private:
	// Camera Effect
	CTexture* m_pVeil;			// ī�޶� ȿ�� �ؽ��� (for fade in, out)
	queue<tCamEffect> m_queEffect; // ����� ī�޶� ����Ʈ��
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