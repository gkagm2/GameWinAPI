#pragma once

struct Pair {
	int first;
	int second;
};

struct pPair {
	double first;
	Pair second;
};

struct Cell {
	int parentX, parentY;
	float  f, g, h;
};

class CTileMap;
class CGTA_AStar
{
private:
	const static int g_maxX = 18; // TODO : 바꾸기
	const static int g_maxY = 18;
	CTileMap* m_pTileMap;

	int grid[g_maxY][g_maxX]; // TODO : tilemap을 가져와서 적용



	enum class Type {
		Path,
		Wall,
		Start,
		Dest
	};

	// ↖ ↑ ↗ ← → ↙ ↓ ↘ (index 순서에 따른 뱡향)
	int dirX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dirY[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

public:
	void SetTileMap(CTileMap* _pTileMap) { m_pTileMap = _pTileMap; }
	void Init();
	 
	int GetDistance(int x1, int y1, int x2, int y2);
	bool IsValid(int x, int y);
	bool IsUnBlocked(int x, int y);
	bool IsDestination(int x, int y, Pair dest);
	void TracePath(Cell cellDetails[g_maxY][g_maxX], Pair dest);
	void AStar(Pair start, Pair dest);

	void Draw(int curX, int curY);

public:
	CGTA_AStar();
	virtual ~CGTA_AStar() final;
};