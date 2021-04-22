#pragma once
#include "CComponent.h"
class CObject;
class CTexture;
class CAnimation;

enum class E_AnimationPlayType {
	ONCE,		// 한번만 함
	LOOP,		// 끝난 후 처음부터 다시 시작하는 반복
	PINGPONG,	// 앞 뒤로 왔다갔다 반복
};

class CAnimator : public CComponent
{
private:
	
private:
	CObject* m_pOwner;
	map<wstring, CAnimation*> m_mapAnimation;
	CAnimation* m_pCurAnimation;

	E_AnimationPlayType m_ePlayType; // 재생 여부
	
	float m_fAnimTexWidth; // Get으로만 접근하기
	float m_fAnimTexHeight;

	bool m_bIsActive;

public:
	void LateUpdate();
	void Render(HDC _hDC);

public:
	CObject* GetOwnerObject() { return m_pOwner; }
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vector2 _vLT, Vector2 _vSlice, int _iMaxFrame, float _fDuration);

	CAnimation* GetAnimation(const wstring& _strName);
	CAnimation* GetCurAnimation() { return m_pCurAnimation; }

	void PlayAnimation(const wstring& _strName, E_AnimationPlayType _eType);

public:
	void Save(const wstring& _strRelativePath);
	void Load(const wstring& _strRelativePath);

public:
	float GetAnimTexWidth(int _iFrameIdx = 0);
	float GetAnimTexHeight(int _iFrameIdx = 0);

public:
	CLONE(CAnimator);

protected:
	CAnimator(const CAnimator& _origin);

public:
	CAnimator() = delete;
	CAnimator(CObject* _pOwner);
	virtual ~CAnimator() final;

	friend class CObject;
};