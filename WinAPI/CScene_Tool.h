#pragma once
#include "CScene.h"
class CTileMap;
class CScene_Tool : public CScene
{
	HMENU m_hMenu; // �޴� �ڵ�

public:
	virtual void Update() override;
	virtual void Start() override;
	virtual void End() override;

public:
	
	void MouseClick();

	void SaveTile(wstring _strPath);

public:
	CScene_Tool();
	virtual ~CScene_Tool() override;
};