#pragma once
#include "CObject.h"

enum class E_ToolMode {
	MAP,
	ITEM,
	VEHICLE,
	CITIZEN,
	PLAYER,
};


class CDragScreen;
class CTileToolPanelUI;
class CColliderRect;
class CObjTool : public CObject
{

private:
	E_ToolMode m_eToolMode;
	CDragScreen* m_pDragScreen; // 드래그 표시



	// Tile Tool
	bool bIsDrag; // 클릭한 정보
	int iSelectedTileIdx;
	E_TileType eTileType;
	int startDragIdxX;
	int startDragIdxY;
	CTexture* pTileTexture;
	CTileToolPanelUI* m_pGroundTileToolUI;
	CTileToolPanelUI* m_pWallTileToolUI;
	
	// Citizen Tool

	// Item Tool
	CObject* m_pSelectedObj;
	bool m_bIsClick;
	
public:
	virtual void Init() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) override;
	virtual void OnCollisionStay(CObject* _pOther) override;
	virtual void OnCollisionExit(CObject* _pOther) override;

public:
	void SetToolMode(E_ToolMode _eToolMode) { m_eToolMode = _eToolMode; }
	E_ToolMode GetToolMode() { return m_eToolMode; }

	void OpenMapTool();
	void OpenItemTool();
	void OpenVehicleTool();
	void OpenCitizenTool();

	// Tile
	void InitMapTool();
	void UpdateMapTool();
	bool IsTileClicked(const Vector2& _vClickPos);
	void MouseClick();
	bool CheckUICollider();

	// Item
	void InitItemTool();
	void UpdateItemTool();
	void CreateWeaponItem(E_WeaponType _eWeaponType);
	void UpdateItemInfo();

	// Vehicle
	void InitVehicleTool();
	void UpdateVehicleTool();

	// Citizen
	void InitCitizenTool();
	void UpdateCitizenTool();

	// Player
	void InitPlayerTool();
	void UpdatePlayerTool();

public:
	CLONE_DISABLE(CObjTool);

public:
	CObjTool() = delete;
	CObjTool(E_GroupType _eGroupType);
	virtual ~CObjTool() override;
};