#pragma once
#include "CObject.h"
class CTile : public CObject
{
private:
	static int g_iTileSize;

private:
	int m_iImageIdx;

	int m_iMaxCol;
	int m_iMaxRow;

public:
	static int GetTileSize() { return g_iTileSize; }

public:
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;

public:
	void RenderDefaultTile(HDC _hDC, const Vector3& _vRenderPos);

	void SetTile(int _iImageIdx) { m_iImageIdx = _iImageIdx; }
	int GetTileIdx() { return m_iImageIdx; }

	virtual Vector3 GetMin() override;
	virtual Vector3 GetMax() override;

	int GetCol() { return 0 == m_iMaxCol ? 0 : m_iImageIdx % m_iMaxCol; }
	int GetRow() { return 0 == m_iMaxRow ? 0 : m_iImageIdx / m_iMaxCol; }
	bool IsOutOfIdx() { return GetRow() >= m_iMaxRow; } // Ÿ�� �ؽ��� ���� �̹��� ������ �ʰ��ߴ��� üũ

	void Save(FILE* _pFile);
	void Load(FILE* _pFile);

public:
	CLONE(CTile);

public:
	CTile() = delete;
	CTile(E_GroupType _eGroupType);
	virtual ~CTile() override;
};