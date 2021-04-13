#pragma once

enum class E_CollisionState {
	ENTER_COLLISION,	// 처음 접촉 시 
	STAY_COLLISION,		// 접촉 중
	EXIT_COLLISION,		// 접촉이 끝날 시 
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

	unordered_map<LONGLONG, bool> m_umapPrevCollisionInfo; // 이전 프레임의 충돌 정보를 저장함.

public:
	void Update();
	void CollisionByGroup(UINT _iGroup1Idx, UINT _iGroup2Idx); // 그룹간 충돌

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
	// 선택한 두 그룹끼리 충돌을 할지 말지 flag를 세팅한다.
	void SetOnOffCollisionGroup(E_GroupType _eGroup1, E_GroupType _eGroup2, bool _bIsOn);
	void ClearAllCollisionGroup() { 
		for (UINT i = 0; i < (UINT)E_GroupType::END; ++i)
			m_bitsetCollisionGroup[i].reset();
		m_umapPrevCollisionInfo.clear();
	}
};