#pragma once

// FSM(Finite State Machine), 유한 상태 기계ㅋ	
class CAIState;
class CMonster;
class C_AI
{
private:
	unordered_map<wstring, CAIState*> m_umapState;
	CAIState* m_pCurState;
	CMonster* m_pMonster;

public:
	void Update();
	void LateUpdate();

public:
	void AddState(const wstring& _strKey, CAIState* _pState);
	void ChangeState(const wstring& _strKey);
	CMonster* GetMonster() { return m_pMonster; }
	CAIState* GetCurState() { return m_pCurState; }

public:
	CLONE(C_AI);	

public:
	C_AI();
	virtual ~C_AI() final;
	
	friend class CMonster;
};