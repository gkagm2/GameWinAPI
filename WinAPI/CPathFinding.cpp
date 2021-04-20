#include "stdafx.h"
#include "CPathFinding.h"
#include "CTileMap.h"
#include "CTile.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include <numeric>

CPathFinding::CPathFinding() :
	m_pTileMap(nullptr),
	m_pvecTiles(nullptr),
	m_iDirX{ 1, 0, -1, 0 },
	m_iDirY{ 0, 1, 0, -1 }
{
	vector<CObject*>& vecTile = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
	m_pvecTiles = &vecTile;
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
	if (x < 0 || y < 0 || x >= (int)m_pTileMap->GetCol() || y >= (int)m_pTileMap->GetRow())
		return false;
	return true;
}

bool CPathFinding::IsUnBlocked(int x, int y)
{
	vector<CObject*>& vecTile = CSceneManager::GetInstance()->GetCurScene()->GetObjects(E_GroupType::TILE);
	int idx = y * (int)(m_pTileMap->GetCol()) + x;
	CTile* pTile = (CTile*)vecTile[idx];

	if (E_TileType::Wall == pTile->GetTileType() || E_TileType::Water == pTile->GetTileType())
		return false;
	return true;
}

bool CPathFinding::IsDestination(int x, int y, const TTilePos& dest)
{
	if (x == dest.x && y == dest.y)
		return true;
	return false;
}

// ������ġ�������� ������������ ��θ� �����Ѵ�.
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
	//	// TODO : �ٸ� ��� ����
	//	//cout << "->(" << p.first << "," << p.second << ")" << "\n";
	//	Draw(p.x, p.y);
	//}
}

// AStar �˰����� �̿��� ��ã��
// return : ���� ã���� true, ��ã���� false ����
bool CPathFinding::PathFind(const TTilePos& start, const TTilePos& dest)
{
	if (false == IsValid(start.y, start.x))
		return false;
	if (false == IsValid(dest.y, dest.x))
		return false;

	if (false == IsUnBlocked(start.y, start.x) ||
		false == IsUnBlocked(dest.y, dest.x))
		return false;

	if (IsDestination(start.y, start.x, dest))
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

	// ���� ��带 �ʱ�ȭ �Ѵ�.
	int i = start.y, j = start.x; // i = y, j = x
	cellDetails[i][j].f = 0.f;
	cellDetails[i][j].g = 0.f;
	cellDetails[i][j].h = 0.f;
	cellDetails[i][j].parentX = j;
	cellDetails[i][j].parentY = i;

	// open list�� �����.
	set<pPair> openList;

	// ���� ������ f�� 0���� �д�.

	openList.insert(std::make_pair(0.f, make_pair(i, j)));

	bool foundDest = false;

	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin()); // openList�� �ִ� vertex�� ����.

		i = p.second.first;  // y
		j = p.second.second; // x
		closedList[i][j] = true;

		// 4������ successor�� �����Ѵ�.
		for (int d = 0; d < 4; ++d) {
			int x = j + m_iDirX[d];
			int y = i + m_iDirY[d];

			if (false == IsValid(x, y))
				continue;

			if (true == IsDestination(x, y, dest)) {
				// ������ Cell�� �θ� �����Ѵ�.
				cellDetails[y][x].parentX = j;
				cellDetails[y][x].parentY = i;
				// TODO : Found Destination
				TracePath(cellDetails, dest);
				return true;
			}
			else if (false == closedList[y][x] && true == IsUnBlocked(x, y)) {
				float gNew = cellDetails[i][j].g + 1.0f;
				float hNew = (float)GetDistance(x, y, dest.x, dest.y);
				float fNew = gNew + hNew;

				// openList�� �ƴϸ� openList�� �߰��Ѵ�.
				// �̹� openList�� 
				if (cellDetails[y][x].f == (std::numeric_limits<float>::max)() ||
					cellDetails[y][x].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(y, x)));
					cellDetails[y][x].f = fNew;
					cellDetails[y][x].g = gNew;
					cellDetails[y][x].h = hNew;
					cellDetails[y][x].parentX = j;
					cellDetails[y][x].parentY = i;
				}
			}
		}
	}

	return false;
}

void CPathFinding::Draw(int curX, int curY)
{
}