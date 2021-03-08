#pragma once
class CTimeManager {
	SINGLETON(CTimeManager)
private:
	LARGE_INTEGER m_llFrequency;
	LARGE_INTEGER m_llCurFrameCount;
	LARGE_INTEGER m_llPrevFrameCount;
	LARGE_INTEGER m_llPrevCount;

	float m_fDeltaTime; // ������ ���� �ð�
	int m_iFPS; // �ʴ� ������ ��
	int m_iFrameCount;// ���� ȣ�� Ƚ��

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