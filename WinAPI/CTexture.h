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
	void Create(UINT _iWidth, UINT _iHeight);

public:
	HDC GetDC() { return m_hDC; }
	UINT GetWidth() { return m_tBitmap.bmWidth; }
	UINT GetHeight() { return m_tBitmap.bmHeight; }

	HBITMAP& GetBitMap() { return m_hBitmap; }

public:
	CTexture();
	~CTexture();
	
	friend class CResourceManager;
};