#pragma once
#include "CComponent.h"
class CGTA_AIState;
class CGTA_Character;
class CGTA_AI : public CComponent
{
private:
	unordered_map<wstring, CGTA_AIState*> m_umapState;
	CGTA_AIState* m_pCurState;
	CGTA_Character* m_pCharacter;

public:
	void Update();
	void LateUpdate();

public:
	void AddState(const wstring& _strKey, CGTA_AIState* _pState);
	void ChangeState(const wstring& _strKey);
	CGTA_Character* GetCharacter() { return m_pCharacter; }
	CGTA_AIState* GetCurState() { return m_pCurState; }

public:
	CLONE(CGTA_AI);

	CGTA_AI(const CGTA_AI& _origin);

public:
	CGTA_AI();
	virtual ~CGTA_AI() final;

	friend class CGTA_Character;
};