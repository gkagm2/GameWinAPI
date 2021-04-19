#pragma once
#define SINGLETON(class_name) \
public:\
	static	class_name* GetInstance() { \
		static class_name instance;\
		return &instance;\
	}\
private:\
	class_name();\
	virtual ~class_name();

#define DeltaTime CTimeManager::GetInstance()->GetDeltaTime()

#define InputKeyHold(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::HOLD)
#define InputKeyPress(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::PRESS)
#define InputKeyRelease(eKeyType) (CKeyManager::GetInstance()->GetKeyState(eKeyType) == E_KeyState::RELEASE)

#define CLONE(type) type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) type* Clone() { assert(nullptr); return nullptr; }

#define MousePosition CKeyManager::GetInstance()->GetMousePosition()
#define MainCamera CSceneManager::GetInstance()->GetCurScene()->GetMainCamera()

#define Debug CDebug::GetInstance()

// Scene Type
enum class E_SceneType{
	START,
	STAGE01,
	TOOL,
	GTA_MAIN,
	GTA_IN,
	END,
};

// Layer Type
enum class E_GroupType {
	TILEMAP = 0,
	TILE,
	DEFAULT,
	BACKGROUND,
	CITIZEN,
	PLAYER,
	ITEM,
	VEHICLE,
	PLAYER_PROJECTILE,
	PROJECTILE,
	PUNCH,
	MONSTER,
	MONSTER_PROJECTILE,
	PREV,
	UI =30,
	MAIN_CAMERA = 31,
	END = 32,
};

// Tile Type
enum class E_TileType {
	Road,
	Sidewalk,
	Wall,
	Water,
	None
};

// Weapon state (Attack state)
enum class E_WeaponType {
	FIST, // none
	PISTOL,
	SUBMACHINE_GUN,
	SHOTGUN,
	ROCKET_LAUNCHER,
	END,
};

// Item type
enum class E_ItemType {
	WEAPON,
	END
};

enum class E_ItemState {
	ON,
	OFF
};

struct TWeaponInfo {
	wstring strName;
	float fSplashRange;
	float fDamage;
	int iBulletCnt;
	bool bSplashDamage;
	bool bIsInfinite;
	float fShootCoolTime;
	

	void Save(FILE* _pFile);
	void Load(FILE* _pFile);

	TWeaponInfo();
	TWeaponInfo(const TWeaponInfo& _other);
};

#define EXCEPTION_COLOR_RGB_MAGENTA RGB(255,0,255) // Magenta color
#define EXCEPTION_COLOR_RGB_BLACK RGB(0,0,0) // Magenta color

#define TILE_SIZE 64
#define TILE_SIZE_ITEM 40

// Math
#define PI 3.14159265f