#pragma once

// ���� �����ϴ� Ű
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
	NONE,		// ���� �����Ӱ� ���� �����ӿ��� ������ ���� ���� ��	 (�ƹ��͵� ������ ���� �� )
	PRESS,		// ���� �����ӿ��� �ȴ����� ���� �����ӿ� ���� ��		 (���� Ű�� ���� ��)
	HOLD,		// ���� �����Ӱ� ���� �����ӿ��� ������ ���� ��			 (Ű�� ������ �ִ� ���� ��)
	RELEASE,	// ���� �����ӿ��� �����ְ� ���� �����ӿ� �ȴ������� ��  (������ Ű�� �� ��)
};

struct tKey {
	E_KeyState eState;	// ������ Ű ����
	bool bPress;		// ���� �����ӿ� Ű�� �������� true
};

class CKeyManager
{
	SINGLETON(CKeyManager)
private:
	vector<tKey> m_vecKey; // ���� �� �ִ� Ű�� ������ ��Ƴ���.
	Vector2 m_mousePosition;
	
public:
	void Init();
	void Update();

public:
	E_KeyState GetKeyState(E_Key _eKey) const { return m_vecKey[(int)_eKey].eState; }
	Vector2& GetMousePosition() { return m_mousePosition; }
};