#pragma once
#include "stdafx.h"
#include <iostream>

class CTexture;
class CCore {
	SINGLETON(CCore)
private:
	HWND m_hMainWnd;	  // 메인 윈도우 핸들
	POINT m_ptResolution; // 메인 윈도우 해상도
	HDC m_hDC;			  // DC(Device Context) 핸들

	CTexture* m_pMemTexture; // 더블버퍼링용 메모리 텍스쳐

public:
	int Init(HWND _hMainWnd, POINT _ptResolution);
	void Progress();

public:
	void ResizeWindowScreen(POINT _ptResolution, bool _bActiveMenu);

public:
	HWND GetWndHandle() { return m_hMainWnd; }
	HDC GetDC() { return m_hDC; }
	CTexture* GetMemTexture() { return m_pMemTexture; }

	const POINT& GetResolution() const { return m_ptResolution; }
};

// 게임의 핵심. 게임 관련 담당, 제어.
#pragma region Singleton - dynamic type

// #define _DYNAMIC_SINGLETON
#ifdef _DYNAMIC_SINGLETON
class CCore
{
private:
	static CCore* g_pCore;

private:
	int m_i;

public:
	// 오로지 GetInstance로만 가져올 수 있다.
	static CCore* GetInstance() {
		if (nullptr == g_pCore)
			g_pCore = new CCore();

		return g_pCore;
}

	static void Delete() {
		if (nullptr != g_pCore) {
			delete g_pCore;
			g_pCore = nullptr;
		}
	}
private:
	CCore() {
		void(*pFunc)(void) = &CCore::Delete;
		atexit(pFunc); // atexit(Delete); 와 같음
	}
	~CCore() {}
};
#endif  
#pragma endregion