#pragma once
class CTimeManager {
	SINGLETON(CTimeManager)
private:
	LARGE_INTEGER m_llFrequency;
	LARGE_INTEGER m_llCurFrameCount;
	LARGE_INTEGER m_llPrevFrameCount;
	LARGE_INTEGER m_llPrevCount;

	float m_fDeltaTime; // 프레임 간격 시간
	int m_iFPS; // 초당 프레임 수
	int m_iFrameCount;// 실제 호출 횟수

	float m_timeScale;

	bool m_bIsRender;

public:
	float GetDeltaTime() { return m_fDeltaTime; }
	int GetFPS() { return m_iFPS; }
	void SetRender(bool _isRender) { m_bIsRender = _isRender; }
	float IsRender(bool _isRender) { m_bIsRender = _isRender; }
	
	void Init();
	void Update();
	void Render(HDC _hdc);
};