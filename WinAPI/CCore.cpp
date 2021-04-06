#include <vector>
#include "stdafx.h"
#include "CCore.h"
#include "CObject.h"
#include "CPathManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CCollisionManager.h"
#include "CResourceManager.h"
#include "CEventManager.h"
#include "CUIManager.h"
#include "CDebug.h"
#include "CTexture.h"

#ifdef DYNAMIC_SINGLETON
CCore* CCore::g_pCore = nullptr; // static이므로 초기화
#endif

// CObject g_obj;

CCore::CCore() :
	m_hMainWnd(0),
	m_ptResolution{},
	m_hDC(0)
{
}

CCore::~CCore() {
	ReleaseDC(m_hMainWnd, m_hDC); // Windows에서 만든 DC는 ReleaseDC로 지워야 함
}

int CCore::Init(HWND _hMainWnd, POINT _ptResolution)
{
	// Windows Screen setting initialize
	m_hMainWnd = _hMainWnd;

	ResizeWindowScreen(_ptResolution, false); // 윈도우 사이즈 재설정
	
	m_hDC = ::GetDC(m_hMainWnd); // Main windows 창에 대한 dc를 가져옴

	// Manager class initialize
	CPathManager::GetInstance()->Init();
	CTimeManager::GetInstance()->Init();
	CKeyManager::GetInstance()->Init();
	CSceneManager::GetInstance()->Init();
	//CCamera::GetInstance()->Init();

	// Double buffering	(연결해줌)
	m_pMemTexture = CResourceManager::GetInstance()->CreateTexture(STR_TABLE_MemoryTexture, m_ptResolution.x, m_ptResolution.y);

	// TODO : Delete this
	//HBITMAP hPrevBitmap = (HBITMAP)SelectObject(m_memDC, m_bitmap); // 출력 비트맵 할 곳을 설정
	//DeleteObject(hPrevBitmap);

	/* Test Drawing Code
	COLORREF redColor = RGB(255, 0, 0);
	COLORREF blueColor = RGB(0, 0, 255);
	COLORREF greenColor = RGB(0, 255, 0);
	
	HPEN hRedPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));

	HPEN hPrevPen = (HPEN)SelectObject(m_hDC, hRedPen);
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hDC, hBlueBrush);

	Rectangle(m_hDC, 10, 10, 110, 110);

	SelectObject(m_hDC, hPrevPen);
	SelectObject(m_hDC, hPrevBrush);

	DeleteObject(hRedPen);
	DeleteObject(hBlueBrush);*/
	/*HPEN hRedPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));

	HPEN hPrevPen = (HPEN)SelectObject(m_hDC, hRedPen);
	HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_hDC, hBlueBrush);

	Rectangle(m_hDC, 10, 10, 110, 110);

	SelectObject(m_hDC, hPrevPen);
	SelectObject(m_hDC, hPrevBrush);

	DeleteObject(hRedPen);
	DeleteObject(hBlueBrush);*/
	return S_OK;
}

void CCore::Progress()
{
	CTimeManager::GetInstance()->Update();
	CKeyManager::GetInstance()->Update();

	CSceneManager::GetInstance()->Progress();
	//CCamera::GetInstance()->Update();
	
	CCollisionManager::GetInstance()->Update();
	CUIManager::GetInstance()->Update();

	CDebug::GetInstance()->Update();
	
	// 회색 배경으로 렌더링
	//HBRUSH hBrush = CreateSolidBrush(RGB(128, 128, 128));
	//HBRUSH hPrevBrush = (HBRUSH)SelectObject(m_pMemTexture->GetDC(), hBrush);
	Rectangle(m_pMemTexture->GetDC(), -1, -1, m_ptResolution.x+1, m_ptResolution.y+1); // Clear
	//SelectObject(m_pMemTexture->GetDC(), hPrevBrush);
	//DeleteObject(hBrush);

	CSceneManager::GetInstance()->Render(m_pMemTexture->GetDC());
	CTimeManager::GetInstance()->Render(m_pMemTexture->GetDC()); // TODO : delete this
	CDebug::GetInstance()->Render(m_pMemTexture->GetDC());
	//CCamera::GetInstance()->Render(m_pMemTexture->GetDC());

	// Copy Bitmap
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTexture->GetDC(), 0, 0, SRCCOPY);

	// 이벤트 처리
	CEventManager::GetInstance()->Update();
}

void CCore::ResizeWindowScreen(POINT _ptResolution, bool _bActiveMenu)
{
	m_ptResolution = _ptResolution;

	// 화면에 그리는 영역을 지정된 해상도만큼 확보
	RECT rect = { 0, 0, m_ptResolution.x ,m_ptResolution.y };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false); // 메뉴바, 테두리 픽셀까지 계산해줌.
	SetWindowPos(m_hMainWnd, nullptr, 10, 10, rect.right - rect.left, rect.bottom - rect.top, 0);
}