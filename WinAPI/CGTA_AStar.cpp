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

// ������ġ�������� ������������ ��θ� �����Ѵ�.
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
		// TODO : �ٸ� ��� ����
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

	// ���� ��带 �ʱ�ȭ �Ѵ�.
	int i = start.first, j = start.second; // i = y, j = x
	cellDetails[i][j].f = 0.f;
	cellDetails[i][j].g = 0.f;
	cellDetails[i][j].h = 0.f;
	cellDetails[i][j].parentX = j;
	cellDetails[i][j].parentY = i;

	// open list�� �����.
	set<pPair> openList;

	// ���� ������ f�� 0���� �д�.
	openList.insert(pPair{ 0.f, Pair{i, j}});

	bool foundDest = false;

	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin()); // openList�� �ִ� vertex�� ����.

		i = p.second.first;  // y
		j = p.second.second; // x
		closedList[i][j] = true;

		// 8������ successor�� �����Ѵ�.
		for (int d = 0; d < 8; ++d) {
			int y = i + dirY[d];
			int x = j + dirX[d];

			if (false == IsValid(x, y))
				continue;

			if (true == IsDestination(x, y, dest)) {
				// ������ Cell�� �θ� �����Ѵ�.
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

				// openList�� �ƴϸ� openList�� �߰��Ѵ�.
				// �̹� openList�� 
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
