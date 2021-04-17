#pragma once
#include "CObject.h"

enum class E_ItemState;
enum class E_ItemType;
struct TWeaponInfo;
class CGTA_Item : public CObject
{
private:
	Vector2 m_vLT;		// Texture속 각 아이템의 좌상단
	E_ItemType m_eItemType; // Item type

	// Weapon Type 일 경우
	E_WeaponType m_eWeaponType;
	TWeaponInfo m_tWeaponInfo;

	float m_fTime;
	Vector3 m_vFixedScale;
	float m_fRespawnCoolTime;
	float m_fMaxRespawnCoolTime;

	E_ItemState m_eItemState;
	
public:
	virtual void Init() override;
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

	virtual void OnCollisionEnter(CObject* _pOther) override;

	virtual void Save(FILE* _pFile) override;
	virtual void Load(FILE* _pFile) override;

public:
	//Texture 
	void SetLT(const Vector2& _vLT) { m_vLT = _vLT; }
	const Vector2& GetLT() { return m_vLT; }

	void SetItemType(E_ItemType _eItemType) { m_eItemType = _eItemType; }
	E_ItemType GetItemType() { return m_eItemType; }


	// WeaponType Item
	void SetWeaponType(E_WeaponType _eWeaponType) { m_eWeaponType = _eWeaponType; }
	E_WeaponType GetWeaponType() { return m_eWeaponType; }
	void SetWeaponInfo(const TWeaponInfo& _tWeaponInfo);
	const TWeaponInfo& GetWeaponInfo() { return m_tWeaponInfo; }

	// In Game
	void SetItemState(E_ItemState _eItemState);


public:
	CLONE(CGTA_Item);

public:
	CGTA_Item() = delete;
	CGTA_Item(E_GroupType _eGroupType);
	virtual ~CGTA_Item() override;
};