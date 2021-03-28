#include "stdafx.h"
#include "CCollisionManager.h"
#include "CCore.h"
#include "CCollider.h"
#include "CObject.h"
#include "CColliderCircle.h"
#include "CColliderRect.h"
#include "CCamera.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CScene.h"

CCollisionManager::CCollisionManager() :
	m_bitsetCollisionGroup{}
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Update()
{
	UINT iGroupSize = (UINT)E_GroupType::END;
	for (UINT i = 0; i < iGroupSize; ++i) {
		for (UINT j = i; j < iGroupSize; ++j) {
			if (m_bitsetCollisionGroup[i].test(j)) {
				CollisionByGroup(i, j); // �׷찣 �浹
			}
		}
	}
}

void CCollisionManager::CollisionByGroup(UINT _iGroup1Idx, UINT _iGroup2Idx)
{
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	const vector<CObject*>& vecLeft = pCurScene->GetObjects((E_GroupType)_iGroup1Idx);
	const vector<CObject*>& vecRight = pCurScene->GetObjects((E_GroupType)_iGroup2Idx);

	for (UINT i = 0; i < vecLeft.size(); ++i) {
		for (UINT j = 0; j < vecRight.size(); ++j) {
			if (nullptr == vecLeft[i]->GetCollider() || nullptr == vecRight[j]->GetCollider())
				continue;
			if (_iGroup1Idx == _iGroup2Idx && i == j)
				continue;
			if (!vecLeft[i]->GetCollider()->IsActive() || !vecRight[j]->GetCollider()->IsActive())
				continue;

			// �� �浹ü�� ���� ���̵� �����ؼ� Ű���� ����� ��
			U_ColliderID id{};
			id.iLeftID = vecLeft[i]->GetCollider()->GetID();
			id.iRightID = vecRight[j]->GetCollider()->GetID();

			unordered_map<LONGLONG, bool>::iterator iter = m_umapPrevCollisionInfo.find(id.llID);
			if (iter == m_umapPrevCollisionInfo.end()) {
				m_umapPrevCollisionInfo.insert(make_pair(id.llID, false));
				iter = m_umapPrevCollisionInfo.find(id.llID);
			}

			// �浹 �˻�
			if (IsCollision(vecLeft[i]->GetCollider(), vecRight[j]->GetCollider())) { // �浹���� ���
				if (iter->second) { // �������� �浹���� ��

					// �� �� �ϳ��� ���� ���� ������ ���
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead()) {
						// �浹�� �����.
						vecLeft[i]->GetCollider()->OnCollisionExit(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnCollisionExit(vecLeft[i]->GetCollider());
						iter->second = false;
					}
					else {
						vecLeft[i]->GetCollider()->OnCollisionStay(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnCollisionStay(vecLeft[i]->GetCollider());
					}
				}
				else {
					// �Ѵ� ���� ������ �ƴϰ� ó�� �浹 �� 
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead()) {
						vecLeft[i]->GetCollider()->OnCollisionEnter(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnCollisionEnter(vecLeft[i]->GetCollider());
						iter->second = true;
					}
				}
			}
			else { // �浹���� �ʾ��� ���
				if (iter->second) { // ������ �浹���� ���
					vecLeft[i]->GetCollider()->OnCollisionExit(vecRight[j]->GetCollider());
					vecRight[j]->GetCollider()->OnCollisionExit(vecLeft[i]->GetCollider());
					iter->second = false;
				}
			}
		}
	}
}

// �簢���� �簢�� �浹üũ
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect1, CColliderRect* _pColRect2)
{
	// TODO : �ϴ� AABB�� �����. ȸ�� �ϸ� �׶� OBB�� �ٲٱ�
	if( (_pColRect1->GetScale().x + _pColRect2->GetScale().x)/2.0f < abs(_pColRect1->GetPosition().x - _pColRect2->GetPosition().x) ||
		(_pColRect1->GetScale().y + _pColRect2->GetScale().y)/2.0f < abs(_pColRect1->GetPosition().y - _pColRect2->GetPosition().y)) {
		return false;
	}
	return true;
}

// �簢���� �� �浹üũ
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect, CColliderCircle* _pColCircle)
{
	return _IsCollision(_pColCircle, _pColRect);
}
bool CCollisionManager::_IsCollision(CColliderCircle* _pColCircle, CColliderRect* _pColRect)
{
	const Vector3& vCirclePosition = _pColCircle->GetPosition();
	float fCircleRadius = _pColCircle->GetRadius();

	// Ȯ��� �簢�� ���� �ȿ� ���� ���� ����־��
	if (_pColRect->GetMinPos().x <= vCirclePosition.x && _pColRect->GetMaxPos().x >= vCirclePosition.x ||
		_pColRect->GetMinPos().y <= vCirclePosition.y && _pColRect->GetMaxPos().y >= vCirclePosition.y) {
		
		// ���� ��������ŭ Ȯ��� �簢���� min, max���� ���Ѵ�.
		Vector3 vExpansionRectMin = _pColRect->GetMinPos() - fCircleRadius;
		Vector3 vExpansionRectMax = _pColRect->GetMaxPos() + fCircleRadius;

		// Ȯ���� �簢���ȿ� ���� �߽��� ���������
		if (vExpansionRectMin.x < vCirclePosition.x && vExpansionRectMax.x > vCirclePosition.x &&
			vExpansionRectMin.y < vCirclePosition.y && vExpansionRectMax.y > vCirclePosition.y)
			return true;
	}
	else { // ���� ���̸�
		Vector3 vRectPoint = _pColRect->GetMinPos();						// �簢���� ��-����� üũ
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		vRectPoint.Set(_pColRect->GetMaxPos());								// �簢���� ��-�ϴ��� üũ
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		vRectPoint.Set(_pColRect->GetMinPos().x, _pColRect->GetMaxPos().y); // �簢���� ��-�ϴ��� üũ
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		vRectPoint.Set(_pColRect->GetMaxPos().x, _pColRect->GetMinPos().y); // �簢���� ��-����� üũ
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		
	}
	return false;
}

// ���� �� �浹üũ
bool CCollisionManager::_IsCollision(CColliderCircle* _pColCircle1, CColliderCircle* _pColCircle2)
{
	float fRadius1 = _pColCircle1->GetRadius();
	float fRadius2 = _pColCircle2->GetRadius();
	// ���� ������ ���� ������ ���� ������� ����
	float fDistance = CMyMath::GetSqrMagnitude(_pColCircle1->GetPosition(), _pColCircle2->GetPosition());

	if (fDistance <= fRadius1 * fRadius1 + fRadius2 * fRadius2)
		return true;
	return false;
}

// ���� �� �浹üũ
bool CCollisionManager::_IsCollision(CColliderCircle* _pColCircle, const Vector3& _vPoint)
{
	float fDistance =CMyMath::GetDistance(_pColCircle->GetPosition(), _vPoint); // ���� �߽ɰ� ������ �Ÿ�
	if(fDistance <= _pColCircle->GetRadius())			// ���� �߽ɰ� ������ �Ÿ��� ���� ���������� ������ �浹
		return true;
	return false;
}

// �簢���� �� �浹üũ
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect, const Vector3& _vPoint)
{
	const Vector3& minPos = _pColRect->GetMinPos();
	const Vector3& maxPos = _pColRect->GetMaxPos();
	if (minPos.x >= _vPoint.x && minPos.y >= _vPoint.y && minPos.z >= _vPoint.z &&
		maxPos.x <= _vPoint.x && maxPos.y <= _vPoint.y && maxPos.z <= _vPoint.z)
		return true;
	return false;
}

// ���� �� �浹üũ
bool CCollisionManager::_IsCollision(const Vector3& _vPoint1, const Vector3& _vPoint2)
{
	return _vPoint1 == _vPoint2;
}

bool CCollisionManager::IsCollision(CCollider* _col1, CCollider* _col2)
{
	{
		auto childCol1 = dynamic_cast<CColliderCircle*>(_col1);
		auto childCol2 = dynamic_cast<CColliderCircle*>(_col2);
		if (childCol1 && childCol2)
			return _IsCollision(childCol1, childCol2);
	}
	{
		auto childCol1 = dynamic_cast<CColliderCircle*>(_col1);
		auto childCol2 = dynamic_cast<CColliderRect*>(_col2);
		if (childCol1 && childCol2)
			return _IsCollision(childCol1, childCol2);
	}
	{
		auto childCol1 = dynamic_cast<CColliderRect*>(_col1);
		auto childCol2 = dynamic_cast<CColliderCircle*>(_col2);
		if (childCol1 && childCol2)
			return _IsCollision(childCol1, childCol2);
	}
	{
		auto childCol1 = dynamic_cast<CColliderRect*>(_col1);
		auto childCol2 = dynamic_cast<CColliderRect*>(_col2);
		if (childCol1 && childCol2)
			return _IsCollision(childCol1, childCol2);
	}
	return false;
}

void CCollisionManager::SetOnOffCollisionGroup(E_GroupType _eGroup1, E_GroupType _eGroup2, bool _bIsOn)
{
	UINT iRow = min((UINT)_eGroup1, (UINT)_eGroup2); // ���� �� ���� ������ ���Ѵ�.
	UINT iColumn = max((UINT)_eGroup1, (UINT)_eGroup2);
	
	if (_bIsOn)
		m_bitsetCollisionGroup[iRow].set(iColumn); // set bit to 1
	else
		m_bitsetCollisionGroup[iRow].reset(iColumn); // set bit to 0
}