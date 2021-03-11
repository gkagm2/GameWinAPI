#include "stdafx.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CObject.h"
#include "CPathManager.h"

CAnimator::CAnimator(CObject* _pOwner) :
	m_pOwner(_pOwner),
	m_pCurAnimation(nullptr),
	m_ePlayType(E_AnimationPlayType::ONCE)
{
	_pOwner->SetAnimator(this);
}

CAnimator::~CAnimator()
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnimation.begin();
	for (; iter != m_mapAnimation.end(); ++iter)
		delete iter->second;
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vector2 _vLT, Vector2 _vSlice, int _iMaxFrame, float _fDuration)
{
	CAnimation* pAnimation = new CAnimation;
	pAnimation->Create(_strName, _pTex, _vLT, _vSlice, _iMaxFrame, _fDuration);
	m_mapAnimation.insert(make_pair(_strName, pAnimation));
	pAnimation->m_pAnimator = this;
}

CAnimation* CAnimator::GetAnimator(const wstring& _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnimation.find(_strName);
	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}

void CAnimator::PlayAnimation(const wstring& _strName, E_AnimationPlayType _eType)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnimation.find(_strName);
	assert(iter != m_mapAnimation.end());

	m_pCurAnimation = iter->second; // 애니메이션 지정
	m_ePlayType = _eType;
}

void CAnimator::LateUpdate()
{
	if (nullptr == m_pCurAnimation)
		return;

	switch (m_ePlayType) {
		case E_AnimationPlayType::ONCE:
			if (m_pCurAnimation->IsFinish())
				return;
			break;
		case E_AnimationPlayType::LOOP:
			if (m_pCurAnimation->IsFinish())
				m_pCurAnimation->Reset();
			break;
		case E_AnimationPlayType::PINGPONG:
			// TODO : 구현하기
			break;
	}

	m_pCurAnimation->LateUpdate();
}

void CAnimator::Render(HDC _hDC)
{
	if (nullptr == m_pCurAnimation)
		return;

	m_pCurAnimation->Render(_hDC);
}

void CAnimator::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// Animation count
	int iAnimCnt = m_mapAnimation.size();
	// fwrite(iAnimCn)
	// TODO : 구현하기
}

void CAnimator::Load(const wstring& _strRelativePath)
{

}