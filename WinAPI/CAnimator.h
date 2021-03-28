#pragma once

class CObject;
class CTexture;
class CAnimation;

enum class E_AnimationPlayType {
	ONCE,		// �ѹ��� ��
	LOOP,		// ���� �� ó������ �ٽ� �����ϴ� �ݺ�
	PINGPONG,	// �� �ڷ� �Դٰ��� �ݺ�
};

class CAnimator
{
private:
	
private:
	CObject* m_pOwner;
	map<wstring, CAnimation*> m_mapAnimation;
	CAnimation* m_pCurAnimation;

	E_AnimationPlayType m_ePlayType; // ��� ����

public:
	void LateUpdate();
	void Render(HDC _hDC);

public:
	CObject* GetOwnerObject() { return m_pOwner; }
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vector2 _vLT, Vector2 _vSlice, int _iMaxFrame, float _fDuration);
	void CreateAnimation(const wstring _strName, CTexture* _pTex, Vector2 _vLT, Vector2 _vSlice, Vector2 _vInsidePadding, int _iMaxFrame, float _fDuration);

	CAnimation* GetAnimation(const wstring& _strName);

	void PlayAnimation(const wstring& _strName, E_AnimationPlayType _eType);

public:
	void Save(const wstring& _strRelativePath);
	void Load(const wstring& _strRelativePath);

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