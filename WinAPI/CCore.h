#pragma once
#include "stdafx.h"
#include <iostream>

class CTexture;
class CCore {
	SINGLETON(CCore)
private:
	HWND m_hMainWnd;	  // ���� ������ �ڵ�
	POINT m_ptResolution; // ���� ������ �ػ�
	HDC m_hDC;			  // DC(Device Context) �ڵ�

	CTexture* m_pMemTexture; // ������۸��� �޸� �ؽ���

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

// ������ �ٽ�. ���� ���� ���, ����.
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
	// ������ GetInstance�θ� ������ �� �ִ�.
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
		atexit(pFunc); // atexit(Delete); �� ����
	}
	~CCore() {}
};
#endif  
#pragma endregion