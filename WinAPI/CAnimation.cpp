#include "stdafx.h"
#include "CAnimation.h"

#include "CScene.h"
#include "CSceneManager.h"

#include "CResourceManager.h"
#include "CResource.h"
#include "CTexture.h"

#include "CCamera.h"
#include "CObject.h"
#include "CAnimator.h"
#include "CTimeManager.h"

#include "CVehicle.h"

// Test
#include "CKeyManager.h"

CAnimation::CAnimation() :
	m_pAnimator(nullptr),
	m_strName{},
	m_pTexture(nullptr),
	m_iCurFrameIdx(0),
	m_fAccumulationTime(0.0f),
	m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::LateUpdate()
{
	if (m_bFinish)
		return;

	m_fAccumulationTime += DeltaTime;

	// �ʰ����� ���� �������� �����ð��� �� �Ѿ�� ��� �ݺ������� ����ؼ� 
	// ���� �ð��뿡 �´� �����ӱ��� ������Ŵ
	while (m_fAccumulationTime > m_vecFrame[m_iCurFrameIdx].fDuration) {
		m_fAccumulationTime -= m_vecFrame[m_iCurFrameIdx].fDuration;
		++m_iCurFrameIdx;

		if (m_iCurFrameIdx >= m_vecFrame.size()) {
			m_bFinish = true;
			--m_iCurFrameIdx;
			break;
		}
	}
}

void CAnimation::Render(HDC _hDC)
{
	Vector3 vPosition = m_pAnimator->GetOwnerObject()->GetPosition();
	Vector3 vRenderPosition = MainCamera->GetRenderPosition(vPosition);

	UINT iWidth = (UINT)m_pAnimator->GetOwnerObject()->GetScale().x;
	UINT iHeight = (UINT)m_pAnimator->GetOwnerObject()->GetScale().y;

	CVehicle* pVehicle = dynamic_cast<CVehicle*>(m_pAnimator->GetOwnerObject());
	if (pVehicle) {
		/*m_pAnimator->GetOwnerObject()->RotateInfo().Update();
		if (InputKeyHold(E_Key::LEFT)) {
			m_pAnimator->GetOwnerObject()->RotateObj(120 * DeltaTime);
		}
		if (InputKeyHold(E_Key::RIGHT)) {
			m_pAnimator->GetOwnerObject()->RotateObj(-120 * DeltaTime);
		}

		POINT rPNT[3];
		rPNT[0].x = m_pAnimator->GetOwnerObject()->RotateInfo().m_vRectPoint[0].x;
		rPNT[0].y = m_pAnimator->GetOwnerObject()->RotateInfo().m_vRectPoint[0].y;
		rPNT[1].x = m_pAnimator->GetOwnerObject()->RotateInfo().m_vRectPoint[1].x;
		rPNT[1].y = m_pAnimator->GetOwnerObject()->RotateInfo().m_vRectPoint[1].y;
		rPNT[2].x = m_pAnimator->GetOwnerObject()->RotateInfo().m_vRectPoint[2].x;
		rPNT[2].y = m_pAnimator->GetOwnerObject()->RotateInfo().m_vRectPoint[2].y;

		HBITMAP bitmap{};
		PlgBlt(_hDC, rPNT, m_pAnimator->GetOwnerObject()->GetTexture()->GetDC(), int(m_vecFrame[m_iCurFrameIdx].vLT.x), int(m_vecFrame[m_iCurFrameIdx].vLT.y), int(m_vecFrame[m_iCurFrameIdx].vSlice.x), int(m_vecFrame[m_iCurFrameIdx].vSlice.y), bitmap, 0, 0);*/
	}
	else {
		TransparentBlt(
			_hDC,
			(int)(vRenderPosition.x - iWidth / 2.0f), (int)(vRenderPosition.y - iHeight / 2.0f),
			iWidth, iHeight,
			m_pTexture->GetDC(),
			int(m_vecFrame[m_iCurFrameIdx].vLT.x),
			int(m_vecFrame[m_iCurFrameIdx].vLT.y),
			int(m_vecFrame[m_iCurFrameIdx].vSlice.x),
			int(m_vecFrame[m_iCurFrameIdx].vSlice.y),
			EXCEPTION_COLOR_RGB_BLACK);
	}
	
	//TransparentBlt(
	//		_hDC,																/*_hDC*/
	//		int(vRenderPosition.x - m_vecFrame[m_iCurFrameIdx].vSlice.x / 2) + m_vecFrame[m_iCurFrameIdx].vOffset.x,	/*��� �簢�� ���� �� �𼭸��� x ��ǥ (�� ����)*/
	//		int(vRenderPosition.y - m_vecFrame[m_iCurFrameIdx].vSlice.y / 2) + m_vecFrame[m_iCurFrameIdx].vOffset.y,	/*��� �簢���� ���� �� �𼭸��� ���� �� ������ y ��ǥ*/
	//		int(m_vecFrame[m_iCurFrameIdx].vSlice.x),							/*��� �簢���� �ʺ� (�� ����)*/
	//		int(m_vecFrame[m_iCurFrameIdx].vSlice.y),							/*��� �簢���� ���� (�� ����)*/
	//		m_pTexture->GetDC(),												/*�ҽ� ��ġ ���ؽ�Ʈ�� ���� �ڵ�*/
	//		int(m_vecFrame[m_iCurFrameIdx].vLT.x),								/*�ҽ� �簢���� x ��ǥ (�� ����)*/
	//		int(m_vecFrame[m_iCurFrameIdx].vLT.y),								/*�ҽ� ���簢���� �� ������ ǥ�õ� y ��ǥ*/
	//		int(m_vecFrame[m_iCurFrameIdx].vSlice.x),							/*�ҽ� ���簢���� �ʺ� (�� ����)*/
	//		int(m_vecFrame[m_iCurFrameIdx].vSlice.y),							/*�ҽ� ���簢���� ����(�� ����)*/
	//		EXCEPTION_COLOR_RGB);												/*�����ϰ� ó�� �� �ҽ� ��Ʈ ���� RGB ����*/
}

void CAnimation::Create(const wstring& _strName, CTexture* _pTexture, Vector2 _vLT, Vector2 _vSlice, int _iMaxFrame, float _fDuration)
{
	m_strName = _strName;
	m_pTexture = _pTexture;

	tAnimFrame frame{};

	for (int i = 0; i < _iMaxFrame; ++i) {
		frame.fDuration = _fDuration;
		frame.vLT.x = _vLT.x + _vSlice.x * i;
		frame.vLT.y = _vLT.y;
		frame.vSlice = _vSlice;
		frame.fDuration = _fDuration;
		m_vecFrame.push_back(frame);
	}
}

// GTA
void CAnimation::Create(const wstring& _strName, CTexture* _pTexture, Vector2 _vLT, Vector2 _vSlice, Vector2 _vInsidePadding, int _iMaxFrame, float _fDuration)
{
	m_strName = _strName;
	m_pTexture = _pTexture;

	tAnimFrame frame{};

	for (int i = 0; i < _iMaxFrame; ++i) {
		frame.fDuration = _fDuration;
		if (i > 0) {
			frame.vLT.x = _vLT.x + _vSlice.x * i + _vInsidePadding.x;
			frame.vLT.y = _vLT.y + _vInsidePadding.y; // TODO : ���� �̻��ѵ�?
		}
		else {
			frame.vLT.x = _vLT.x + _vSlice.x * i;
			frame.vLT.y = _vLT.y + _vInsidePadding.y;
		}
		frame.vSlice = _vSlice;
		frame.fDuration = _fDuration;
		m_vecFrame.push_back(frame);
	}
}


void CAnimation::Save(FILE* _pFile)
{
	// Animation Name
	SaveWString(m_strName, _pFile);
	assert(m_pTexture);

	// ���� �ؽ���
	SaveWString(m_pTexture->GetKey(), _pFile);
	SaveWString(m_pTexture->GetRelativePath(), _pFile);

	// �� ������ ������
	size_t iFrameCount = m_vecFrame.size();
	fwrite(&iFrameCount, sizeof(int), 1, _pFile);
	fwrite(m_vecFrame.data(), sizeof(tAnimFrame), iFrameCount, _pFile); //vector.data() ù �������� �ּҸ� ����
}

void CAnimation::Load(FILE* _pFile)
{
	LoadWString(m_strName, _pFile);

	// ���� �ؽ���
	wstring strKey, strRelativePath;
	LoadWString(strKey, _pFile);
	LoadWString(strRelativePath, _pFile);

	m_pTexture = CResourceManager::GetInstance()->FindTexture(strKey);
	if (nullptr == m_pTexture)
		m_pTexture = CResourceManager::GetInstance()->LoadTexture(strKey, strRelativePath);

	// �� ������ ������
	int iFrameCount = 0;
	fread(&iFrameCount, sizeof(int), 1, _pFile);

	tAnimFrame tFrame{};
	for (int i = 0; i < iFrameCount; ++i) {
		
		fread(&tFrame, sizeof(tAnimFrame), 1, _pFile);
		m_vecFrame.push_back(tFrame);
	}
}