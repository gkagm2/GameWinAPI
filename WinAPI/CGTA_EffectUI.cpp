#include "stdafx.h"
#include "CGTA_EffectUI.h"
#include "CCamera.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CCamera.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CTexture.h"
#include "CTimeManager.h"

CGTA_EffectUI::CGTA_EffectUI(E_GroupType _eGroupType) :
	CTextUI(_eGroupType),
	m_fOffCoolTime(0.f),
	m_fOffMaxCoolTime(99.f),
	m_fDestroyScale(200.f),
	m_fSpeed(300.f)
{
	SetTexture(CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_GTA_Font_A, STR_FILE_PATH_GTA_Font_A));
	SetRenderMode(CTextUI::E_RenderMode::alphaBlend);
	SetTextUIPivot(CTextUI::E_TextUIPivot::center);
}

CGTA_EffectUI::~CGTA_EffectUI()
{
}

void CGTA_EffectUI::Init()
{
}

void CGTA_EffectUI::Update()
{
	if (GetScale().x > m_fDestroyScale)
		DestroyObject(this);

	SetScale(GetScale() + DeltaTime * m_fSpeed);
	{
		float  fStart = GetScale().x;
		float fEnd = m_fDestroyScale;
		float fRatio = 1.f - fStart / fEnd;
		if(fRatio < 0.1f)
			fRatio = 0.f;
		SetAlpha((BYTE)(255.f * fRatio));
	}
}

void CGTA_EffectUI::SetEffectPos(const Vector3& _vTargetPos)
{
	SetPosition(MainCamera->GetRenderPosition(_vTargetPos));
}
