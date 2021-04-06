#pragma once
class CCamera;
class CCameraManager
{
	SINGLETON(CCameraManager)

public:
	void Init();
	void Update();
	void Render(HDC _hDC);
};