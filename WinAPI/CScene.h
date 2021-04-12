#pragma once
class CObject; // ���� ������ �ؾ� ã�ư��� ���ŷο��� ����.
class CTileMap;
class CCamera;
class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)E_GroupType::END];

protected:
	CTileMap* m_pTileMap; // ����ϴ� Scene�� Tile�� ����� ��� ����Ŵ.
	CCamera* m_pMainCamera; // ����ϴ� Scene�� Main ī�޶� ����Ŵ.

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
	void LoadAll(wstring _strPath);

public:
	virtual void Start() = 0;
	virtual void End() = 0;

	vector<CObject*>& GetObjects(E_GroupType _eGroup) { return m_vecObj[(UINT)_eGroup]; }

	void AddObject(CObject* _obj);
	void DeleteObjects(E_GroupType _eType);
	void DeleteAllObjects();

	CObject* FindObject(wstring _objectName);

	// Camera
	CCamera* GetMainCamera() { return m_pMainCamera; }
	void SetMainCamera(CCamera* pCamera) { m_pMainCamera = pCamera;	}

public:
	CScene();
	virtual ~CScene();
};