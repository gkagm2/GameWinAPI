#pragma once
class CObject; // 전방 선언을 해야 찾아가는 번거로움을 안함.
class CTileMap;
class CCamera;
class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)E_GroupType::END];

protected:
	CTileMap* m_pTileMap; // 사용하는 Scene에 Tile을 사용할 경우 가리킴.
	CCamera* m_pMainCamera; // 사용하는 Scene에 Main 카메라를 가리킴.

public:
	virtual void PrevUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(HDC _hDC);

public: // Tile map
	virtual void PrevUpdateTile();
	virtual void UpdateTile();
	virtual void LateUpdateTile();
	virtual void RenderTile(HDC _hDC);

	CTileMap* GetTileMap() { return m_pTileMap; }
	void LoadTile(wstring _strRelativePath);
	void LoadTileDialogBox(wstring _strPath);

	void LoadItem(wstring _strRelativePath);
	void LoadItemDialogBox(wstring _strPath);

	void LoadPlayer(wstring _strRelativePath);
	void LoadPlayerDialogBox(wstring _strPath);

	void LoadAll();

public:
	virtual void Start() = 0;
	virtual void End() = 0;

	vector<CObject*>& GetObjects(E_GroupType _eGroup) { return m_vecObj[(UINT)_eGroup]; }

	void AddObject(CObject* _obj);
	void DeleteObjects(E_GroupType _eType);
	void DeleteAllObjects();

	CObject* FindObject(const wstring& _objectName);
	CObject* FindObject(const wstring& _objectName, E_GroupType _eGroupType);

	// Camera
	CCamera* GetMainCamera() { return m_pMainCamera; }
	void SetMainCamera(CCamera* pCamera) { m_pMainCamera = pCamera;	}

public:
	CScene();
	virtual ~CScene();
};