#pragma once
#include "stdafx.h"
class CGTA_AI;
class CGTA_Character;
class CPathFinding;
class CGTA_AIState
{
private:
	wstring m_strName;
	CGTA_AI* m_pAI;

public:
	virtual void Update() = 0;
	virtual void LateUpdate() {};

	virtual void Start() = 0;
	virtual void End() = 0;

public:
	void ChangeState(const wstring& _strNextState);

	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	CGTA_AI* GetAI() { return m_pAI; }
	void SetAI(CGTA_AI* _ai) { m_pAI = _ai; }

	CGTA_Character* GetCharacter();

public:
	virtual CGTA_AIState* Clone() = 0;

	CGTA_AIState(const CGTA_AIState& _origin);

public:
	CGTA_AIState();
	virtual ~CGTA_AIState();

	friend class CAI;
};