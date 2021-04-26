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