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
	HDC GetDC() { return m_hDC; }
	UINT GetWidth() { return m_tBitmap.bmWidth; }
	UINT GetHeight() { return m_tBitmap.bmHeight; }

public:
	CTexture();
	~CTexture();
};

