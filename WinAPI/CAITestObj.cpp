#include "stdafx.h"
#include "CAITestObj.h"
#include "CGTA_AI.h"
#include "CObject.h"

#include "CScene.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CTileMap.h"
#include "CTile.h"
#include "CPathFinding.h"
#include "CTexture.h"
#include "CResourceManager.h"

#include "CCore.h"
CAITestObj::CAITestObj(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_vTargetPos{},
	m_pAI(nullptr),
	m_fPathFindCoolTime(0.f),
	m_fPathFindMaxCoolTime(3.f),
	m_bIsPathFind(false),
	m_pPathFinding(nullptr)
{
	m_pAI = new CGTA_AI();
	CreatePathFinding();
}

CAITestObj::~CAITestObj()
{
	if (m_pAI)
		delete m_pAI;
	if (m_pPathFinding)
		delete m_pPathFinding;
}

void CAITestObj::Init()
{
	CTexture* pTexture = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_Monster);
	if (nullptr == pTexture)
		pTexture = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_Monster, STR_FILE_PATH_Monster);
	SetTexture(pTexture);
}

void CAITestObj::Update()
{
	CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	TTilePos startPos = pTileMap->VectorToTilePos(GetPosition());
	Vector2 vClickedPos = MainCamera->GetScreenToWorldPosition(MousePosition);
	TTilePos destPos = pTileMap->VectorToTilePos(vClickedPos);

	if (InputKeyPress(E_Key::Ctrl)) {
		m_bIsPathFind = m_pPathFinding->PathFind(startPos, destPos);
	}
	if (m_bIsPathFind) {
		HDC _hDC = CCore::GetInstance()->GetDC();
		list<TTilePos>& path = m_pPathFinding->GetPath();

		list<TTilePos>::iterator iter = path.begin();

		TTilePos prev= *iter;
		++iter;
		for (; iter != path.end(); ++iter) {
			Vector2 vCurPos = MainCamera->GetRenderPosition(pTileMap->TilePosToVector(prev));
			vCurPos += (TILE_SIZE * 0.5f);
			Rectangle(_hDC, vCurPos.x - 5, vCurPos.y - 5, vCurPos.x + 5, vCurPos.y + 5);
			MoveToEx(_hDC, vCurPos.x, vCurPos.y, nullptr);
			Vector2 destPos = MainCamera->GetRenderPosition(pTileMap->TilePosToVector(*iter));

			destPos += (TILE_SIZE * 0.5f);
			LineTo(_hDC, destPos.x, destPos.y);
			Ellipse(_hDC, destPos.x - 5, destPos.y - 5, destPos.x + 5, destPos.y + 5);
			prev = *iter;
		}
	}
}

void CAITestObj::Render(HDC _hDC)
{
	CObject::Render(_hDC);
}

void CAITestObj::OnCollisionEnter(CObject* _pOther)
{
}

void CAITestObj::OnCollisionStay(CObject* _pOther)
{
}

void CAITestObj::OnCollisionExit(CObject* _pOther)
{
}

void CAITestObj::CreatePathFinding()
{
	if (nullptr == m_pPathFinding) {
		m_pPathFinding = new CPathFinding();
		m_pPathFinding->Init();
	}
}
