#pragma once

// FSM(Finite State Machine), ���� ���� ��褻	
class CAIState;
class CMonster;
class CAI
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
	CLONE(CAI);	

public:
	CAI();
	virtual ~CAI() final;
	
	friend class CMonster;
};