#include "stdafx.h"
#include "CAnimation.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CObject.h"
#include "CAnimator.h"
#include "CTimeManager.h"

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

	if (m_fAccumulationTime > m_vecFrame[m_iCurFrameIdx].fDuration) {
		++m_iCurFrameIdx;

		if (m_iCurFrameIdx >= m_vecFrame.size()) {
			m_bFinish = true;
			--m_iCurFrameIdx;
		}
		m_fAccumulationTime = 0.0f;
	}
}

void CAnimation::Render(HDC _hDC)
{
	Vector3 vPosition = m_pAnimator->GetOwnerObject()->GetPosition();
	Vector3 vRenderPosition = CCamera::GetInstance()->GetRenderPosition(vPosition);

	TransparentBlt(
			_hDC,																/*_hDC*/
			int(vRenderPosition.x - m_vecFrame[m_iCurFrameIdx].vSlice.x / 2),	/*대상 사각형 왼쪽 위 모서리의 x 좌표 (논리 단위)*/
			int(vRenderPosition.y - m_vecFrame[m_iCurFrameIdx].vSlice.y / 2),	/*대상 사각형의 왼쪽 위 모서리에 대한 논리 단위의 y 좌표*/
			int(m_vecFrame[m_iCurFrameIdx].vSlice.x),							/*대상 사각형의 너비 (논리 단위)*/
			int(m_vecFrame[m_iCurFrameIdx].vSlice.y),							/*대상 사각형의 높이 (논리 단위)*/
			m_pTexture->GetDC(),												/*소스 장치 컨텍스트에 대한 핸들*/
			int(m_vecFrame[m_iCurFrameIdx].vLT.x),								/*소스 사각형의 x 좌표 (논리 단위)*/
			int(m_vecFrame[m_iCurFrameIdx].vLT.y),								/*소스 직사각형의 논리 단위로 표시된 y 좌표*/
			int(m_vecFrame[m_iCurFrameIdx].vSlice.x),							/*소스 직사각형의 너비 (논리 단위)*/
			int(m_vecFrame[m_iCurFrameIdx].vSlice.y),							/*소스 직사각형의 높이(논리 단위)*/
			EXCEPTION_COLOR_RGB);												/*투명하게 처리 할 소스 비트 맵의 RGB 색상*/
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