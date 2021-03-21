#include "stdafx.h"

#include "CKeyManager.h"

#include "CUIManager.h"
#include "CSceneManager.h"
#include "CScene.h"

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
		if (InputKeyPress(E_Key::LBUTTON)) {
			// 영역 내부에 있는지 체크한다.

		}
		if (InputKeyRelease(E_Key::LBUTTON)) {

		}
		if (InputKeyHold(E_Key::LBUTTON)) {

		}
	}
}
