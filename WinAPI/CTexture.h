#pragma once
#include "CResource.h"

class CTexture : public CResource
{
private:
	HBITMAP m_hBitmap;  // ��Ʈ�� �ڵ�
	HDC m_hDC;		    // ��Ʈ�� DC
	BITMAP m_tBitmap;	// ��Ʈ�� ���� ����ü

private:
	virtual int Load(const wchar_t* _pFilePath) override;

public:
	void Create(UINT _iWidth, UINT _iHeight);

public:
	HDC& GetDC() { return m_hDC; }
	UINT GetWidth() { return m_tBitmap.bmWidth; }
	UINT GetHeight() { return m_tBitmap.bmHeight; }

	HBITMAP& GetBitMap() { return m_hBitmap; }

	TRGB GetPixel(UINT _x, UINT _y);
	void SetPixel(UINT _x, UINT _y, const TRGB& _tColor);

public:
	CTexture();
	~CTexture();

	friend class CResourceManager;
};