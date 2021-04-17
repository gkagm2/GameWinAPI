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
						// �ϳ��� active�� false�� �Ȱ� ������
						if (!vecLeft[i]->GetCollider()->IsActive() || !vecRight[j]->GetCollider()->IsActive()) {
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
				}
				else {
					// �Ѵ� ���� ������ �ƴϰ� ó�� �浹 �� 
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead()) {
						if (vecLeft[i]->GetCollider()->IsActive() && vecRight[j]->GetCollider()->IsActive()) {
							vecLeft[i]->GetCollider()->OnCollisionEnter(vecRight[j]->GetCollider());
							vecRight[j]->GetCollider()->OnCollisionEnter(vecLeft[i]->GetCollider());
							iter->second = true;
						}
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

// �簢���� �簢�� �浹üũ AABB
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

	// ���� ������ ���� ������ ������ �����ϸ�
	if (nullptr != _ptIntersectionRect) {
		// ���� �浹
		_ptIntersectionRect->left = max(_pColRect1->GetMinPos().x, _pColRect2->GetMinPos().x);
		_ptIntersectionRect->right = min(_pColRect1->GetMaxPos().x, _pColRect2->GetMaxPos().x);
		// ���� �浹
		_ptIntersectionRect->top = max(_pColRect1->GetMinPos().y, _pColRect2->GetMinPos().y);
		_ptIntersectionRect->bottom = min(_pColRect1->GetMaxPos().y, _pColRect2->GetMaxPos().y);
	}

	return true;
}
*/

// �簢���� �簢�� �浹üũ OBB 2D
bool CCollisionManager::_IsCollision(CColliderRect* _pColRect1, CColliderRect* _pColRect2, RECT* _ptIntersectionRect) {
	// ������ų �� �簢���� up, right ����
	Vector3 vProjection[]{ _pColRect1->GetOwnerObject()->GetUpVector(), _pColRect1->GetOwnerObject()->GetRightVector(), _pColRect2->GetOwnerObject()->GetUpVector(), _pColRect2->GetOwnerObject()->GetRightVector() };

	// �� ������Ʈ���� width�� height�� ������
	float v1HalfWidth = _pColRect1->GetScale().x * 0.5f;
	float v1HalfHeight = _pColRect1->GetScale().y * 0.5f;
	float v2HalfWidth = _pColRect2->GetScale().x * 0.5f;
	float v2HalfHeight = _pColRect2->GetScale().y * 0.5f;

	for (int i = 0; i < 4; ++i) {
		Vector3 vProj = vProjection[i]; // ���� ����

		// ������Ʈ1
		Vector3 v1RightExtend = _pColRect1->GetOwnerObject()->GetRightVector() * v1HalfWidth;
		Vector3 v1UpExtend = _pColRect1->GetOwnerObject()->GetUpVector() * v1HalfHeight;

		// ������Ʈ2
		Vector3 v2RightExtend = _pColRect2->GetOwnerObject()->GetRightVector() * v2HalfWidth;
		Vector3 v2UpExtend = _pColRect2->GetOwnerObject()->GetUpVector() * v2HalfHeight;

		// �ΰ��� �簢������ ���� ������ �Ÿ�
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

		// ���� ������ ���� ������ ������ �����ϸ�
		if (nullptr != _ptIntersectionRect) {
			Vector3 vObj1Pos = _pColRect1->GetOwnerObject()->GetPosition();
			Vector3 vObj2Pos = _pColRect2->GetOwnerObject()->GetPosition();
			// ���� �浹
			_ptIntersectionRect->left = max(min(vObj1Pos.x + v1RightExtend.x, vObj1Pos.x + v1UpExtend.x), min(vObj2Pos.x + v2RightExtend.x, vObj2Pos.x + v2UpExtend.x));
			_ptIntersectionRect->right = min(max(vObj1Pos.x + v1RightExtend.x, vObj1Pos.x + v1UpExtend.x), max(vObj2Pos.x + v2RightExtend.x, vObj2Pos.x + v2UpExtend.x));

			// ���� �浹
			_ptIntersectionRect->top = max(min(vObj1Pos.y + v1RightExtend.y, vObj1Pos.y + v1UpExtend.y), min(vObj2Pos.x + v2RightExtend.y, vObj2Pos.x + v2UpExtend.y));
			_ptIntersectionRect->bottom = min(max(vObj1Pos.y + v1RightExtend.y, vObj1Pos.y + v1UpExtend.y), max(vObj2Pos.x + v2RightExtend.y, vObj2Pos.x + v2UpExtend.y));
		}
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
	else { // ���� ���̸� (�簢�� �������� �� �ȿ� ���Դ��� üũ)
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
	if (minPos.x <= _vPoint.x && minPos.y <= _vPoint.y && minPos.z <= _vPoint.z &&
		maxPos.x >= _vPoint.x && maxPos.y >= _vPoint.y && maxPos.z >= _vPoint.z)
		return true;
	return false;
}

// ���� �� �浹üũ
bool CCollisionManager::IsCollision(const Vector3& _vPoint1, const Vector3& _vPoint2)
{
	return _vPoint1 == _vPoint2;
}

// �ݶ��̴��� �ȼ� ��ǥ �浹üũ
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

// �簢���� �簢�� �浹 �� �������� �ʰ� �ϱ� (�浹�Ǿ��� ��� ����ȴ�)
void CCollisionManager::SetNotIntersection(CColliderRect* _pColRect, CColliderRect* _pColHoldRect)
{
	RECT tIntersectionRect{}; // ���� ������ ���� ����
	if (_IsCollision(_pColRect, _pColHoldRect, &tIntersectionRect)) {
		// ������ ������ ���� ������ ũ�⸦ ������.
		int iWidth = tIntersectionRect.right - tIntersectionRect.left;
		int iHeight = tIntersectionRect.bottom - tIntersectionRect.top;

		// �� �Ʒ� üũ
		if (iWidth > iHeight) {
			// ������ �浹 ��
			if (tIntersectionRect.top == _pColHoldRect->GetMinPos().y) {
				// �浹 ���� ���̸�ŭ ���� �о���
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x, vRectPos.y - iHeight, vRectPos.z);
			}
			// �Ʒ����� �浹 ��
			else if (tIntersectionRect.bottom == _pColHoldRect->GetMaxPos().y) {
				// �浹 ���� ���̸�ŭ �Ʒ��� �о���
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x, vRectPos.y + iHeight, vRectPos.z);
			}
		}
		// �¿� üũ
		else {
			// ���ʿ��� �浹 ��
			if (tIntersectionRect.left == _pColHoldRect->GetMinPos().x) {
				// �浹�� ���� ���̸�ŭ �������� �о���
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x - iWidth, vRectPos.y, vRectPos.z);
			}
			// �����ʿ��� �浹 ��
			else if (tIntersectionRect.right == _pColHoldRect->GetMaxPos().x) {
				// �浹�� ���� ���̸�ŭ ���������� �о���
				const Vector3& vRectPos = _pColRect->GetOwnerObject()->GetPosition();
				_pColRect->GetOwnerObject()->SetPosition(vRectPos.x + iWidth, vRectPos.y, vRectPos.z);
			}
		}
	}
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