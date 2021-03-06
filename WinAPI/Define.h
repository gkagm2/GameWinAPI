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
enum class E_SceneType {
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
	TERRAIN,
	CITIZEN,
	PLAYER,
	ITEM,
	VEHICLE,
	PLAYER_PROJECTILE,
	PROJECTILE,
	PUNCH,
	EFFECT,
	MONSTER,
	MONSTER_PROJECTILE,
	PATH_FINDING,
	MANAGER, // 전체 관리용
	PREV,
	UI_CONTAINER,
	UI = 30,
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
	wstring strBulletSound;
	wstring strVoiceSound;

	void Save(FILE* _pFile);
	void Load(FILE* _pFile);

	TWeaponInfo();
	TWeaponInfo(const TWeaponInfo& _other);
	void InitWeapon(E_WeaponType _eWeaponType);
};

struct TTilePos {
	int x;
	int y;
	TTilePos(int _x, int _y) : x(_x), y(_y) {}
	TTilePos() : x(0), y(0) {}

	bool operator==(const TTilePos& _rhs) {
		if (x == _rhs.x && y == _rhs.y)
			return true;
		return false;
	}
	bool operator<(const TTilePos& rhs) const
	{
		if (x > rhs.x && y > rhs.y)
			return true;
		else if (x > rhs.x) {
			return true;
		}
		else if (y > rhs.y)
			return true;
		else
			return false;
	}
};

struct TTilePosComparae {
	bool operator()(const TTilePos& l, const TTilePos& r) {
		if (l.x > r.x)
			return true;
		else if (l.y > r.y)
			return true;
		else
			return false;
	}
};


#define EXCEPTION_COLOR_RGB_MAGENTA RGB(255,0,255) // Magenta color
#define EXCEPTION_COLOR_RGB_BLACK RGB(0,0,0) // Magenta color

#define TILE_SIZE 64
#define TILE_SIZE_ITEM 40
#define FONT_SIZE 10

// Math
#define PI 3.14159265f