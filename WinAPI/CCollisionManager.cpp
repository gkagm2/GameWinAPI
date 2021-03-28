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
				CollisionByGroup(i, j); // 그룹간 충돌
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

			// 두 충돌체의 고유 아이디를 조합해서 키값을 만들어 냄
			U_ColliderID id{};
			id.iLeftID = vecLeft[i]->GetCollider()->GetID();
			id.iRightID = vecRight[j]->GetCollider()->GetID();

			unordered_map<LONGLONG, bool>::iterator iter = m_umapPrevCollisionInfo.find(id.llID);
			if (iter == m_umapPrevCollisionInfo.end()) {
				m_umapPrevCollisionInfo.insert(make_pair(id.llID, false));
				iter = m_umapPrevCollisionInfo.find(id.llID);
			}

			// 충돌 검사
			if (IsCollision(vecLeft[i]->GetCollider(), vecRight[j]->GetCollider())) { // 충돌했을 경우
				if (iter->second) { // 이전에도 충돌했을 시

					// 둘 중 하나라도 삭제 예정 상태일 경우
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead()) {
						// 충돌을 벗어난다.
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
					// 둘다 삭제 예정이 아니고 처음 충돌 시 
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead()) {
						vecLeft[i]->GetCollider()->OnCollisionEnter(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnCollisionEnter(vecLeft[i]->GetCollider());
						iter->second = true;
					}
				}
			}
			else { // 충돌하지 않았을 경우
				if (iter->second) { // 이전에 충돌했을 경우
					vecLeft[i]->GetCollider()->OnCollisionExit(vecRight[j]->GetCollider());
					vecRight[j]->GetCollider()->OnCollisionExit(vecLeft[i]->GetCollider());
					iter->second = false;
				}
			}
		}
	}
}

// 사각형과 사각형 충돌체크
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect1, CColliderRect* _pColRect2)
{
	// TODO : 일단 AABB로 만든다. 회전 하면 그때 OBB로 바꾸기
	if( (_pColRect1->GetScale().x + _pColRect2->GetScale().x)/2.0f < abs(_pColRect1->GetPosition().x - _pColRect2->GetPosition().x) ||
		(_pColRect1->GetScale().y + _pColRect2->GetScale().y)/2.0f < abs(_pColRect1->GetPosition().y - _pColRect2->GetPosition().y)) {
		return false;
	}
	return true;
}

// 사각형과 원 충돌체크
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect, CColliderCircle* _pColCircle)
{
	return _IsCollision(_pColCircle, _pColRect);
}
bool CCollisionManager::_IsCollision(CColliderCircle* _pColCircle, CColliderRect* _pColRect)
{
	const Vector3& vCirclePosition = _pColCircle->GetPosition();
	float fCircleRadius = _pColCircle->GetRadius();

	// 확장된 사각형 범위 안에 원의 점이 들어있어면
	if (_pColRect->GetMinPos().x <= vCirclePosition.x && _pColRect->GetMaxPos().x >= vCirclePosition.x ||
		_pColRect->GetMinPos().y <= vCirclePosition.y && _pColRect->GetMaxPos().y >= vCirclePosition.y) {
		
		// 원의 반지름만큼 확장된 사각형의 min, max값을 구한다.
		Vector3 vExpansionRectMin = _pColRect->GetMinPos() - fCircleRadius;
		Vector3 vExpansionRectMax = _pColRect->GetMaxPos() + fCircleRadius;

		// 확장한 사각형안에 원의 중심이 들어있으면
		if (vExpansionRectMin.x < vCirclePosition.x && vExpansionRectMax.x > vCirclePosition.x &&
			vExpansionRectMin.y < vCirclePosition.y && vExpansionRectMax.y > vCirclePosition.y)
			return true;
	}
	else { // 범위 밖이면
		Vector3 vRectPoint = _pColRect->GetMinPos();						// 사각형의 좌-상단점 체크
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		vRectPoint.Set(_pColRect->GetMaxPos());								// 사각형의 우-하단점 체크
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		vRectPoint.Set(_pColRect->GetMinPos().x, _pColRect->GetMaxPos().y); // 사각형의 좌-하단점 체크
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		vRectPoint.Set(_pColRect->GetMaxPos().x, _pColRect->GetMinPos().y); // 사각형의 우-상단점 체크
		if (_IsCollision(_pColCircle, vRectPoint))
			return true;
		
	}
	return false;
}

// 원과 원 충돌체크
bool CCollisionManager::_IsCollision(CColliderCircle* _pColCircle1, CColliderCircle* _pColCircle2)
{
	float fRadius1 = _pColCircle1->GetRadius();
	float fRadius2 = _pColCircle2->GetRadius();
	// 빠른 연산을 위해 제곱근 연산 사용하지 않음
	float fDistance = CMyMath::GetSqrMagnitude(_pColCircle1->GetPosition(), _pColCircle2->GetPosition());

	if (fDistance <= fRadius1 * fRadius1 + fRadius2 * fRadius2)
		return true;
	return false;
}

// 원과 점 충돌체크
bool CCollisionManager::_IsCollision(CColliderCircle* _pColCircle, const Vector3& _vPoint)
{
	float fDistance =CMyMath::GetDistance(_pColCircle->GetPosition(), _vPoint); // 원의 중심과 점간의 거리
	if(fDistance <= _pColCircle->GetRadius())			// 원의 중심과 점간의 거리가 원의 반지름보다 작으면 충돌
		return true;
	return false;
}

// 사각형과 점 충돌체크
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect, const Vector3& _vPoint)
{
	const Vector3& minPos = _pColRect->GetMinPos();
	const Vector3& maxPos = _pColRect->GetMaxPos();
	if (minPos.x >= _vPoint.x && minPos.y >= _vPoint.y && minPos.z >= _vPoint.z &&
		maxPos.x <= _vPoint.x && maxPos.y <= _vPoint.y && maxPos.z <= _vPoint.z)
		return true;
	return false;
}

// 점과 점 충돌체크
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
	UINT iRow = min((UINT)_eGroup1, (UINT)_eGroup2); // 행을 더 작은 값으로 정한다.
	UINT iColumn = max((UINT)_eGroup1, (UINT)_eGroup2);
	
	if (_bIsOn)
		m_bitsetCollisionGroup[iRow].set(iColumn); // set bit to 1
	else
		m_bitsetCollisionGroup[iRow].reset(iColumn); // set bit to 0
}