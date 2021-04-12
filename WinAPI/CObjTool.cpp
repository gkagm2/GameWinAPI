#include "stdafx.h"
#include "CObjTool.h"
#include "CDragScreen.h"
#include "CTileToolPanelUI.h";
#include "CCore.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CImageUI.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CUIManager.h"
#include "CTileMap.h"
#include "CTile.h"
#include "CCamera.h"

CObjTool::CObjTool(E_GroupType _eGroupType) :
	CObject(_eGroupType),
	m_eToolMode(E_ToolMode::MAP),
	m_pDragScreen(nullptr),
	// Tile Tool
	bIsDrag(false),
	iSelectedTileIdx(-1),
	eTileType(E_TileType::None),
	startDragIdxX(-1),
	startDragIdxY(-1),
	pTileTexture(nullptr),
	m_pGroundTileToolUI(nullptr),
	m_pWallTileToolUI(nullptr)
	// Tile Tool end
{
}

CObjTool::~CObjTool()
{
}

void CObjTool::Init()
{
	// 드래그 표시
	m_pDragScreen = new CDragScreen(E_GroupType::PREV);
	CreateObject(m_pDragScreen);

	InitMapTool();
	InitItemTool();
	InitVehicleTool();
	InitCitizenTool();
	InitPlayerTool();

	CObject::Init();
}

void CObjTool::PrevUpdate()
{
}

void CObjTool::Update()
{
	if (InputKeyPress(E_Key::A)) {
		m_eToolMode = E_ToolMode::MAP;
		m_pGroundTileToolUI->SetActive(true);
		m_pWallTileToolUI->SetActive(true);
	}
	if (InputKeyPress(E_Key::S)) {
		m_eToolMode = E_ToolMode::ITEM;
		m_pGroundTileToolUI->SetActive(false);
		m_pWallTileToolUI->SetActive(false);
	}
	if (InputKeyPress(E_Key::D)) {
		m_eToolMode = E_ToolMode::VEHICLE;
		m_pGroundTileToolUI->SetActive(false);
		m_pWallTileToolUI->SetActive(false);
	}
	if (InputKeyPress(E_Key::W)) {
		m_eToolMode = E_ToolMode::PLAYER;
		m_pGroundTileToolUI->SetActive(false);
		m_pWallTileToolUI->SetActive(false);
	}

	switch (m_eToolMode) {
	case E_ToolMode::MAP:
		UpdateMapTool();
		break;
	case E_ToolMode::ITEM:
		UpdateItemTool();
		break;
	case E_ToolMode::VEHICLE:
		UpdateVehicleTool();
		break;
	case E_ToolMode::CITIZEN:
		UpdateCitizenTool();
		break;
	case E_ToolMode::PLAYER:
		UpdatePlayerTool();
		break;
	}
}

void CObjTool::LateUpdate()
{
}

void CObjTool::Render(HDC _hDC)
{

}

void CObjTool::OnCollisionEnter(CObject* _pOther)
{
}

void CObjTool::OnCollisionStay(CObject* _pOther)
{
}

void CObjTool::OnCollisionExit(CObject* _pOther)
{
}


// Tile
void CObjTool::InitMapTool()
{
	// Tile Tool (바닥) UI 생성
	m_pGroundTileToolUI = new CTileToolPanelUI(E_GroupType::UI);
	m_pGroundTileToolUI->Init();
	m_pGroundTileToolUI->SetScale(Vector3{ 300.f,500.f,0.f });
	m_pGroundTileToolUI->SetPosition(Vector3{});
	m_pGroundTileToolUI->SetObjectName(L"TileToolUI");

	Vector2 vResolution = CCore::GetInstance()->GetResolution();
	m_pGroundTileToolUI->SetPosition(vResolution.x - m_pGroundTileToolUI->ScaleX(), 0.f, 0.f);

	float fPadding[4]{ 20.f, 120.f, 20.f, 20.f }; // l, t, r, b
	m_pGroundTileToolUI->SetGridUI(5, 5, fPadding[0], fPadding[1], fPadding[2], fPadding[3], 5.f, 5.f);
	{
		CTexture* pTileGroundTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_UI_TileRoad);
		if (nullptr == pTileGroundTex)
			pTileGroundTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_UI_TileRoad, STR_FILE_PATH_UI_TileRoad);

		int iCol = pTileGroundTex->GetWidth() / TILE_SIZE;
		int iRow = pTileGroundTex->GetHeight() / TILE_SIZE;

		vector<CUI*>& vecChildUI = m_pGroundTileToolUI->GetChildsUI();
		for (UINT i = 0; i < vecChildUI.size(); ++i) {

			CImageUI* pImgUI = dynamic_cast<CImageUI*>(vecChildUI[i]);
			// 텍스쳐 설정
			pImgUI->SetTexture(pTileGroundTex);
			pImgUI->SetLT(Vector2(TILE_SIZE * (int)(i % iCol), TILE_SIZE * (int)(i / iRow)));
			pImgUI->SetTileIdx(i);
			pImgUI->SetTileType(E_TileType::Road);
			pImgUI->SetTexturePath(STR_FILE_PATH_UI_TileRoad);
		}
	}
	CreateObject(m_pGroundTileToolUI);

	// Wall Tile Tool UI 생성
	CTexture* pTileWallTex = CResourceManager::GetInstance()->FindTexture(STR_FIlE_NAME_UI_TileWall);
	m_pWallTileToolUI = m_pGroundTileToolUI->Clone();
	m_pWallTileToolUI->SetPosition(vResolution.x - m_pGroundTileToolUI->ScaleX() - m_pGroundTileToolUI->ScaleX(), 0.f, 0.f);
	m_pWallTileToolUI->SetObjectName(L"WallTileToolUI");
	{
		CTexture* pTileWallTex = CResourceManager::GetInstance()->FindTexture(STR_FIlE_NAME_UI_TileWall);
		if (nullptr == pTileWallTex)
			pTileWallTex = CResourceManager::GetInstance()->LoadTexture(STR_FIlE_NAME_UI_TileWall, STR_FILE_PATH_UI_TileWall);

		int iCol = pTileWallTex->GetWidth() / TILE_SIZE;
		int iRow = pTileWallTex->GetHeight() / TILE_SIZE;

		vector<CUI*>& vecChildUI = m_pWallTileToolUI->GetChildsUI();
		for (UINT i = 0; i < vecChildUI.size(); ++i) {

			CImageUI* pImgUI = dynamic_cast<CImageUI*>(vecChildUI[i]);
			// 텍스쳐 설정
			pImgUI->SetTexture(pTileWallTex);
			pImgUI->SetLT(Vector2(TILE_SIZE * (int)(i % iCol), TILE_SIZE * (int)(i / iRow)));
			pImgUI->SetTileIdx(i);
			pImgUI->SetTileType(E_TileType::Wall);
			pImgUI->SetTexturePath(STR_FILE_PATH_UI_TileWall);
		}
	}
	CreateObject(m_pWallTileToolUI);
}
void CObjTool::UpdateMapTool()
{
	MouseClick();
}
bool CObjTool::IsTileClicked(const Vector2& _vClickPos)
{
	CScene* pScene = CSceneManager::GetInstance()->GetCurScene();
	if (_vClickPos.x < pScene->GetTileMap()->GetPosition().x ||
		_vClickPos.x > pScene->GetTileMap()->GetPosition().x + pScene->GetTileMap()->GetTileMapWidth() ||
		_vClickPos.y < pScene->GetTileMap()->GetPosition().y ||
		_vClickPos.y > pScene->GetTileMap()->GetPosition().y + pScene->GetTileMap()->GetTileMapHeight()) {
		return false;
	}
	return true;
}
void CObjTool::MouseClick()
{

	if (InputKeyPress(E_Key::LBUTTON)) {
		// TODO : UI를 클릭 했을 경우가 타이밍이 안맞는다. UIManager보다 이게 먼저 실행되버림ㄴ
		// 포커스된 UI의 정보를 가져옴
		
		vector<CObject*>& pTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
		CUI* pFocusedUI = CUIManager::GetInstance()->GetCurFocusedUI();
		if (nullptr != pFocusedUI) {
			iSelectedTileIdx = -1;
			eTileType = E_TileType::None;
			pTileTexture = nullptr;

			CTileToolPanelUI* pPanelUI = dynamic_cast<CTileToolPanelUI*>(pFocusedUI);
			if (nullptr != pPanelUI) {
				iSelectedTileIdx = pPanelUI->GetSelectedTileIdx();
				eTileType = pPanelUI->GetSelectedTileType();
				pTileTexture = pPanelUI->GetTileTexture();
			}

			// 클릭된 UI가 없으면 종료
			if (-1 == iSelectedTileIdx || E_TileType::None == eTileType || nullptr == pTileTexture) {
				bIsDrag = false;
				iSelectedTileIdx = -1;
				startDragIdxX = -1;
				startDragIdxY = -1;
				return;
			}
		}

		// UI말고 다른곳을 클릭했다면
		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		if (pTileMap) {
			vector<CObject*>& pTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
			Vector2 vClickPos = MainCamera->GetScreenToWorldPosition(MousePosition);

			int iClickedCol = int(vClickPos.x / CTile::GetTileSize());
			int iClickedRow = int(vClickPos.y / CTile::GetTileSize());

			// 타일 영역 안에 클릭했는지 체크
			if (false == IsTileClicked(vClickPos))
				return;

			// 클릭한 타일의 정보를 가온다.
			int iClickedIdx = iClickedRow * pTileMap->GetCol() + iClickedCol;
			CTile* pTile = dynamic_cast<CTile*>(pTiles[iClickedIdx]);
			if (pTile) { // 타일을 설정한다.
				startDragIdxX = iClickedCol;
				startDragIdxY = iClickedRow;
				bIsDrag = true;
			}
		}
	}
	if (InputKeyRelease(E_Key::LBUTTON)) {
		if (bIsDrag == false)
			return;
		if (nullptr == pTileTexture || -1 == iSelectedTileIdx)
			return;

		CTileMap* pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();
		if (pTileMap) {
			Vector2 vMousePos = MousePosition;

			// 영역을 체크한다.
			Vector2 vClickPos = MainCamera->GetScreenToWorldPosition(vMousePos);

			int iClickedCol = int(vClickPos.x / CTile::GetTileSize());
			int iClickedRow = int(vClickPos.y / CTile::GetTileSize());

			if (iClickedCol < startDragIdxX) {
				int temp = iClickedCol;
				iClickedCol = startDragIdxX;
				startDragIdxX = temp;
			}
			if (iClickedRow < startDragIdxY) {
				int temp = iClickedRow;
				iClickedRow = startDragIdxY;
				startDragIdxY = temp;
			}

			// 타일 영역 안에 클릭했는지 체크
			if (false == IsTileClicked(vClickPos))
				return;

			vector<CObject*>& pTiles = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
			// 드래그 한 영역의 타일을 설정한다.
			for (int iRow = startDragIdxY; iRow <= iClickedRow; ++iRow) {
				for (int iCol = startDragIdxX; iCol <= iClickedCol; ++iCol) {
					int iClickedIdx = iRow * pTileMap->GetCol() + iCol;
					CTile* pTile = dynamic_cast<CTile*>(pTiles[iClickedIdx]);
					if (pTile)
						pTile->SetTile(iSelectedTileIdx, eTileType, pTileTexture);
				}
			}
		}
		bIsDrag = false;
	}
}


// Citizen
void CObjTool::InitCitizenTool()
{
	CTexture* pCitizenTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_gta_citizen);
	if (nullptr == pCitizenTex)
		pCitizenTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_gta_citizen, STR_FILE_PATH_gta_citizen);
	// TODO : Citizen은 여기서 해야되나..? 수정할게 뭐가있지?
}
void CObjTool::UpdateCitizenTool()
{
}

// Item
void CObjTool::InitItemTool()
{
	CTexture* pItemTex = CResourceManager::GetInstance()->FindTexture(STR_FILE_NAME_gta_item);
	if (nullptr == pItemTex)
		pItemTex = CResourceManager::GetInstance()->LoadTexture(STR_FILE_NAME_gta_item, STR_FILE_PATH_gta_item);

	// Item Panel UI  생성.
	// TODO : Item Tool Panel 생성
	/*CItemToolPanelUI* pItemToolPanelUI = new CItemToolPanelUI(E_GroupType::UI);

	CreateObject(pItemPanelUI);
	CObject* pItemObjs;*/



}
void CObjTool::UpdateItemTool()
{

}


// Vehicle
void CObjTool::InitVehicleTool()
{
}
void CObjTool::UpdateVehicleTool()
{
}

// Player
void CObjTool::InitPlayerTool()
{
}
void CObjTool::UpdatePlayerTool()
{
}
