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
						// 하나라도 active가 false가 된게 있으면
						if (!vecLeft[i]->GetCollider()->IsActive() || !vecRight[j]->GetCollider()->IsActive()) {
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
				}
				else {
					// 둘다 삭제 예정이 아니고 처음 충돌 시 
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead()) {
						if (vecLeft[i]->GetCollider()->IsActive() && vecRight[j]->GetCollider()->IsActive()) {
							vecLeft[i]->GetCollider()->OnCollisionEnter(vecRight[j]->GetCollider());
							vecRight[j]->GetCollider()->OnCollisionEnter(vecLeft[i]->GetCollider());
							iter->second = true;
						}
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

// 사각형과 사각형 충돌체크 AABB
/*
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect1, CColliderRect* _pColRect2, RECT* _ptIntersectionRect)
{
	if( (_pColRect1->GetScale().x + _pColRect2->GetScale().x) * 0.5f < abs(_pColRect1->GetPosition().x - _pColRect2->GetPosition().x) ||
		(_pColRect1->GetScale().y + _pColRect2->GetScale().y) * 0.5f < abs(_pColRect1->GetPosition().y - _pColRect2->GetPosition().y)) {
		if (nullptr != _ptIntersectionRect) {
			_ptIntersectionRect->left = 0;
			_ptIntersectionRect->right = 0;
			_ptIntersectionRect->top = 0;
			_ptIntersectionRect->bottom = 0;
		}
		return false;
	}

	// 교차 영역을 담을 포인터 변수가 존재하면
	if (nullptr != _ptIntersectionRect) {
		// 수평 충돌
		_ptIntersectionRect->left = max(_pColRect1->GetMinPos().x, _pColRect2->GetMinPos().x);
		_ptIntersectionRect->right = min(_pColRect1->GetMaxPos().x, _pColRect2->GetMaxPos().x);
		// 수직 충돌
		_ptIntersectionRect->top = max(_pColRect1->GetMinPos().y, _pColRect2->GetMinPos().y);
		_ptIntersectionRect->bottom = min(_pColRect1->GetMaxPos().y, _pColRect2->GetMaxPos().y);
	}

	return true;
}
*/

// 사각형과 사각형 충돌체크 OBB 2D
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect1, CColliderRect* _pColRect2, RECT* _ptIntersectionRect) {
	// 투영시킬 각 사각형의 up, right 벡터
	Vector3 vProjection[]{ _pColRect1->GetOwnerObject()->GetUpVector(), _pColRect1->GetOwnerObject()->GetRightVector(), _pColRect2->GetOwnerObject()->GetUpVector(), _pColRect2->GetOwnerObject()->GetRightVector() };

	// 각 오브젝트들의 width과 height의 반지름
	float v1HalfWidth = _pColRect1->GetScale().x * 0.5f;
	float v1HalfHeight = _pColRect1->GetScale().y * 0.5f;
	float v2HalfWidth = _pColRect2->GetScale().x * 0.5f;
	float v2HalfHeight = _pColRect2->GetScale().y * 0.5f;

	for (int i = 0; i < 4; ++i) {
		Vector3 vProj = vProjection[i]; // 투영 벡터

		// 오브젝트1
		Vector3 v1RightExtend = _pColRect1->GetOwnerObject()->GetRightVector() * v1HalfWidth;
		Vector3 v1UpExtend = _pColRect1->GetOwnerObject()->GetUpVector() * v1HalfHeight;

		// 오브젝트2
		Vector3 v2RightExtend = _pColRect2->GetOwnerObject()->GetRightVector() * v2HalfWidth;
		Vector3 v2UpExtend = _pColRect2->GetOwnerObject()->GetUpVector() * v2HalfHeight;

		// 두개의 사각형들의 중점 사이의 거리
		float fDistance = fabsf(CMyMath::GetDot(vProj, (_pColRect2->GetPosition() - _pColRect1->GetPosition())));

		float fDis1 = fabsf(CMyMath::GetDot(vProj, v1RightExtend));
		float fDis2 = fabsf(CMyMath::GetDot(vProj, v1UpExtend));
		float fDis3 = fabsf(CMyMath::GetDot(vProj, v2RightExtend));
		float fDis4 = fabsf(CMyMath::GetDot(vProj, v2UpExtend));

		if (fDistance > fDis1 + fDis2 + fDis3 + fDis4) {
			if (nullptr != _ptIntersectionRect) {
				_ptIntersectionRect->left = 0;
				_ptIntersectionRect->right = 0;
				_ptIntersectionRect->top = 0;
				_ptIntersectionRect->bottom = 0;
			}
			return false;
		}

		// 교차 영역을 담을 포인터 변수가 존재하면
		if (nullptr != _ptIntersectionRect) {
			Vector3 vObj1Pos = _pColRect1->GetOwnerObject()->GetPosition();
			Vector3 vObj2Pos = _pColRect2->GetOwnerObject()->GetPosition();
			// 수평 충돌
			_ptIntersectionRect->left = max(min(vObj1Pos.x + v1RightExtend.x, vObj1Pos.x + v1UpExtend.x), min(vObj2Pos.x + v2RightExtend.x, vObj2Pos.x + v2UpExtend.x));
			_ptIntersectionRect->right = min(max(vObj1Pos.x + v1RightExtend.x, vObj1Pos.x + v1UpExtend.x), max(vObj2Pos.x + v2RightExtend.x, vObj2Pos.x + v2UpExtend.x));

			// 수직 충돌
			_ptIntersectionRect->top = max(min(vObj1Pos.y + v1RightExtend.y, vObj1Pos.y + v1UpExtend.y), min(vObj2Pos.x + v2RightExtend.y, vObj2Pos.x + v2UpExtend.y));
			_ptIntersectionRect->bottom = min(max(vObj1Pos.y + v1RightExtend.y, vObj1Pos.y + v1UpExtend.y), max(vObj2Pos.x + v2RightExtend.y, vObj2Pos.x + v2UpExtend.y));
		}
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
	else { // 범위 밖이면 (사각형 꼭지점이 원 안에 들어왔는지 체크)
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
	if (minPos.x <= _vPoint.x && minPos.y <= _vPoint.y && minPos.z <= _vPoint.z &&
		maxPos.x >= _vPoint.x && maxPos.y >= _vPoint.y && maxPos.z >= _vPoint.z)
		return true;
	return false;
}

// 점과 점 충돌체크
bool CCollisionManager::IsCollision(const Vector3& _vPoint1, const Vector3& _vPoint2)
{
	return _vPoint1 == _vPoint2;
}

// 콜라이더와 픽셀 좌표 충돌체크
bool CCollisionManager::IsCollision(CCollider* _col, const Vector3& _vPoint)
{
	{
		auto childCol = dynamic_cast<CColliderCircle*>(_col);
		if (childCol)
			return _IsCollision(childCol, _vPoint);
	}
	{
		auto childCol = dynamic_cast<CColliderRect*>(_col);
		if (childCol)
			return _IsCollision(childCol, _vPoint);
	}
	return false;
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
		if (childCol1 && childCol2) {
			return _IsCollision(childCol1, childCol2);
		}	
	}

	return false;
}

// 사각형과 사각형 충돌 시 교차되지 않게 하기 (충돌되었을 경우 실행된다)
void CCollisionManager::SetNotIntersection(CColliderRect* _pColRect, CColliderRect* _pColHoldRect)
{
	RECT tIntersectionRect{}; // 교차 영역을 얻을 변수
	if (_IsCollision(_pColRect, _pColHoldRect, &tIntersectionRect)) {
		// 교차된 영역의 가로 세로의 크기를 따진다.
		int iWidth = tIntersectionRect.right - tIntersectionRect.left;
		int iHeight = tIntersectionRect.bottom - tIntersectionRect.top;

		// 위 아래 체크
		if (iWidth > iHeight) {
			// 위에서 충돌 시
			if (tIntersectionRect.top == _pColHoldRect->GetMinPos().y) {
				// 충돌 수직 높이만큼 위로 밀어줌
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x, vRectPos.y - iHeight, vRectPos.z);
			}
			// 아래에서 충돌 시
			else if (tIntersectionRect.bottom == _pColHoldRect->GetMaxPos().y) {
				// 충돌 수직 높이만큼 아래로 밀어줌
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x, vRectPos.y + iHeight, vRectPos.z);
			}
		}
		// 좌우 체크
		else {
			// 왼쪽에서 충돌 시
			if (tIntersectionRect.left == _pColHoldRect->GetMinPos().x) {
				// 충돌된 가로 길이만큼 왼쪽으로 밀어줌
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x - iWidth, vRectPos.y, vRectPos.z);
			}
			// 오른쪽에서 충돌 시
			else if (tIntersectionRect.right == _pColHoldRect->GetMaxPos().x) {
				// 충돌된 가로 길이만큼 오른쪽으로 밀어줌
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x + iWidth, vRectPos.y, vRectPos.z);
			}
		}
	}
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