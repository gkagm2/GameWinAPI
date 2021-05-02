#include "CGTA_ExplosionEffect.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CTexture.h"
#include "CAnimator.h"
#include "CAnimation.h"
CGTA_ExplosionEffect::CGTA_ExplosionEffect(E_GroupType _eGroupType) :
	CObject(_eGroupType)
{
}

CGTA_ExplosionEffect::~CGTA_ExplosionEffect()
{
}

void CGTA_ExplosionEffect::Init()
{
	CTexture* pEffectTex = CResourceManager::GetInstance()->GetTexture(STR_FILE_PATH_gta_ExplosionEffect, STR_FILE_PATH_gta_ExplosionEffect);
	SetTexture(pEffectTex);

	SetScale(350, 350, 0);

	SetAnimator(new CAnimator(this));
	GetAnimator()->CreateAnimation(L"explosion", pEffectTex, Vector2(0, 0), Vector2(200, 200), 16, 0.05f);
	GetAnimator()->PlayAnimation(L"explosion", E_AnimationPlayType::ONCE);
	CObject::Init();
}

void CGTA_ExplosionEffect::Update()
{
	if (GetAnimator()->GetCurAnimation()->IsFinish())
		DestroyObject(this);
}

void CGTA_ExplosionEffect::Render(HDC _hDC)
{
	CObject::Render(_hDC);
}