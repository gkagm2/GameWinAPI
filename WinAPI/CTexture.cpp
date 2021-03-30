#include "stdafx.h"
#include "CTexture.h"
#include "CCore.h"

CTexture::CTexture() :
	m_hBitmap(nullptr),
	m_hDC(nullptr),
	m_tBitmap{}
{
}

CTexture::~CTexture()
{
	DeleteObject(m_hBitmap);
	DeleteDC(m_hDC);
}

int CTexture::Load(const wchar_t* _pFilePath)
{
	// ��Ʈ�� �ε�
	m_hBitmap = (HBITMAP)LoadImage(nullptr, _pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (nullptr == m_hBitmap)
		return E_FAIL;

	// ��Ʈ�ʰ� �������� DC ����
	m_hDC = CreateCompatibleDC(CCore::GetInstance()->GetDC());

	// ��Ʈ�ʰ� DC ����
	HBITMAP hPrevBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
	DeleteObject(hPrevBitmap);

	// �ε��� ��Ʈ���� ���� ���
	m_tBitmap = {};
	GetObject(m_hBitmap, sizeof(BITMAP), (void*)&m_tBitmap);

	return S_OK;
}

void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	assert(!m_hBitmap);
	assert(!m_hDC);

	// ��Ʈ��, DC����
	m_hBitmap = (HBITMAP)CreateCompatibleBitmap(CCore::GetInstance()->GetDC(), (int)_iWidth, (int)_iHeight);
	m_hDC = (HDC)CreateCompatibleDC(CCore::GetInstance()->GetDC());

	// ��Ʈ�ʰ� DC����
	HBITMAP hPrevBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
	DeleteObject(hPrevBitmap);

	// ��Ʈ���� ���� ���
	m_tBitmap = {};
	GetObject(m_hBitmap, sizeof(BITMAP), (void*)&m_tBitmap);
}