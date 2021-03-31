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
	TILEMAP,
	TILE,
	DEFAULT,
	BACKGROUND,
	PLAYER,
	PLAYER_PROJECTILE,
	MONSTER,
	MONSTER_PROJECTILE,
	VEHICLE,
	UI =31,
	END = 32,
};

#define EXCEPTION_COLOR_RGB_MAGENTA RGB(255,0,255) // Magenta color
#define EXCEPTION_COLOR_RGB_BLACK RGB(0,0,0) // Magenta color

#define TILE_SIZE 64

// Math
#define PI 3.14159265f
