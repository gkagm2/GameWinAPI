#include "stdafx.h"
#include "CGTA_AStar.h"
#include <numeric>

CGTA_AStar::CGTA_AStar()
{
}

CGTA_AStar::~CGTA_AStar()
{
}

void CGTA_AStar::Init()
{
}

int CGTA_AStar::GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool CGTA_AStar::IsValid(int x, int y) {
	if (x < 0 || y < 0 || x >= g_maxX || y >= g_maxY)
		return false;
	return true;
}

bool CGTA_AStar::IsUnBlocked(int x, int y)
{
	if (grid[y][x] == (int)Type::Wall)
		return false;
	return true;
}

bool CGTA_AStar::IsDestination(int x, int y, Pair dest)
{
	if (x == dest.second && y == dest.first)
		return true;
	return false;
}

// 시작위치에서부터 목적지까지의 경로를 추적한다.
void CGTA_AStar::TracePath(Cell cellDetails[g_maxY][g_maxX], Pair dest)
{
	int x = dest.second;
	int y = dest.first;

	stack<Pair> path;

	while (!(cellDetails[y][x].parentX == x && cellDetails[y][x].parentY == y)) {
		path.push(Pair{ y, x });

		int tempX = cellDetails[y][x].parentX;
		int tempY = cellDetails[y][x].parentY;
		x = tempX;
		y = tempY;
	}

	path.push(Pair{ y, x });
	while (!path.empty()) {
		Pair p = path.top();
		path.pop();
		// TODO : 다른 방법 몰색
		//cout << "->(" << p.first << "," << p.second << ")" << "\n";
		Draw(p.second, p.first);
		Sleep(200);
	}
}

void CGTA_AStar::AStar(Pair start, Pair dest)
{
	if (false == IsValid(start.first, start.second))
		return;
	if (false == IsValid(dest.first, dest.second))
		return;

	if (false == IsUnBlocked(start.first, start.second) ||
		false == IsUnBlocked(dest.first, dest.second))
		return;

	if (IsDestination(start.first, start.second, dest))
		return;

	bool closedList[g_maxY][g_maxX];
	memset(closedList, false, sizeof(closedList));

	Cell cellDetails[g_maxY][g_maxX];

	// init cells
	for (int i = 0; i < g_maxY; ++i) {
		for (int j = 0; j < g_maxX; ++j) {
			cellDetails[i][j].f = (std::numeric_limits<float>::max)();
			cellDetails[i][j].g = (std::numeric_limits<float>::max)();
			cellDetails[i][j].h = (std::numeric_limits<float>::max)();
			cellDetails[i][j].parentX = -1;
			cellDetails[i][j].parentY = -1;
		}
	}

	// 시작 노드를 초기화 한다.
	int i = start.first, j = start.second; // i = y, j = x
	cellDetails[i][j].f = 0.f;
	cellDetails[i][j].g = 0.f;
	cellDetails[i][j].h = 0.f;
	cellDetails[i][j].parentX = j;
	cellDetails[i][j].parentY = i;

	// open list를 만든다.
	set<pPair> openList;

	// 시작 지점의 f를 0으로 둔다.
	openList.insert(pPair{ 0.f, Pair{i, j}});

	bool foundDest = false;

	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin()); // openList에 있는 vertex를 삭제.

		i = p.second.first;  // y
		j = p.second.second; // x
		closedList[i][j] = true;

		// 8방향의 successor를 생성한다.
		for (int d = 0; d < 8; ++d) {
			int y = i + dirY[d];
			int x = j + dirX[d];

			if (false == IsValid(x, y))
				continue;

			if (true == IsDestination(x, y, dest)) {
				// 목적지 Cell의 부모를 설정한다.
				cellDetails[y][x].parentX = j;
				cellDetails[y][x].parentY = i;
				// TODO : Found Destination
				TracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (false == closedList[y][x] && true == IsUnBlocked(x, y)) {
				float gNew = cellDetails[i][j].g + 1.0f;
				float hNew = GetDistance(x, y, dest.first, dest.second);
				float fNew = gNew + hNew;

				// openList가 아니면 openList에 추가한다.
				// 이미 openList면 
				if (cellDetails[y][x].f == (std::numeric_limits<float>::max)() ||
					cellDetails[y][x].f > fNew) {
					openList.insert(pPair{ fNew, Pair{y, x} });

					cellDetails[y][x].f = fNew;
					cellDetails[y][x].g = gNew;
					cellDetails[y][x].h = hNew;
					cellDetails[y][x].parentX = j;
					cellDetails[y][x].parentY = i;
				}
			}
		}
	}

	if (false == foundDest) {
		// Can not found destination
	}
}

void CGTA_AStar::Draw(int curX, int curY)
{
}
