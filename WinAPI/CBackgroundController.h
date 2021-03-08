#pragma once
#include "CObject.h"

class CBackground;
class CBackgroundController : public CObject
{
private:
	CTexture* m_pBackgroundTextrue;
	CTexture* m_pBackgroundTextrueF1;
	CTexture* m_pBackgroundTextrueF2;
	CTexture* m_pBackgroundTextrueF3;



private:
	vector<CBackground*> m_vecBackground;

	vector<CBackground*> m_vecBackgroundFront1;
	vector<CBackground*> m_vecBackgroundFront2;
	vector<CBackground*> m_vecBackgroundFront3;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	void Init();

public:
	CBackgroundController() = delete;
	CBackgroundController(E_GroupType e_GroupType);
	virtual ~CBackgroundController() override;
};

