#pragma once
#include "CObject.h"
class CGTA_Citizen;
class CCamera2D;
class CGTA_CitizenCreator : public CObject
{
private:
	int m_iCreateMargin; // 생성 영역 여백

	CGTA_Citizen* m_pCitizenObjPrefab;
	CCamera2D* m_pCamera;

public:
	virtual void Init() override;
	virtual void Update() override;

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