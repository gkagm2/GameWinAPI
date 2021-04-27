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
	CObject(_eGroupType),
	angle(0.f)
{

}

CLookAtTestObj::~CLookAtTestObj()
{
}

void CLookAtTestObj::Init()
{
	CTexture* pTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_PATH_Vehicle_car_alpha);
	if (nullptr == pTex) {
		pTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_PATH_Vehicle_car_alpha, STR_FILE_PATH_Vehicle_car_alpha);
	}
	SetTexture(pTex);
	CObject::Init();
}

void CLookAtTestObj::Update()
{
	Vector2 vMousePos = MainCamera->GetScreenToWorldPosition(MousePosition);

	Vector3 targetDir = vMousePos - GetPosition();
	targetDir.Normalized();

	angle = atan2f(targetDir.y, targetDir.x) * CMyMath::Rad2Deg(); // 타겟 방향으로 회전 한 각 구하기
	
	Debug->Print(Vector2{ 10, 90 }, L"d", angle);
	Vector3 upVec{ 0, -1, 0 };
	upVec = Rotate(upVec, angle + 90);
	Vector3 rightVec = Rotate(upVec, 90);

	SetRotateDegree(angle);

	HDC dc = CCore::GetInstance()->GetDC();
	MoveToEx(dc, GetPosition().x, GetPosition().y, nullptr);
	LineTo(dc, GetPosition().x + upVec.x * 200.f, GetPosition().y + upVec.y * 200.f);

	MoveToEx(dc, GetPosition().x, GetPosition().y, nullptr);
	LineTo(dc, GetPosition().x + rightVec.x * 180.f, GetPosition().y + rightVec.y * 180.f);

	//LookAt(Vector3 { vMousePos }, 1000 * DeltaTime);
}

void CLookAtTestObj::Render(HDC _hDC)
{
	if (false == IsRender())
		return;

	Vector3 vRenderPosition = MainCamera->GetRenderPosition(GetPosition());

	if (GetTexture()) {
		RenderRotatedTex(_hDC, vRenderPosition, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight());
	}
}