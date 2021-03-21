#include "stdafx.h"
#include "CSceneManager.h"
#include "CScene_Start.h"
#include "CScene_Stage01.h"
#include "CScene_Tool.h"

CSceneManager::CSceneManager() :
	m_arrScene{},
	m_pCurScene(nullptr)
{
}
CSceneManager::~CSceneManager() {
	for (int i = 0; i < (int)E_SceneType::END; ++i) {
		if (nullptr != m_arrScene[i])
			delete m_arrScene[i];
	}
}

void CSceneManager::Init()
{
	m_arrScene[(UINT)E_SceneType::START] = new CScene_Start;
	m_arrScene[(UINT)E_SceneType::STAGE01] = new CScene_Stage01;
	m_arrScene[(UINT)E_SceneType::TOOL] = new CScene_Tool;
	ChangeScene(E_SceneType::TOOL);
}

void CSceneManager::Progress()
{
	m_pCurScene->PrevUpdate();
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();
}

void CSceneManager::Render(HDC _hDC)
{
	m_pCurScene->Render(_hDC);
}

void CSceneManager::ChangeScene(E_SceneType _eSceneType)
{
	if (nullptr != m_pCurScene)
		m_pCurScene->End();

	m_pCurScene = m_arrScene[(UINT)_eSceneType];
	m_pCurScene->Start();
}
