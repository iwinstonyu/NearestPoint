#include "pch.h"
#include "GridNear.h"
#include <algorithm>

GridMgr::GridMgr(int maxX, int maxY, int width)
	: maxX_(maxX)
	, maxY_(maxY)
	, width_(width)
{
	xLen_ = maxX / width + (maxX % width ? 1 : 0);
	yLen_ = maxY / width + (maxY % width ? 1 : 0);

	grids_ = DBG_NEW Grid*[xLen_];
	for (int i = 0; i < xLen_; ++i) {
		grids_[i] = DBG_NEW Grid[yLen_];
	}
}

GridMgr::~GridMgr()
{
	for (int i = 0; i < xLen_; ++i) {
		delete[] grids_[i];
	}
	delete[] grids_;
}

bool GridMgr::AddCoord(Coord coord)
{
	return grids_[(coord.x_ - 1) / width_][(coord.y_ - 1) / width_].coords_.insert(coord).second;
}

bool GridMgr::DelCoord(Coord coord)
{
	return grids_[(coord.x_ - 1) / width_][(coord.y_ - 1) / width_].coords_.erase(coord) ? true : false;
}

Coord GridMgr::SearchNearby(Coord coord)
{
	int x = (coord.x_ - 1) / width_;
	int y = (coord.y_ - 1) / width_;

	int maxgox = max(xLen_ - x - 1, x);
	int maxgoy = max(yLen_ - y - 1, y);
	int maxspread = max(maxgox, maxgoy);
	int spread = 0;

	int distsq = INT_MAX;
	Coord nearCoord;
	do {
		//EASYLOG << "Spread: " << spread;

		// 已经找到一个点，且扩散的范围已经超出这两个点所画的圆
		if (nearCoord && spread) {
			if ((spread - 1)*width_*(spread-1)*width_ > distsq)
				break;
		}

		int lx = x - spread;
		int uy = y - spread;
		int rx = x + spread;
		int dy = y + spread;
		int cx = lx;
		int cy = uy;

		int direction = 0;
		do {
			// 第一次的点可能是无效的点
			if (cx >= 0 && cx <= rx && cx < (int)xLen_ && cy >= 0 && cy <= dy && cy < (int)yLen_ 
				&& !grids_[cx][cy].coords_.empty()) {
				//EASYLOG << "Check grid: " << cx << " " << cy;

				bool check = true;
				if (nearCoord) {
					int gridDistSq = INT_MAX;

					int glx = cx * 10 + 1;
					int grx = glx + 9;
					int guy = cy * 10 + 1;
					int gdy = guy + 9;

					int minDistX = min(abs(glx - coord.x_), abs(grx - coord.x_));
					int minDistY = min(abs(guy - coord.y_), abs(gdy - coord.y_));

					if (coord.x_ >= glx && coord.x_ <= grx)
						gridDistSq = minDistY*minDistY;
					else if (coord.y_ >= guy && coord.y_ <= gdy)
						gridDistSq = minDistX*minDistX;
					else
						gridDistSq = minDistX * minDistX + minDistY * minDistY;

					if (gridDistSq > distsq)
						check = false;
				}

				if (check) {
					for (auto tempCoord : grids_[cx][cy].coords_) {
						if (!nearCoord) {
							distsq = Coord::DistSq(tempCoord, coord);
							nearCoord = tempCoord;
						}
						else {
							int tempDistSq = Coord::DistSq(tempCoord, coord);
							if (tempDistSq < distsq) {
								distsq = tempDistSq;
								nearCoord = tempCoord;
							}
						}
					}
				}
			}

			bool finish = false;
			while (true) {
				switch (direction)
				{
				case 0:
					if (cx >= rx || cx > (int)xLen_ || uy < 0) {
						cx = rx;
						++direction;
						break;
					}
					if (cx < 0)
						cx = 0;
					else
						++cx;
					finish = true;
					break;
				case 1:
					if (cy >= dy || cy > (int)yLen_ || rx < 0) {
						cy = dy;
						++direction;
						break;
					}
					if (cy < 0)
						cy = 0;
					else
						++cy;
					finish = true;
					break;
				case 2:
					if (cx <= lx || cx <= 0 || dy < 0) {
						cx = lx;
						++direction;
						break;
					}
					if (cx > rx)
						cx = rx;
					else
						--cx;
					finish = true;
					break;
				case 3:
					if (cy <= uy+1 || cy <= 0 || lx < 0) {
						cy = uy;
						++direction;
						break;
					}
					if (cy > dy)
						cy = dy;
					else
						--cy;
					finish = true;
					break;
				default:
					finish = true;
					break;
				}

				if (finish)
					break;
			}
		} while (direction <= 3);
	} while (++spread <= maxspread);

	return nearCoord;
}

SimpleWorld::SimpleWorld()
{

}

SimpleWorld::~SimpleWorld()
{

}

bool SimpleWorld::AddCoord(Coord coord)
{
	return coords_.insert(coord).second;
}

bool SimpleWorld::DelCoord(Coord coord)
{
	return coords_.erase(coord) ? true : false;
}

Coord SimpleWorld::SearchNearby(Coord coord)
{
	Coord nearCoord;
	int distSq = INT_MAX;

	for (auto tempCoord : coords_) {
		if (!nearCoord) {
			distSq = Coord::DistSq(tempCoord, coord);
			nearCoord = tempCoord;
		}
		else {
			int tempDistSq = Coord::DistSq(tempCoord, coord);
			if (tempDistSq < distSq) {
				distSq = tempDistSq;
				nearCoord = tempCoord;
			}
		}
	}

	return nearCoord;
}
