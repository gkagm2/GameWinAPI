#pragma once
#include "CComponent.h"
class CObject;
class CTexture;
class CAnimation;

enum class E_AnimationPlayType {
	ONCE,		// �ѹ��� ��
	LOOP,		// ���� �� ó������ �ٽ� �����ϴ� �ݺ�
	PINGPONG,	// �� �ڷ� �Դٰ��� �ݺ�
};

class CAnimator : public CComponent
{
private:
	
private:
	CObject* m_pOwner;
	map<wstring, CAnimation*> m_mapAnimation;
	CAnimation* m_pCurAnimation;

	E_AnimationPlayType m_ePlayType; // ��� ����
	
	float m_fAnimTexWidth; // Get���θ� �����ϱ�
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