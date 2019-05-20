// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#ifndef PCH_H
#define PCH_H

#define _CRTDBG_MAP_ALLOC

#include <cmath>
#include <ctime>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <random>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


inline std::string GetTimeStamp(time_t tCur = 0)
{
	if (!tCur)
		tCur = std::time(nullptr);

	struct tm t;
	localtime_s(&t, &tCur);

	using std::setw;
	using std::setfill;

	std::ostringstream oss;
	oss << t.tm_year - 100
		<< "-" << setw(2) << setfill('0') << t.tm_mon + 1
		<< "-" << setw(2) << setfill('0') << t.tm_mday
		<< " " << setw(2) << setfill('0') << t.tm_hour
		<< ":" << setw(2) << setfill('0') << t.tm_min
		<< ":" << setw(2) << setfill('0') << t.tm_sec;
	return oss.str();
}

struct EasyLogger
{
	~EasyLogger()
	{
		std::cout << GetTimeStamp() << " " << osslog_.str() << std::endl;

		std::ofstream ofs("log.txt", std::ios::app);
		if (ofs) {
			ofs << GetTimeStamp() << " " << osslog_.str() << std::endl;
			ofs.close();
		}
	}

	std::ostringstream& Stream() { return osslog_; }

	std::ostringstream osslog_;
};
#define EASYLOG EasyLogger().Stream()

class RandomEngine
{
public:
	RandomEngine(uint32_t a, uint32_t b) : gen_(seed_ + clock()), dist_(a, b > a ? b - 1 : a) { seed_ += clock(); }
	uint32_t GetRand() { return dist_(gen_); }

private:
	static uint32_t seed_;
	std::mt19937 gen_;
	std::uniform_int_distribution<uint32_t> dist_;
};

static const uint32_t MAX_X = 1501;
static const uint32_t MAX_Y = 1501;
static const uint32_t ZONE_WIDTH = 20;

enum Orientation
{
	None,
	X,
	Y,
};

struct Coord
{
	Coord() : x_(0), y_(0) {}
	Coord(int x, int y) : x_(x), y_(y) {}
	explicit operator bool() { return x_ && x_ < MAX_X && y_ && y_ < MAX_Y; }
	inline bool operator<(const Coord& rhs) const
	{
		return x_ == rhs.x_ ? y_ < rhs.y_ : x_ < rhs.x_;
	}
	inline bool operator==(const Coord& rhs) const
	{
		return x_ == rhs.x_ && y_ == rhs.y_;
	}
	void Reset(int x, int y) { x_ = x; y_ = y; }

	static int DistSq(Coord a, Coord b) { int distX = a.x_ - b.x_; int distY = a.y_ - b.y_; return distX*distX+distY*distY; }
	static float Dist(Coord a, Coord b) { return sqrt((float)DistSq(a, b)); }

	int x_;
	int y_;
};
inline std::ostream& operator<<(std::ostream& os, const Coord& coord)
{
	os << "[" << coord.x_ << ", " << coord.y_ << "]";
	return os;
}

#endif //PCH_H
