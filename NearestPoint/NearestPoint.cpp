// NearestPoint.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "GridNear.h"
#include <cassert>

struct Node
{
	Node() : orient_(Orientation::None), left_(nullptr), right_(nullptr) {}

	Orientation orient_;
	Node* left_;
	Node* right_;
};

struct TimeNote
{
	TimeNote() :total_(0), minTime_(10000), maxTime_(0), amount_(0) {}

	void Record(clock_t useTime)
	{
		++amount_;
		total_ += useTime;
		minTime_ = min(useTime, minTime_);
		maxTime_ = max(useTime, maxTime_);
	}

	void Statistic()
	{
		EASYLOG << "Time note: " << minTime_ << " " << maxTime_ << " " << total_ / amount_;
	}

	clock_t total_;
	clock_t minTime_;
	clock_t maxTime_;
	int amount_;
};

class GridTestCase
{
public:
	GridTestCase() : gm_(1500, 1500, 10) {}

	bool AddCoord(Coord coord)
	{
		if (sw_.AddCoord(coord)){
			gm_.AddCoord(coord);
			return true;
		}
		return false;
	}

	void SearchNearby(Coord coord)
	{
		clock_t cur = clock();
		Coord nearby = gm_.SearchNearby(coord);
		clock_t useTime = clock() - cur;
		gmTime_.Record(useTime);
		EASYLOG << "GridMgr search nearby: [" << setw(5) << coord.x_ << setw(5) << coord.y_ << "] [" << setw(5) << nearby.x_ << setw(5) << nearby.y_
			<< "] " << setw(6) << (int)Coord::Dist(coord, nearby) << " " << setw(3) << useTime;

		clock_t cur2 = clock();
		Coord nearby2 = sw_.SearchNearby(coord);
		clock_t useTime2 = clock() - cur2;
		swTime_.Record(useTime2);
		
		if (Coord::DistSq(nearby, coord) != Coord::DistSq(nearby2, coord))
			EASYLOG << "Error: search nearby invalid: " << coord << " " << nearby << " " << nearby2;
		if (useTime > useTime2)
			EASYLOG << "Error: search nearby time more: " << coord << " " << useTime << " " << useTime2;
	}

	void Finish()
	{
		EASYLOG << "Grid mgr time: ";
		gmTime_.Statistic();
		EASYLOG << "Simple world time: ";
		swTime_.Statistic();
	}

private:
	GridMgr gm_;
	SimpleWorld sw_;
	TimeNote gmTime_;
	TimeNote swTime_;
};

/// @brief 测试网格遍历顺序的正确性
void TestGridMgr1()
{
	EASYLOG << "Start: " << __FUNCTION__;

	GridMgr gm(50, 50, 10);

	vector<Coord> coords;
	//coords.emplace_back(10, 10);
	//coords.emplace_back(25, 25);
	//coords.emplace_back(1, 25);
	//coords.emplace_back(31, 1);
	//coords.emplace_back(1, 41);
	//coords.emplace_back(32, 33);
	coords.emplace_back(41, 22);

	for (auto coord : coords) {
		EASYLOG << "Search nearby: " << coord;
		gm.SearchNearby(coord);
		EASYLOG << "=======================" << endl;
	}
}

/// @brief 已存在点和搜索点都是随机分配
void TestGriMgr2()
{
	EASYLOG << "Start: " << __FUNCTION__;

	GridTestCase tc;

	RandomEngine rd(1, 1501);
	int count = 0;
	while (count < 100000) {
		Coord coord(rd.GetRand(), rd.GetRand());
		if (tc.AddCoord(coord))
			++count;
	}

	for (int i = 0; i < 10000; ++i) {
		Coord coord(rd.GetRand(), rd.GetRand());
		tc.SearchNearby(coord);
	}
	tc.Finish();

	EASYLOG << "============================";
}

/// @brief 已存在点占满左上角两条边，宽度为width，右下角搜索最近点。
void TestGriMgr3(int width)
{
	EASYLOG << "Start: " << __FUNCTION__ << " " << width;

	GridTestCase tc;

	for (int i = 1; i <= 1500; ++i) {
		for (int j = 1; j <= width; ++j) {
			Coord coord(i, j);
			tc.AddCoord(coord);
		}
	}
	for (int i = 1; i <= width; ++i) {
		for (int j = 1; j <= 1500; ++j) {
			Coord coord(i, j);
			tc.AddCoord(coord);
		}
	}

	RandomEngine rd(1400, 1501);
	for (int i = 0; i < 10000; ++i) {
		Coord coord(rd.GetRand(), rd.GetRand());
		tc.SearchNearby(coord);
	}
	tc.Finish();

	EASYLOG << "============================";
}

/// @brief 10w个已存在点分布在左上角500*500，搜索点随机分布
void TestGriMgr4()
{
	EASYLOG << "Start: " << __FUNCTION__;

	GridTestCase tc;

	RandomEngine rd(1, 501);
	int count = 0;
	while (count < 100000) {
		Coord coord(rd.GetRand(), rd.GetRand());
		if (tc.AddCoord(coord))
			++count;
	}

	RandomEngine rd2(1, 1501);
	for (int i = 0; i < 10000; ++i) {
		Coord coord(rd2.GetRand(), rd2.GetRand());
		tc.SearchNearby(coord);
	}
}

/// @brief 已存在点占满4条边，宽度为width
void TestGriMgr5(int width)
{
	EASYLOG << "Start: " << __FUNCTION__ << " " << width;

	GridTestCase tc;

	for (int i = 1; i <= 1500; ++i) {
		for (int j = 1; j <= width; ++j) {
			Coord coord(i, j);
			tc.AddCoord(coord);
		}
	}
	for (int i = 1; i <= width; ++i) {
		for (int j = 1; j <= 1500; ++j) {
			Coord coord(i, j);
			tc.AddCoord(coord);
		}
	}
	for (int i = 1500-width+1; i <= 1500; ++i) {
		for (int j = 1; j <= 1500; ++j) {
			Coord coord(i, j);
			tc.AddCoord(coord);
		}
	}
	for (int i = 1; i <= 1500; ++i) {
		for (int j = 1500-width+1; j <= 1500; ++j) {
			Coord coord(i, j);
			tc.AddCoord(coord);
		}
	}

	Coord coord(750, 750);
	tc.SearchNearby(coord);

	RandomEngine rd(300, 401);
	for (int i = 0; i < 10000; ++i) {
		Coord coord(rd.GetRand(), rd.GetRand());
		tc.SearchNearby(coord);
	}

	tc.Finish();

	EASYLOG << "============================";
}

void TestGriMgrX()
{
	GridMgr gm(1500, 1500, 10);

	RandomEngine rd(1, 501);
	int count = 0;
	while (count < 100000) {
		Coord coord(rd.GetRand(), rd.GetRand());
		if (gm.AddCoord(coord))
			++count;
	}

	clock_t total = 0;
	clock_t minTime = 1000;
	clock_t maxTime = 0;
	RandomEngine rd2(1, 1501);
	int testCount = 10000;
	for (int i = 0; i < testCount; ++i) {
		clock_t cur = clock();
		Coord coord(rd2.GetRand(), rd2.GetRand());
		Coord nearby = gm.SearchNearby(coord);
		clock_t useTime = clock() - cur;
		minTime = min(useTime, minTime);
		maxTime = max(useTime, maxTime);
		total += useTime;
		EASYLOG << "Search nearby: [" << setw(5) << coord.x_ << setw(5) << coord.y_ << "] [" << setw(5) << nearby.x_ << setw(5) << nearby.y_
			<< "] " << setw(6) << (int)Coord::Dist(coord, nearby) << " " << setw(3) << useTime;
	}
	EASYLOG << "Average use time: " << total / testCount;
	EASYLOG << "Min use time: " << minTime;
	EASYLOG << "Max use time: " << maxTime;
}

int main()
{
	//TestGridMgr1();

	TestGriMgr2();

	TestGriMgr3(8);
	TestGriMgr3(9);
	TestGriMgr3(10);
	TestGriMgr3(11);
	TestGriMgr3(12);

	TestGriMgr4();

	TestGriMgr5(8);
	TestGriMgr5(9);
	TestGriMgr5(10);
	TestGriMgr5(11);
	TestGriMgr5(12);

	system("pause");

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}
