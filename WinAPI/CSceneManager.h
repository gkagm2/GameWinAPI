#pragma once
class CScene;
class CSceneManager
{
	SINGLETON(CSceneManager)
private:
	CScene* m_arrScene[(UINT)E_SceneType::END];
	CScene* m_pCurScene;

public:
	void Init();
	void Progress();
	void Render(HDC _hDC);
	
public:
	void ChangeScene(E_SceneType _eSceneType);
	CScene* GetCurScene() { return m_pCurScene; }
};