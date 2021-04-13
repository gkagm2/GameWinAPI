#pragma once

enum class E_CollisionState {
	ENTER_COLLISION,	// ó�� ���� �� 
	STAY_COLLISION,		// ���� ��
	EXIT_COLLISION,		// ������ ���� �� 
	END
};

class CCollider;
class CColliderRect;
class CColliderCircle;

union U_ColliderID {
	struct {
		UINT iLeftID;
		UINT iRightID;
	};
	LONGLONG llID;
};

class CCollisionManager
{
	SINGLETON(CCollisionManager)
private:
	bitset<(UINT)E_GroupType::END> m_bitsetCollisionGroup[(UINT)E_GroupType::END];// (use group size by group size)

	unordered_map<LONGLONG, bool> m_umapPrevCollisionInfo; // ���� �������� �浹 ������ ������.

public:
	void Update();
	void CollisionByGroup(UINT _iGroup1Idx, UINT _iGroup2Idx); // �׷찣 �浹

private:
	bool _IsCollision(CColliderRect* _pColRect1, CColliderRect* _pColRect2, RECT* _ptIntersectionRect = nullptr); 
	bool _IsCollision(CColliderRect* _pColRect, CColliderCircle* _pColCircle);
	bool _IsCollision(CColliderCircle* _pColCircle, CColliderRect* _pColRect);
	bool _IsCollision(CColliderCircle* _pColCircle1, CColliderCircle* _pColCircle2);
	bool _IsCollision(CColliderCircle* _pColCircle, const Vector3& _vPoint);
	bool _IsCollision(CColliderRect* _pColRect, const Vector3& _vPoint);
	bool _IsCollision(const Vector3& _vPoint1, const Vector3& _vPoint2);
	bool IsCollision(CCollider* _col1, CCollider* _col2);

public:
	void SetNotIntersection(CColliderRect* _pColRect1, CColliderRect* _pColRect2);

public:
	// ������ �� �׷쳢�� �浹�� ���� ���� flag�� �����Ѵ�.
	void SetOnOffCollisionGroup(E_GroupType _eGroup1, E_GroupType _eGroup2, bool _bIsOn);
	void ClearAllCollisionGroup() { 
		for (UINT i = 0; i < (UINT)E_GroupType::END; ++i)
			m_bitsetCollisionGroup[i].reset();
		m_umapPrevCollisionInfo.clear();
	}
};