#pragma once
#include <set>
#include <vector>
using namespace std;

struct Grid
{
	set<Coord> coords_;
};

/// 将maxX * maxY的地图划分为宽度为width的网格
class GridMgr
{
public:
	GridMgr(int maxX, int maxY, int width);
	~GridMgr();

	bool AddCoord(Coord coord);
	bool DelCoord(Coord coord);
	Coord SearchNearby(Coord coord);

private:
	int maxX_;
	int maxY_;
	int width_;
	int xLen_;
	int yLen_;
	
	Grid** grids_;
};

/// @brief 暴力搜索，用于比较
class SimpleWorld
{
public:
	SimpleWorld();
	~SimpleWorld();

	bool AddCoord(Coord coord);
	bool DelCoord(Coord coord);
	Coord SearchNearby(Coord coord);

private:
	set<Coord> coords_;
};
