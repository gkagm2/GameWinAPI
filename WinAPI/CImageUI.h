#pragma once
#include "CUI.h"

class CTexture;
class CImageUI : public CUI
{
private:
	const static int g_iTileSize;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	//virtual void OnPointerEnter() override;
	//virtual void OnPointerExit() override;
	//virtual void OnPointerDown() override;
	//virtual void OnPointerUp() override;
	virtual void OnPointerClick() override;
	
public:
public:
	CImageUI() = delete;
	CImageUI(E_GroupType _eGroupType);
	virtual ~CImageUI() override;
};