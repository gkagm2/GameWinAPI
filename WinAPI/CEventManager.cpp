#include "stdafx.h"
#include "CEventManager.h"

#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"
#include "CAI.h"

CEventManager::CEventManager() {

}

CEventManager::~CEventManager() {

}

void CEventManager::Update()
{
	// ���� ���� ������Ʈ�� ����
	while (!m_queDeadObj.empty()) {
		CObject* pObject = m_queDeadObj.front();
		m_queDeadObj.pop();

		assert(nullptr != pObject);
		delete pObject;
	}

	// �̺�Ʈ ó��
	while (!m_queEvent.empty()) {
		Execute(m_queEvent.front());
		m_queEvent.pop();
	}
}

void CEventManager::Execute(const tEvent& _event)
{
	switch (_event.eType) {
		// lParam : CObject �ּҰ�.
		// wParam : ��� ����.
	case E_EventType::CREATE_OBJECT:
	{
		CObject* pNewObject = (CObject*)_event.lParam;

		CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
		pCurScene->AddObject(pNewObject);
	}
		break;
	case E_EventType::DELETE_OBJECT:
	{
		CObject* pDeleteObject = (CObject*)_event.lParam;
		if (!pDeleteObject->IsDead()) {
			pDeleteObject->_SetIsDead(true);
			m_queDeadObj.push(pDeleteObject);
		}
	}
		break;
		// lParam : ���ڿ� �ּ�
		// wParam : AI �ּ�
	case E_EventType::CHANGE_STATE: // AI State Change Event
	{
		E_EventType type = _event.eType;
		wstring* pStrKey= (wstring*)_event.lParam;
		CAI* pAI = (CAI*)_event.wParam;
		pAI->ChangeState(*pStrKey);
		delete pStrKey;
	}
		break;
	case E_EventType::COLLIDER_ACTIVE:
	{
		CCollider* pCollider = (CCollider*)_event.lParam;
		bool isActive = (bool)_event.wParam;
		pCollider->m_bIsActive = isActive;
	}
		break;
	}
}
