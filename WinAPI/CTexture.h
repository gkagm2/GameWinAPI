#pragma once
#include "CResource.h"
class CTexture : public CResource
{
private:
	HBITMAP m_hBitmap;  // 비트맵 핸들
	HDC m_hDC;		    // 비트맵 DC
	BITMAP m_tBitmap;	// 비트맵 정보 구조체

private:
	virtual int Load(const wchar_t* _pFilePath) override;

public:
	HDC GetDC() { return m_hDC; }
	UINT GetWidth() { return m_tBitmap.bmWidth; }
	UINT GetHeight() { return m_tBitmap.bmHeight; }

public:
	CTexture();
	~CTexture();
};

