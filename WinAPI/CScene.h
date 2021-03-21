#pragma once
class CObject; // ���� ������ �ؾ� ã�ư��� ���ŷο��� ����.
class CScene
{
private:
	vector<CObject*> m_vecObj[(UINT)E_GroupType::END];
public:
	virtual void PrevUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(HDC _hDC);
	
	virtual void Start() = 0;
	virtual void End() = 0;

	vector<CObject*>& GetObjects(E_GroupType _eGroup) { return m_vecObj[(UINT)_eGroup]; }

	void AddObject(CObject* _obj);
	void DeleteObjects(E_GroupType _eType);
	void DeleteAllObjects();

	CObject* FindObject(wstring _objectName);

public:
	CScene();
	virtual ~CScene();
};