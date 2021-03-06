#pragma once
#include "CObject.h"
class CGTA_Citizen;
class CCamera2D;
class CGTA_SuspectSearchSystem;
class CGTA_CitizenCreator : public CObject
{
private:
	int m_iCreateMargin; // 생성 영역 여백

	CCamera2D* m_pCamera;

	int m_iCurCitizenCnt; // 현재 생성된 시민 개수
	int m_imaxCitizenCnt; // 시민 최대 생성 개수

	float m_fCreateCoolTime;
	float m_fMaxCreateCoolTime;
	float m_fDeleteCoolTime;
	float m_fMaxDeleteCoolTime;

	float m_fDeleteDistance; // 삭제할 거리
	float m_fCreatableMinDistance; // 생성할 최대 거리
	float m_fCreatableMaxDistance; // 삭제할 최대 거리

	CObject* m_pPlayer;

	// Level에 따른 비율
	int m_arrCitizenCnt[5];
	int m_arrGunCopCnt[5];
	int m_arrCopCnt[5];
	CGTA_SuspectSearchSystem* m_pSuspectSearchSys;
	
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void SetCamera(CCamera2D* _pCamera) { m_pCamera = _pCamera; }
	CCamera2D* GetCamera() { return m_pCamera; }

public:
	CLONE(CGTA_CitizenCreator);

public:
	CGTA_CitizenCreator() = delete;
	CGTA_CitizenCreator(E_GroupType e_GroupType);
	virtual ~CGTA_CitizenCreator() override;
};