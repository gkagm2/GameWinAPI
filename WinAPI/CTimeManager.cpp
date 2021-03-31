 #include "stdafx.h"
#include "CTimeManager.h"
#include "CCore.h"

CTimeManager::CTimeManager() :
	m_llFrequency{},
	m_llCurFrameCount{},
	m_llPrevFrameCount{},
	m_llPrevCount{},
	m_fDeltaTime(.0f),
	m_iFPS(0),
	m_iFrameCount(0),
	m_timeScale(1.0f),
	m_bIsRender(false)
{
	m_bIsRender = true;
}
CTimeManager::~CTimeManager() {
}

void CTimeManager::Init()
{
	QueryPerformanceFrequency(&m_llFrequency); // 초당 빈도수
	QueryPerformanceCounter(&m_llPrevCount); // 프레임의 시간을 얻어옴  (현재까지 진행한 카운터)
	m_llCurFrameCount = m_llPrevCount;
	m_llPrevFrameCount = m_llPrevCount;
}

void CTimeManager::Update()
{
	++m_iFrameCount;
	m_llPrevFrameCount = m_llCurFrameCount;
	QueryPerformanceCounter(&m_llCurFrameCount); // 현재 프레임의 시간을 얻어옴.

	if (m_llCurFrameCount.QuadPart - m_llPrevCount.QuadPart > m_llFrequency.QuadPart) {
		QueryPerformanceFrequency(&m_llFrequency); // 초당 빈도수 갱신
		m_llPrevCount = m_llCurFrameCount;
		m_iFPS = m_iFrameCount;

		// Print FPS
		wchar_t szFPSBuffer[255] = L"";
		swprintf_s(szFPSBuffer, 255, L"FPS:%d", m_iFPS);
		SetWindowText(CCore::GetInstance()->GetWndHandle(), szFPSBuffer);

		m_iFrameCount = 0;
	}
	m_fDeltaTime = (float)(m_llCurFrameCount.QuadPart - m_llPrevFrameCount.QuadPart) / (float)(m_llFrequency.QuadPart);  // (현재 틱 - 이전 틱) / 빈도 수(초당 틱)

#ifdef _DEBUG
	if (m_fDeltaTime > 0.166666f)
		m_fDeltaTime = 0.166666f;
#endif
}

void CTimeManager::Render(HDC _hdc)
{
	if (!m_bIsRender)
		return;

	HDC hdc = _hdc;
	// TODO : Delete this
	//wchar_t strFPS[255]{'\0'};
	//wchar_t strFrequency[255]{'\0'};
	//wchar_t str[255]{'\0'};
	//wchar_t strDeltaTime[255]{ '\0' };
	//swprintf(strFPS, 255, L"FPS : %d", m_iFPS);
	//swprintf(strFrequency, 255, L"Frequency : %lli", m_llFrequency.QuadPart);
	//swprintf(str, 255, L"Frame Tick  : %lli", m_llCurFrameCount.QuadPart);
	//swprintf(strDeltaTime, 255, L"delta Time  : %f", m_fDeltaTime);

	//SetTextAlign(hdc, TA_TOP | TA_LEFT); //왼쪽 상단
	//TextOut(hdc, 10, 30, strFPS, (int)wcslen(strFPS));

	//HWND hWnd = CCore::GetInstance()->GetWndHandle();
	//RECT clientRect{};
	//GetClientRect(hWnd, &clientRect);
	//SetTextAlign(hdc, TA_TOP| TA_RIGHT); // 오른쪽 상단
	//TextOut(hdc, clientRect.right - 10, 30, strFrequency, (int)wcslen(strFrequency));
	//SetTextAlign(hdc, TA_TOP | TA_LEFT);
	//TextOut(hdc, 10, 70, str, (int)wcslen(str));
	//TextOut(hdc, 10, 90, strDeltaTime, (int)wcslen(strDeltaTime));

	//RECT textBoxArea = { 200, 200, 500, 500};
	//Rectangle(hdc, textBoxArea.left, textBoxArea.top, textBoxArea.right, textBoxArea.bottom);
	//wchar_t testStr[] = L"가나다라마바사아자차카타파하 abcaklsdj fkal;sdj"
	//	L";fklasdjfkladsjklf jdas;klf jsdklfsj klj fklasd jkl;f jakl;we"
	//	L"jk;lfjkl;xvmadfpomv poasermopv emsrpov mrepvmposemrpovbmseoprmbvopsemrpovbmsopermvposemropvmspoermbvp"
	//	L"'sermbp'osermbop'smep'rotbm'opstembpostemrbpo'setmop'bmsetopr bmsptorebmsp'ortmbsrop'tmbp'orst'op DONE";
	//DrawText(hdc, testStr, -1, &textBoxArea, DT_LEFT | DT_WORDBREAK);
}