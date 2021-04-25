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

	bool m_bIsRender;
	bool m_bIsActive;

public:
	void Update();
	void LateUpdate();
	void Render(HDC _hDC);

public:
	
	void ActiveRender(bool _bIsRender) { m_bIsRender = _bIsRender; }
	bool IsRender() { return m_bIsRender; }

	bool SetActive(bool _bIsActive) { m_bIsActive = _bIsActive; }
	bool IsActive() { return m_bIsActive; }

	void AddState(const wstring& _strKey, CGTA_AIState* _pState);
	void ChangeState(const wstring& _strKey);
	CGTA_Character* GetCharacter() { return m_pCharacter; }
	CGTA_AIState* GetCurState() { return m_pCurState; }

	virtual void Move(float _fSpeed);
	virtual void RotateBody();


public:
	CLONE(CGTA_AI);
	CGTA_AI(const CGTA_AI& _origin);

public:
	CGTA_AI();
	virtual ~CGTA_AI() final;

	friend class CGTA_Character;
};