#include "stdafx.h"
#include "CPathFinding.h"
#include "CTileMap.h"
#include "CTile.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include <numeric>

const int CPathFinding::m_iDirX[] = { 1, 0, -1, 0, 1, -1, 1, -1 };
const int CPathFinding::m_iDirY[] = { 0, 1, 0, -1, 1, 1, -1, -1 };

CPathFinding::CPathFinding() :
	m_pTileMap(nullptr),
	m_pvecTiles(nullptr),
	m_bFoundDestination(false)
{
	Init();
}

CPathFinding::CPathFinding(const CPathFinding& _origin) :
	m_pTileMap(_origin.m_pTileMap),
	m_pvecTiles(nullptr),
	m_bFoundDestination(_origin.m_bFoundDestination)
{
	m_stkPath.assign(_origin.m_stkPath.begin(), _origin.m_stkPath.end());
	set<E_TileType>::iterator iter = _origin.m_setObstacleTile.begin();
	for (; iter != _origin.m_setObstacleTile.end(); ++iter)
		m_setObstacleTile.insert(*iter);
}

CPathFinding::~CPathFinding()
{
}

void CPathFinding::Init()
{
	m_pTileMap = CSceneManager::GetInstance()->GetCurScene()->GetTileMap();

	vector<CObject*>& vecTile = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
	m_pvecTiles = &vecTile;
}

float CPathFinding::GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrtf(float((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

bool CPathFinding::IsValid(int x, int y) {
	int iCol = (int)m_pTileMap->GetCol();
	int iRow = (int)m_pTileMap->GetRow();
	if (x < 0 || y < 0 || x >= iCol || y >= iRow)
		return false;
	return true;
}

bool CPathFinding::IsUnBlocked(int x, int y)
{
	vector<CObject*>& vecTile = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
	int idx = y * (int)(m_pTileMap->GetCol()) + x;
	CTile* pTile = (CTile*)vecTile[idx];

	set<E_TileType>::iterator iter = m_setObstacleTile.begin();
	for (; iter != m_setObstacleTile.end(); ++iter) {
		E_TileType tileType = *iter;
		if (tileType == pTile->GetTileType())
			return false;
	}
	return true;
}

bool CPathFinding::IsDestination(int x, int y, const TTilePos& dest)
{
	if (x == dest.x && y == dest.y)
		return true;
	return false;
}

// 시작위치에서부터 목적지까지의 경로를 추적한다.
void CPathFinding::TracePath(const vector<vector<Cell> >& cellDetails, const TTilePos& dest)
{
	int x = dest.x;
	int y = dest.y;

	m_stkPath.clear();

	while (!(cellDetails[y][x].parentX == x && cellDetails[y][x].parentY == y)) {
		m_stkPath.push_front(TTilePos{ x, y });

		int tempX = cellDetails[y][x].parentX;
		int tempY = cellDetails[y][x].parentY;
		x = tempX;
		y = tempY;
	}

	//m_stkPath.push(TTilePos{ x, y });
	//while (!m_stkPath.empty()) {
	//	TTilePos p = m_stkPath.top();
	//	m_stkPath.pop();
	//	// TODO : 다른 방법 몰색
	//	//cout << "->(" << p.first << "," << p.second << ")" << "\n";
	//	Draw(p.x, p.y);
	//}
}

// AStar 알고리즘을 이용한 길찾기
// return : 길을 찾으면 true, 못찾으면 false 리턴
bool CPathFinding::PathFind(const TTilePos& start, const TTilePos& dest)
{
	m_bFoundDestination = false;
	if (false == IsValid(start.x, start.y))
		return false;
	if (false == IsValid(dest.x, dest.y))
		return false;

	if (false == IsUnBlocked(start.x, start.y))
		return false;
	if (false == IsUnBlocked(dest.x, dest.y))
		return false;

	if (IsDestination(start.x, start.y, dest))
		return false;

	int iRow = m_pTileMap->GetRow();
	int iCol = m_pTileMap->GetCol();
	vector<vector<bool> > closedList(iRow, vector<bool>(iCol, false));
	vector<vector<Cell> > cellDetails(iRow, vector<Cell>(iCol));

	// init cells
	for (int i = 0; i < iRow; ++i) {
		for (int j = 0; j < iCol; ++j) {
			cellDetails[i][j].f = (std::numeric_limits<float>::max)();
			cellDetails[i][j].g = (std::numeric_limits<float>::max)();
			cellDetails[i][j].h = (std::numeric_limits<float>::max)();
			cellDetails[i][j].parentX = -1;
			cellDetails[i][j].parentY = -1;
		}
	}

	// 시작 노드를 초기화 한다.
	int iX = start.x, iY = start.y; // i = y, j = x
	cellDetails[iY][iX].f = 0.f;
	cellDetails[iY][iX].g = 0.f;
	cellDetails[iY][iX].h = 0.f;
	cellDetails[iY][iX].parentX = iX;
	cellDetails[iY][iX].parentY = iY;

	// open list를 만든다.
	set<pPair> openList;

	// 시작 지점의 f를 0으로 둔다.

	openList.insert(std::make_pair(0.f, make_pair(iX, iY)));


	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin()); // openList에 있는 vertex를 삭제.

		iX = p.second.first; // x
		iY = p.second.second;  // y
		closedList[iY][iX] = true;

		// 8방향의 successor를 생성한다.
		for (int d = 0; d < 8; ++d) {
			int x = iX + m_iDirX[d];
			int y = iY + m_iDirY[d];

			if (false == IsValid(x, y))
				continue;

			if (true == IsDestination(x, y, dest)) {
				// 목적지 Cell의 부모를 설정한다.
				cellDetails[y][x].parentX = iX;
				cellDetails[y][x].parentY = iY;
				m_bFoundDestination = true;
				TracePath(cellDetails, dest);
				return true;
			}
			else if (false == closedList[y][x] && true == IsUnBlocked(x, y)) {
				float gNew = cellDetails[iY][iX].g + 1.0f;
				float hNew = (float)GetDistance(x, y, dest.x, dest.y);
				float fNew = gNew + hNew;

				// openList가 아니면 openList에 추가한다.
				// 이미 openList면 
				if (cellDetails[y][x].f == (std::numeric_limits<float>::max)() ||
					cellDetails[y][x].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(x, y)));
					cellDetails[y][x].f = fNew;
					cellDetails[y][x].g = gNew;
					cellDetails[y][x].h = hNew;
					cellDetails[y][x].parentX = iX;
					cellDetails[y][x].parentY = iY;
				}
			}
		}
	}

	return false;
}

void CPathFinding::Draw(int curX, int curY)
{
}

void CPathFinding::AddObstacleTile(E_TileType _eTileType)
{
	m_setObstacleTile.insert(_eTileType);
}

void CPathFinding::DeleteObstacleTile(E_TileType _eTileType)
{
	m_setObstacleTile.erase(_eTileType);
}

const vector<E_TileType>& CPathFinding::GetObstacleTiles()
{
	vector<E_TileType> vecObstacleTiles;
	set<E_TileType>::iterator iter = m_setObstacleTile.begin();
	for (; iter != m_setObstacleTile.end(); ++iter) {
		vecObstacleTiles.push_back(*iter);
	}
	return vecObstacleTiles;
}

bool CPathFinding::IsArrivedDestination()
{
	if (m_bFoundDestination == false)
		return false;

	list<TTilePos>& path = GetPath();
	if (path.size() == 0)
		return true;
	return false;
}