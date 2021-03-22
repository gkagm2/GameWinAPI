#include "stdafx.h"

#include "CKeyManager.h"

#include "CUIManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CUI.h"

CUIManager::CUIManager() {

}

CUIManager::~CUIManager() {

}

void CUIManager::Update()
{
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	if (nullptr == pScene)
		return;

	vector<CObject*>& vecUI = pScene->GetObjects(E_GroupType::UI);

	// Click event
	for (UINT i = 0; i < vecUI.size(); ++i) {
		CUI* pUI = dynamic_cast<CUI*>(vecUI[i]);
		if (nullptr == pUI)
			continue;

		if (pUI->IsPointerOn(MousePosition)) { // 영역 내로 들어왔을 때 
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
		else { // 영역 밖일 때 
			if (pUI->m_bIsOn) {
				pUI->OnPointerExit();
				pUI->m_bIsOn = false;
			}
		}
	}
}
