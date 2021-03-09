#pragma once

class CObject;
class CTexture;
class CAnimation;

enum class E_AnimationPlayType {
	ONCE,		// 한번만 함
	LOOP,		// 끝난 후 처음부터 다시 시작하는 반복
	PINGPONG,	// 앞 뒤로 왔다갔다 반복
};

class CAnimator
{
private:
	
private:
	CObject* m_pOwner;
	map<wstring, CAnimation*> m_mapAnimation;
	CAnimation* m_pCurAnimation;

	E_AnimationPlayType m_ePlayType; // 재생 여부

public:
	void LateUpdate();
	void Render(HDC _hDC);

public:
	CObject* GetOwnerObject() { return m_pOwner; }
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vector2 _vLT, Vector2 _vSlice, int _iMaxFrame, float _fDuration);

	void PlayAnimation(const wstring& _strName, E_AnimationPlayType _eType);



public:
	CAnimator() = delete;
	CAnimator(CObject* _pOwner);
	virtual ~CAnimator() final;

	friend class CObject;
};

