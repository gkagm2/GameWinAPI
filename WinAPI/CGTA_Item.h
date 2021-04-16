#pragma once
#include "CObject.h"

enum class E_ItemType;
class CGTA_Item : public CObject
{
private:
	Vector2 m_vLT;		// Texture속 각 아이템의 좌상단
	E_ItemType m_eItemType; // Item type

public:
	virtual void Init() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) override;

public:
	//Texture 
	void SetLT(const Vector2& _vLT) { m_vLT = _vLT; }
	const Vector2& GetLT() { return m_vLT; }

	void SetItemType(E_ItemType _eItemType) { m_eItemType = _eItemType; }

public:
	CLONE(CGTA_Item);

public:
	CGTA_Item() = delete;
	CGTA_Item(E_GroupType _eGroupType);
	virtual ~CGTA_Item() override;
};