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
	CRigidbody::PrevUpdate();
}

void CRigidbody2D::Update()
{
	CRigidbody::Update();
}

void CRigidbody2D::LateUpdate()
{
	CRigidbody::LateUpdate();
}

