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

// Scene Type
enum class E_SceneType{
	START,
	STAGE01,
	END,
};

// Layer Type
enum class E_GroupType {
	DEFAULT,
	BACKGROUND,
	PLAYER,
	PLAYER_PROJECTILE,
	MONSTER,
	MONSTER_PROJECTILE,
	END = 32,
};

#define EXCEPTION_COLOR_RGB RGB(255,0,255) // Magenta color

// Math
#define PI 3.14159265f
