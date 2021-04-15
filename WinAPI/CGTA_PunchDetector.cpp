#include "stdafx.h"
#include "CGTA_PunchDetector.h"

CGTA_PunchDetector::CGTA_PunchDetector(E_GroupType _eGroupType) :
	CObject(_eGroupType)
{
}

CGTA_PunchDetector::~CGTA_PunchDetector()
{
}

void CGTA_PunchDetector::Init()
{
	CObject::Init();
}

void CGTA_PunchDetector::Update()
{
}

void CGTA_PunchDetector::OnCollisionEnter(CObject* _pOther)
{
}

void CGTA_PunchDetector::OnCollisionStay(CObject* _pOther)
{
}

void CGTA_PunchDetector::OnCollisionExit(CObject* _pOther)
{
}