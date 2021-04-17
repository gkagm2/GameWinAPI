#include "stdafx.h"
#include "CRigidbody2D.h"

CRigidbody2D::CRigidbody2D(CObject* _pTargetObj) :
	CRigidbody(_pTargetObj)
{
}

CRigidbody2D::~CRigidbody2D()
{
}

void CRigidbody2D::PrevUpdate()
{
	if (false == GetActive())
		return;
	CRigidbody::PrevUpdate();
}

void CRigidbody2D::Update()
{
	if (false == GetActive())
		return;
	CRigidbody::Update();
}

void CRigidbody2D::LateUpdate()
{
	if (false == GetActive())
		return;
	CRigidbody::LateUpdate();
}

