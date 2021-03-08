#include "stdafx.h"
#include "CKeyManager.h"

// TIP : VK_RETURN 키가 엔터키임
int g_arrVK[(int)E_Key::END] = {
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	VK_SPACE,
	'W',
	'S',
	'A',
	'D',
};

CKeyManager::CKeyManager() {

}
CKeyManager::~CKeyManager() {

}

void CKeyManager::Init()
{
	for (int i = 0; i < (int)E_Key::END; ++i)
		m_vecKey.push_back(tKey{ E_KeyState::NONE, false }); // state = none , press = false
}

void CKeyManager::Update()
{
	HWND hWnd = GetFocus();
	if (nullptr == hWnd) {
		for (UINT i = 0; i < m_vecKey.size(); ++i) {
			if (E_KeyState::PRESS == m_vecKey[i].eState || E_KeyState::HOLD == m_vecKey[i].eState)
				m_vecKey[i].eState = E_KeyState::RELEASE;
			else if (E_KeyState::RELEASE == m_vecKey[i].eState)
				m_vecKey[i].eState = E_KeyState::NONE;
			m_vecKey[i].bPress = false;
		}
	}
	else {
		for (UINT i = 0; i < m_vecKey.size(); ++i) {
			tKey& keyInfo = m_vecKey[i];
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000) { // 키가 눌림
				if (!keyInfo.bPress) { // 누르지 않은 상태면
					keyInfo.bPress = true;
					keyInfo.eState = E_KeyState::PRESS;
				}
				else // 이전에도 눌려있으면
					keyInfo.eState = E_KeyState::HOLD;
			}
			else { // 키를 누르지 않았으면
				if (keyInfo.bPress) { // 눌려있었으면 (떼는 순간)
					keyInfo.bPress = false;
					keyInfo.eState = E_KeyState::RELEASE;
				}
				else // 이전에도 안눌려있다면
					keyInfo.eState = E_KeyState::NONE;
			}
			m_vecKey[i] = keyInfo;
		}
	}
}