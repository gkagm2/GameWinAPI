#include "stdafx.h"
#include "CBackgroundController.h"
#include "CBackground.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CCore.h"
#include "CTimeManager.h"
CBackgroundController::CBackgroundController(E_GroupType e_GroupType) :
	CObject(e_GroupType),
	m_pBackgroundTextrue(nullptr)
{
	Init();
}

CBackgroundController::~CBackgroundController()
{
	for (int i = 0; i < m_vecBackground.size(); ++i)
		DestroyObject(m_vecBackground[i]);
	for (int i = 0; i < m_vecBackgroundFront1.size(); ++i)
		DestroyObject(m_vecBackgroundFront1[i]);
	for (int i = 0; i < m_vecBackgroundFront2.size(); ++i)
		DestroyObject(m_vecBackgroundFront2[i]);
	for (int i = 0; i < m_vecBackgroundFront3.size(); ++i)
		DestroyObject(m_vecBackgroundFront3[i]);

	delete m_pBackgroundTextrue;
	delete m_pBackgroundTextrueF1;
	delete m_pBackgroundTextrueF2;
	delete m_pBackgroundTextrueF3;
}

void CBackgroundController::Update()
{
	for (int i = 0; i < m_vecBackground.size(); ++i) {
		Vector3 position = m_vecBackground[i]->GetPosition();
		position.y += 35.0f * DeltaTime;

		// 화면 아래로 내려갔으면 다시 위로 올리기
		Vector3 vResolution = CCore::GetInstance()->GetResolution();
		Vector2 middlePos = vResolution * 0.5f;
		if (position.y > middlePos.y + vResolution.y) {
			position.y = middlePos.y - vResolution.y;
		}
		m_vecBackground[i]->SetPosition(position);
	}

	for (int i = 0; i < m_vecBackgroundFront1.size(); ++i) {
		Vector3 position = m_vecBackgroundFront1[i]->GetPosition();
		position.y += 50.0f * DeltaTime;

		// 화면 아래로 내려갔으면 다시 위로 올리기
		Vector3 vResolution = CCore::GetInstance()->GetResolution();
		Vector2 middlePos = vResolution * 0.5f;
		if (position.y > middlePos.y + vResolution.y) {
			float randomX = float(rand() % (int)vResolution.x + 30);
			position.x = randomX; 
			position.y = position.y - vResolution.y * 2;
		}
		m_vecBackgroundFront1[i]->SetPosition(position);
	}

	for (int i = 0; i < m_vecBackgroundFront2.size(); ++i) {
		Vector3 position = m_vecBackgroundFront2[i]->GetPosition();
		position.y += 80.0f * DeltaTime;

		// 화면 아래로 내려갔으면 다시 위로 올리기
		Vector3 vResolution = CCore::GetInstance()->GetResolution();
		Vector2 middlePos = vResolution * 0.5f;
		if (position.y > middlePos.y + vResolution.y) {
			float randomX = float(rand() % (int)vResolution.x + 30);
			position.x = randomX;
			position.y = position.y - vResolution.y * 2;
		}
		m_vecBackgroundFront2[i]->SetPosition(position);

	}

	for (int i = 0; i < m_vecBackgroundFront3.size(); ++i) {
		Vector3 position = m_vecBackgroundFront3[i]->GetPosition();
		position.y += 120.0f * DeltaTime;

		// 화면 아래로 내려갔으면 다시 위로 올리기
		Vector3 vResolution = CCore::GetInstance()->GetResolution();
		Vector2 middlePos = vResolution * 0.5f;
		if (position.y > middlePos.y + vResolution.y) {
			float randomX = float(rand() % (int)vResolution.x + 30);
			position.x = randomX;
			position.y = position.y - vResolution.y * 2;
		}
		m_vecBackgroundFront3[i]->SetPosition(position);
	}

}

void CBackgroundController::LateUpdate()
{
}

void CBackgroundController::Init()
{
	// Get screen size
	Vector3 vResolution = CCore::GetInstance()->GetResolution();
	SetScale(vResolution);

	// Textrue load
	CTexture* pBGTexture = CResourceManager::GetInstance()->GetTexture(STR_FILE_NAME_Background, STR_FILE_PATH_Background);
	m_pBackgroundTextrue = pBGTexture;

	// Create Background Object
	for (int i = 0; i < 2; ++i) {
		CBackground* pBackground = new CBackground(E_GroupType::BACKGROUND);
		pBackground->SetTexture(pBGTexture);
		pBackground->SetScale(vResolution);
		m_vecBackground.push_back(pBackground);
		CreateObject(pBackground);
	}

	// Position Setting
	Vector2 middlePos = vResolution * 0.5f;
	m_vecBackground[0]->SetPosition(middlePos.x, middlePos.y - vResolution.y);
	m_vecBackground[1]->SetPosition(middlePos);

	CTexture* pBGTexture1 = CResourceManager::GetInstance()->GetTexture(STR_FILE_NAME_Background1, STR_FILE_PATH_Background1);
	m_pBackgroundTextrueF1 = pBGTexture1;
	CTexture* pBGTexture2 = CResourceManager::GetInstance()->GetTexture(STR_FILE_NAME_Background2, STR_FILE_PATH_Background2);
	m_pBackgroundTextrueF2 = pBGTexture2;
	CTexture* pBGTexture3 = CResourceManager::GetInstance()->GetTexture(STR_FILE_NAME_Background3, STR_FILE_PATH_Background3);
	m_pBackgroundTextrueF3 = pBGTexture3;

	for (int i = 0; i < 3; ++i) {
		CBackground* pBackground = new CBackground(E_GroupType::BACKGROUND);
		pBackground->SetTexture(pBGTexture1);
		pBackground->SetScale(Vector3(50.0f, 50.0f,0.0f));
		float randomX = float(rand() % (int)(vResolution.x + 30));
		float randomY = float(rand() % (int)(vResolution.y + 3));
		pBackground->SetPosition(randomX, randomY - vResolution.y, 0.0f);
		m_vecBackgroundFront1.push_back(pBackground);
		CreateObject(pBackground);
	}
	for (int i = 0; i < 3; ++i) {
		CBackground* pBackground = new CBackground(E_GroupType::BACKGROUND);
		pBackground->SetTexture(pBGTexture2);
		pBackground->SetScale(Vector3(90.0f, 90.0f, 0.0f));
		float randomX = float(rand() % (int)(vResolution.x + 30));
		float randomY = float(rand() % (int)(vResolution.y + 3));
		pBackground->SetPosition(randomX, randomY - vResolution.y, 0.0f);
		m_vecBackgroundFront2.push_back(pBackground);
		CreateObject(pBackground);
	}
	for (int i = 0; i < 3; ++i) {
		CBackground* pBackground = new CBackground(E_GroupType::BACKGROUND);
		pBackground->SetTexture(pBGTexture3);
		pBackground->SetScale(Vector3(130.0f, 130.0f, 0.0f));
		float randomX = float(rand() % (int)(vResolution.x + 30));
		float randomY = float(rand() % (int)(vResolution.y + 3));
		pBackground->SetPosition(randomX , randomY - vResolution.y, 0.0f);
		m_vecBackgroundFront3.push_back(pBackground);
		CreateObject(pBackground);
	}
}