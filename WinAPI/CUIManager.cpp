#include "stdafx.h"

#include "CKeyManager.h"

#include "CUIManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CUI.h"

CUIManager::CUIManager() :
	m_pCurFocusedUI(nullptr)
{

}

CUIManager::~CUIManager() {

}

void CUIManager::Update()
{
	CUI* pPrevFocusedUI = m_pCurFocusedUI;
	m_pCurFocusedUI = GetFocusedUI();

	// ��Ŀ�̵� UI�� ����Ǿ����� �׷� ���Ϳ� ���� �ڷ� �ű�
	if (pPrevFocusedUI != m_pCurFocusedUI) {
		CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
		if (nullptr == pScene)
			return;
		vector<CObject*>& vecUI = pScene->GetObjects(E_GroupType::UI);
		for (auto iter = vecUI.begin(); iter != vecUI.end(); ++iter) {
			if ((*iter) == m_pCurFocusedUI) {
				vecUI.erase(iter);
				break;
			}
		}
		vecUI.push_back(m_pCurFocusedUI);
	}

	if (nullptr != m_pCurFocusedUI)
		UIEvent(m_pCurFocusedUI);
}

void CUIManager::UIEvent(CUI* _pParentUI)
{
	CUI* pUI = GetTargetedUi(_pParentUI);
	if (nullptr == pUI)
		return;

	if (pUI->IsPointerOn(MousePosition)) { // ���� ���� ������ �� 
		if (false == pUI->m_bIsOn)
			pUI->OnPointerEnter();

		if (InputKeyPress(E_Key::LBUTTON)) {
			pUI->OnPointerDown();
		}
		else if (InputKeyRelease(E_Key::LBUTTON)) {
			pUI->OnPointerUp();
			if (pUI->m_bIsOn)
				pUI->OnPointerClick();
		}

		if (false == pUI->m_bIsOn)
			pUI->m_bIsOn = true;
	}
	else { // ���� ���� �� 
		if (pUI->m_bIsOn) {
			pUI->OnPointerExit();
			pUI->m_bIsOn = false;
		}
	}
}

CUI* CUIManager::GetFocusedUI()
{
	// Mouse Left Button ���¸� ������
	E_KeyState eCurKeyState = CKeyManager::GetInstance()->GetKeyState(E_Key::LBUTTON);

	// ������� UI ������Ʈ���� ������
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetObjects(E_GroupType::UI);

	CUI* pCurFocusedUI = m_pCurFocusedUI; // ���� ��Ŀ�̵� UI�� ������
	if (nullptr == pCurFocusedUI)
		return nullptr;

	for (UINT i = 0; i < vecUI.size(); ++i) {
		// ��Ŀ�� �Ǿ������鼭 ��������
		if (pCurFocusedUI->IsPointerOn(MousePosition) && eCurKeyState == E_KeyState::PRESS) {
			pCurFocusedUI = dynamic_cast<CUI*>(vecUI[i]);
		}
	}
	return pCurFocusedUI;
}

CUI* CUIManager::GetTargetedUi(CUI* _pParentUI)
{

	return nullptr;
}
