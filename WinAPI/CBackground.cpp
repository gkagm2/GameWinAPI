#include "stdafx.h"
#include "CBackground.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CCore.h"

CBackground::CBackground(E_GroupType e_GroupType) :
	CObject(e_GroupType)
{
	InitTexture();
}

CBackground::~CBackground()
{
}

void CBackground::Update()
{
}

void CBackground::LateUpdate()
{

}

void CBackground::InitTexture() {

}