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

//회전 함수 정의
void CLookAtTestObj::RotateSizingImage(HDC hdc, HBITMAP hBmp, RECT rt,
	double dblAngle,
	int ixRotateAxis, int iyRotateAxis,
	int ixDisplay, int iyDisplay,
	double dblSizeRatio = 1,
	HBITMAP hMaskBmp = NULL, int ixMask = 0, int iyMask = 0) {
	int i;
	BITMAP bm;
	GetObject(hBmp, sizeof(BITMAP), &bm);
	POINT apt[3] = { 0 };
	double dblWidth = (double)bm.bmWidth * dblSizeRatio;
	double dblHeight = (double)bm.bmHeight * dblSizeRatio;
	double ixRotate = (int)((double)ixRotateAxis * dblSizeRatio); // 크기가 변하는 것 고려
	double iyRotate = (int)((double)iyRotateAxis * dblSizeRatio);
	const double pi = 3.14159265358979323846;

	double dblRadian, dblx, dbly, dblxDest, dblyDest, cosVal, sinVal;
	dblRadian = dblAngle * pi / 180.0f;
	cosVal = cos(dblRadian), sinVal = sin(dblRadian);
	// 1. 회전축을 기준으로 상대좌표를 구하고 
	// 2. 회전후 위치좌표(상대좌표)를 얻은 후
	// 3. 얻은 값을 원래의 좌표에 적용.
	for (i = 0; i < 3; i++) {
		if (i == 0) { dblx = -ixRotate, dbly = -iyRotate; }    // left up  꼭지점 부분
		else if (i == 1) { dblx = dblWidth - ixRotate, dbly = -iyRotate; }  // right up 꼭지점 부분
		else if (i == 2) { dblx = -ixRotate, dbly = dblHeight - iyRotate; } // left low 꼭지점 부분
		dblxDest = dblx * cosVal - dbly * sinVal;
		dblyDest = dblx * sinVal + dbly * cosVal;
		dblxDest += ixRotate, dblyDest += iyRotate;
		apt[i].x = ixDisplay - (long)ixRotate + (long)dblxDest;
		apt[i].y = iyDisplay - (long)iyRotate + (long)dblyDest;
	}
	HDC memdc, memdc2, memdc3;
	HBITMAP hBit2, hBit3;

	memdc = CreateCompatibleDC(hdc);
	memdc2 = CreateCompatibleDC(hdc);
	memdc3 = CreateCompatibleDC(hdc);


	hBit2 = CreateCompatibleBitmap(hdc, GetTexture()->GetWidth(), GetTexture()->GetHeight());
	hBit3 = CreateCompatibleBitmap(hdc, GetTexture()->GetWidth(), GetTexture()->GetHeight());

	SelectObject(memdc, hBmp);
	SelectObject(memdc2, hBit2);
	SelectObject(memdc3, hBit3);

	FillRect(memdc2, &rt, (HBRUSH)NULL_BRUSH);
	FillRect(memdc3, &rt, (HBRUSH)NULL_BRUSH);

	TransparentBlt(memdc2, 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), memdc, 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), EXCEPTION_COLOR_RGB_BLACK);//<-이 RGB값은 출력할 비트맵의 알파값(?)수정해서 사용.
	PlgBlt(memdc3, apt, memdc2, 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), hMaskBmp, ixMask, iyMask);
	TransparentBlt(hdc, 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), memdc3, 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), RGB(0, 0, 0));//<-이 RGB는 건드리지않는다. (original rgb : 255, 255 ,255)
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

		HBITMAP bitmap = GetTexture()->GetBitMap();
		/*HBITMAP hbmMask = CreateBitmap(800, 800, 1, 1, NULL);
		
		RECT rect{ 0, 0, GetTexture()->GetWidth() ,GetTexture()->GetHeight() };*/
		//RotateSizingImage(_hDC, hbmMask, rect, 60.0, 0, 0, 500, 500, 1.0, NULL, 0, 0);

		//HBITMAP bitmap{};
		//HDC memDC1 = CreateCompatibleDC(_hDC);
		//HDC memDC2 = CreateCompatibleDC(_hDC);

		//HBITMAP hBit2, hBit3;
		//hBit2 = CreateCompatibleBitmap(_hDC, GetTexture()->GetWidth(), GetTexture()->GetHeight());
		//hBit3 = CreateCompatibleBitmap(_hDC, GetTexture()->GetWidth(), GetTexture()->GetHeight());

		////SelectObject(memdc, hBmp);
		//SelectObject(memDC1, hBit2);
		//SelectObject(memDC2, hBit3);

		//RECT rt{ vRenderPosition.x,vRenderPosition.y, vRenderPosition.x + GetTexture()->GetWidth(), vRenderPosition.y + GetTexture()->GetHeight()};
		//FillRect(memDC1, &rt, (HBRUSH)NULL_BRUSH);
		//FillRect(memDC2, &rt, (HBRUSH)NULL_BRUSH);


		//BLENDFUNCTION tBlendFunc;
		//tBlendFunc.BlendOp = AC_SRC_OVER;
		//tBlendFunc.BlendFlags = 0;					// 반드시 0
		//tBlendFunc.SourceConstantAlpha = 0;	// 0 : 투명, 255 불투명
		//tBlendFunc.AlphaFormat = AC_SRC_ALPHA;					// AC_SRC_ALPHA

		//AlphaBlend(memDC2,
		//	(int)(vRenderPosition.x - GetTexture()->GetWidth() * 0.5f), (int)(vRenderPosition.y - GetTexture()->GetHeight() * 0.5f), (int)GetTexture()->GetWidth(), (int)GetTexture()->GetHeight(),
		//	memDC1,
		//	0, 0, (int)GetTexture()->GetWidth(), (int)GetTexture()->GetHeight(),
		//	tBlendFunc);

		HDC hRotDC = CreateCompatibleDC(_hDC);

		HBITMAP hBit2, hBit3;
		hBit2 = CreateCompatibleBitmap(_hDC, GetTexture()->GetWidth(), GetTexture()->GetHeight());
		 
		//SelectObject(memdc, hBmp);
		SelectObject(hRotDC, hBit2);


		PlgBlt(hRotDC, rPNT, GetTexture()->GetDC(), 0, 0, GetTexture()->GetWidth(), GetTexture()->GetHeight(), bitmap, 0, 0);
		GdiTransparentBlt(_hDC, vRenderPosition.x, vRenderPosition.y, 100, 100, hRotDC, 0, 0, 100, 100, EXCEPTION_COLOR_RGB_BLACK); // 투명화

	}
}