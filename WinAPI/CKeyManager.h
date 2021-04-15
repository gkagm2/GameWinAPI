#pragma once

// 현재 지원하는 키
enum class E_Key {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE,
	W,
	S,
	A,
	D,
	Q,
	LBUTTON,
	RBUTTON,
	MBUTTON,
	
	LSHIFT,
	F4,
	F5,

	END,
};

enum class E_KeyState {
	NONE,		// 이전 프레임과 현재 프레임에도 누르고 있지 않을 때	 (아무것도 누르지 않을 때 )
	PRESS,		// 이전 프레임에는 안눌리고 현재 프레임에 누를 때		 (현재 키를 누를 때)
	HOLD,		// 이전 프레임과 현재 프레임에도 누르고 있을 때			 (키를 누르고 있는 중일 때)
	RELEASE,	// 이전 프레임에는 눌려있고 현재 프레임에 안눌려있을 때  (눌렀던 키를 뗄 때)
};

struct tKey {
	E_KeyState eState;	// 현재의 키 상태
	bool bPress;		// 이전 프레임에 키가 눌렸으면 true
};

class CKeyManager
{
	SINGLETON(CKeyManager)
private:
	vector<tKey> m_vecKey; // 누를 수 있는 키의 정보를 모아놓음.
	Vector2 m_mousePosition;
	
public:
	void Init();
	void Update();

public:
	E_KeyState GetKeyState(E_Key _eKey) const { return m_vecKey[(int)_eKey].eState; }
	Vector2& GetMousePosition() { return m_mousePosition; }
};