#include "stdafx.h"
#include "CScene.h"
#include "CObject.h"
#include <typeinfo>
#include "CCollisionManager.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	DeleteAllObjects();
}

void CScene::AddObject(CObject* _obj)
{
	assert(nullptr != _obj);
	E_GroupType groupType = _obj->GetGroupType();
	m_vecObj[(UINT)groupType].push_back(_obj);
}

// 그룹에 있는 모든 오브젝트 삭제
void CScene::DeleteObjects(E_GroupType _eType)
{
	for (UINT i = 0; i < m_vecObj[(UINT)_eType].size(); ++i) {
		assert(nullptr != m_vecObj[(UINT)_eType][i]);
		delete m_vecObj[(UINT)_eType][i];
	}
	m_vecObj[(UINT)_eType].clear();
}

void CScene::DeleteAllObjects()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i)
		DeleteObjects((E_GroupType)i);
}

CObject* CScene::FindObject(wstring _objectName)
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		for (UINT j = 0; j < m_vecObj[i].size(); ++j) {
			if (m_vecObj[i][j]->GetObjectName() == _objectName)
				return m_vecObj[i][j];
		}
	}
	return nullptr;
}

void CScene::PrevUpdate()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		for (UINT j = 0; j < m_vecObj[i].size(); ++j)
			if (!m_vecObj[i][j]->IsDead())
				m_vecObj[i][j]->PrevUpdate();
	}
}

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		for (UINT j = 0; j < m_vecObj[i].size(); ++j)
			if(!m_vecObj[i][j]->IsDead())
				m_vecObj[i][j]->Update();
	}
}
void CScene::LateUpdate()
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		for (UINT j = 0; j < m_vecObj[i].size(); ++j)
			if(!m_vecObj[i][j]->IsDead())
				m_vecObj[i][j]->LateUpdate();
	}
}

void CScene::Render(HDC _hDC)
{
	for (UINT i = 0; i < (UINT)E_GroupType::END; ++i) {
		vector<CObject*>::iterator iter = m_vecObj[i].begin();
		while (iter != m_vecObj[i].end()) {
			if ((*iter)->IsDead())
				iter = m_vecObj[i].erase(iter);
			else {
				(*iter)->Render(_hDC);
				++iter;
			}
		}
	}
}