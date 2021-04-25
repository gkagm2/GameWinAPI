#include "stdafx.h"
#include "CLookAtTestObj.h"
#include "CCamera.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CResource.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CCore.h"
#include "CDebug.h"

CLookAtTestObj::CLookAtTestObj(E_GroupType _eGroupType) :
	CObject(_eGroupType)
{

}

CLookAtTestObj::~CLookAtTestObj()
{
}

void CLookAtTestObj::Init()
{
	CTexture* pTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Monster);
	if (nullptr == pTex) {
		pTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Monster, STR_FILE_PATH_Monster);
	}
	SetTexture(pTex);
	CObject::Init();
}

void CLookAtTestObj::Update()
{
	Vector2 vMousePos = MainCamera->GetScreenToWorldPosition(MousePosition);

	Vector3 targetDir = vMousePos - GetPosition();
	targetDir.Normalized();
	float angle = atan2f(targetDir.y, targetDir.x) * CMyMath::Rad2Deg();
	Debug->Print(Vector2{ 10, 90 }, L"d", angle);
	Vector3 upVec{ 0, -1, 0 };
	upVec = Rotate(upVec, angle + 90);

	HDC dc = CCore::GetInstance()->GetDC();
	MoveToEx(dc, GetPosition().x, GetPosition().y, nullptr);
	LineTo(dc, GetPosition().x + upVec.x * 200.f, GetPosition().y + upVec.y * 200.f);

	//LookAt(Vector3 { vMousePos }, 1000 * DeltaTime);
}

void CLookAtTestObj::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (GetTexture()) {
		POINT rPNT[3];

		Vector3 v1 = GetRectPoint(0);
		Vector3 v2 = GetRectPoint(1);
		Vector3 v3 = GetRectPoint(2);
		rPNT[0].x = (int)(vRenderPosition.x + GetRectPoint(0).x);
		rPNT[0].y = (int)(vRenderPosition.y + GetRectPoint(0).y);
		rPNT[1].x = (int)(vRenderPosition.x + GetRectPoint(1).x);
		rPNT[1].y = (int)(vRenderPosition.y + GetRectPoint(1).y);
		rPNT[2].x = (int)(vRenderPosition.x + GetRectPoint(2).x);
		rPNT[2].y = (int)(vRenderPosition.y + GetRectPoint(2).y);

		HBITMAP bitmap{};
		PlgBlt(_hDC, rPNT, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), bitmap, 8, 5);
	}
}
