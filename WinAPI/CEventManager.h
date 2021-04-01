#pragma once

enum class E_EventType {
	CREATE_OBJECT,
	DELETE_OBJECT,
	CHANGE_STATE
};

struct tEvent {
	E_EventType eType;
	DWORD_PTR lParam;  //DWORD_PTR : 가변 자료형임. 전처리기를 통해서 win64면 long long(8byte), 32버전이면 uint)4byte)
	DWORD_PTR wParam;
};

class CObject;
class CEventManager
{
	SINGLETON(CEventManager)
private:
	queue<tEvent> m_queEvent;
	queue<CObject*> m_queDeadObj;

public:
	void Update();
	void AddEvent(const tEvent& _event) { m_queEvent.push(_event); }

private:
	void Execute(const tEvent& _event);
};