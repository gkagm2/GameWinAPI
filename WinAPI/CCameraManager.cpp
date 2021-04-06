#include "stdafx.h"
#include "CCameraManager.h"
#include "CCamera.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CObject.h"

CCameraManager::CCameraManager() {

}

CCameraManager::~CCameraManager() {

}

void CCameraManager::Init() {
	
}

void CCameraManager::Update()
{
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	if (nullptr == pScene)
		return;

	vector<CObject*>& vecCam = pScene->GetObjects(E_GroupType::MAIN_CAMERA);
	for (UINT i = 0; i < vecCam.size(); ++i)
		vecCam[i]->Update();
}

void CCameraManager::Render(HDC _hDC)
{
}