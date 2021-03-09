#pragma once

struct tAnimFrame { //Anim : Animation
	Vector3 vLT;
	Vector3 vSlice;
	float fDuration;
};

class CAnimator;
class CTexture;
class CAnimation
{
private:
	CAnimator* m_pAnimator;
	wstring m_strName;
	CTexture* m_pTexture;
	vector<tAnimFrame> m_vecFrame;
	int m_iCurFrameIdx;
	float m_fAccumulationTime; // 누적 시간 (Acc : Accumulation)

	bool m_bFinish;

public:
	void LateUpdate();
	void Render(HDC _hDC);

public:
	void Create(const wstring& _strName, CTexture* _pTexture, Vector2 _vLT, Vector2 _vSlice, int _iMaxFrame, float _fDuration);
	bool IsFinish() { return m_bFinish; }
	void Reset() {
		m_fAccumulationTime = 0.0f;
		m_bFinish = false;
		m_iCurFrameIdx = 0;
	}

public:
	CAnimation();
	virtual ~CAnimation() final;

public:
	friend class CAnimator;
};