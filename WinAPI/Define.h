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

#define MousePosition CKeyManager::GetInstance()->GetMousePosition()

#define Debug CDebug::GetInstance()

// Scene Type
enum class E_SceneType{
	START,
	STAGE01,
	TOOL,
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
	UI,
	END = 32,
};

#define EXCEPTION_COLOR_RGB RGB(255,0,255) // Magenta color

#define TILE_SIZE 64

// Math
#define PI 3.14159265f
