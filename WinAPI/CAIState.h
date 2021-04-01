#pragma once
class CAI;
class CMonster;
class CAIState
{
private:
	wstring m_strName;
	CAI* m_pAI;

public:
	virtual void Update() = 0;
	virtual void LateUpdate() {};

	virtual void Start() = 0;
	virtual void End() = 0;

public:
	void ChangeState(const wstring& _strNextState);

	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	CMonster* GetMonster();

public:
	CAIState();
	virtual ~CAIState();

	friend class CAI;
};