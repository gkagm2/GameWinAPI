#pragma once
#include "CComponent.h"

struct Cell {
	int parentX, parentY;
	float  f, g, h; // f:가장 낮은 노드 , g: , h: 휴리스틱
};

class CTileMap;
class CObject;
class CPathFinding : public CComponent
{
private:
	CTileMap* m_pTileMap;
	list<TTilePos> m_stkPath;
	set<E_TileType> m_setObstacleTile;

	bool m_bFoundDestination;

	// ↖ ↑ ↗ ← → ↙ ↓ ↘ (index 순서에 따른 뱡향)
	static const  int m_iDirX[8];
	static const int m_iDirY[8];

	typedef std::pair<int, int> Pair;
	typedef std::pair<float, std::pair<int, int> > pPair;

public:
	void Init();

protected:
	float GetDistance(int x1, int y1, int x2, int y2);
	bool IsUnBlocked(int x, int y);
	bool IsDestination(int x, int y, const TTilePos& dest);
	void TracePath(const vector<vector<Cell> >& cellDetails, const TTilePos& dest);

public:
	bool IsValid(int x, int y);
	bool PathFind(const TTilePos& start, const TTilePos& dest);
	void Draw(int curX, int curY);

	list<TTilePos>& GetPath() { return m_stkPath; }

	// 장애물로 취급할 타일 타입 설정
	void AddObstacleTile(E_TileType _eTileType);
	void DeleteObstacleTile(E_TileType _eTileType);
	const set<E_TileType>& GetObstacleTiles() { return m_setObstacleTile; }

	bool IsArrivedDestination();
	bool IsFoundDestination() {	return m_bFoundDestination;	}
	bool IsObstacle(int _iCol, int _iRow);
public:
	CLONE(CPathFinding);
	CPathFinding(const CPathFinding& _origin);
public:
	CPathFinding();
	virtual ~CPathFinding();
};