#include "stdafx.h"
#include "CVehicle.h"

CVehicle::CVehicle(E_GroupType _eGroupType) :
	CObject(_eGroupType = E_GroupType::VEHICLE)
{
}

CVehicle::~CVehicle()
{
}

void CVehicle::Update()
{
}