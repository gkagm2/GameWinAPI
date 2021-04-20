#pragma once
struct TTilePos {
	int x;
	int y;
	TTilePos(int _x, int _y) : x(_x), y(_y) {}
	TTilePos() : x(0), y(0) {}

	bool operator<(const TTilePos& _rhs) {
		if (_rhs.x > x && _rhs.y > y)
			return true;
		return false;
	}
};

struct Cell {
	int parentX, parentY;
	float  f, g, h; // f:가장 낮은 노드 , g: , h: 휴리스틱
};

class CTileMap;
class CObject;
class CPathFinding
{
private:
	CTileMap* m_pTileMap;
	vector<CObject*>* m_pvecTiles;
	list<TTilePos> m_stkPath;

	// ↖ ↑ ↗ ← → ↙ ↓ ↘ (index 순서에 따른 뱡향)
	int m_iDirX[8];
	int m_iDirY[8];

	typedef std::pair<int, int> Pair;
	typedef std::pair<float, std::pair<int, int> > pPair;

public:
	void Init();

protected:
	float GetDistance(int x1, int y1, int x2, int y2);
	bool IsValid(int x, int y);
	bool IsUnBlocked(int x, int y);
	bool IsDestination(int x, int y, const TTilePos& dest);
	void TracePath(const vector<vector<Cell> >& cellDetails, const TTilePos& dest);

public:
	bool PathFind(const TTilePos& start, const TTilePos& dest);
	void Draw(int curX, int curY);

	list<TTilePos>& GetPath() { return m_stkPath; }
	
public:
	CPathFinding();
	virtual ~CPathFinding();
};