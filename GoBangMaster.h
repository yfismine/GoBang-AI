#ifndef _GOBANGH_
#define _GOBANGH_
#pragma once
#include<vector>
#include<set>
#include<stack>
#include<memory>
#include<unordered_map>
#include<functional>
#include<queue>
#include<ctime>
#include<tuple>
#define HUMAN 'O'
#define COMPTER 'X'
#define COMPTER_WIN 10000000
#define COMPTER_LOSS -10000000
#define FORBIDDEN 5
#define IMMKILL   6
#define DRAW 0
using namespace std;
struct point                       //存放棋子坐标
{
	int x = 0;
	int y = 0;
	point& operator=(const point& p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}
	friend bool operator==(const point &p1, const point& p2);
	friend bool operator<(const point &p1, const point&p2);
	point() = default;
	point(int row, int col)
		:x(row), y(col) {};
};
size_t hasher(const point& p);
class GoBang
{
public:
	GoBang();
	~GoBang();
	void addPiece(int x, int y);        //玩家可调用的落子函数
	bool regretChess();
private:
	using dirmark = unordered_map<point, vector<bool>,decltype(hasher)*>;
	struct direction                //存放方向
	{
		int dx;
		int dy;
	};
	struct sum                     //棋子类型统计
	{
		int win5 = 0, alive4 = 0, dalive4 = 0, die4 = 0, alive3 = 0,
			dalive3 = 0, die3 = 0, alive2 = 0, dalive2 = 0, die2 = 0,
			alive1 = 0, dalive1 = 0, die1 = 0;
		sum operator +=(sum &rhs)
		{
			win5 += rhs.win5;
			alive4 += rhs.alive4;
			dalive4 += rhs.dalive4;
			die4 += rhs.die4;
			alive3 += rhs.alive3;
			dalive3 += rhs.dalive3;
			die3 += rhs.die3;
			alive2 += rhs.alive2;
			dalive2 += rhs.dalive2;
			die2 += rhs.die2;
			alive1 += rhs.alive1;
			dalive1 += rhs.dalive1;
			die1 += rhs.die1;
			return *this;
		}
	};
	struct waitPoint              //待选坐标点
	{
		int value;
		point p;
		waitPoint() = default;
		waitPoint(int v, point p1)
			:value(v), p(p1) {};
	};
	struct cmp                   //比较函数用以优先队列的比较
	{
		bool operator()(const waitPoint &p1, const waitPoint &p2)
		{
			return p1.value < p2.value;
		}
	};
	int number;
	int maxDeep;
	long currentValue;
	bool extension;
	clock_t beginTime;
	clock_t endTime;
	stack<pair<point,long>> points;
	unsigned long long zobristKey;
	vector<unsigned long long> randomArray[15][15];
	unordered_map<unsigned long long,long> zobristMap;
	char chessBoard[15][15];      //棋盘
	int prieceNumber = 0;         //棋盘中的棋子数
	inline bool fullBoard();    //判断是否满盘
	inline bool isEmpty(const int x, const int y);  //判断一点是否为空位置
	inline void place(int x, int y, char name);                     //实际的放置棋子函数
	inline void unPlace(int x, int y);                              //取消棋子函数
	inline bool judge(point p,char name);
	inline void printChess();   //打印棋盘
	long score(point p, int name,dirmark& mark);  //单点评估函数
	long score(point p, int name,bool& hasddie4);
	inline long wholeValue(); //全盘评估函数
	//inline long wholeValue(point remove,char name, point add, long curValue);
	inline long wholeValue(long preValue, point addPoint);
	inline bool isInBoard(point p);  //判断该坐标是否合法
	inline direction mapping(int i);  //映射方向
	inline point nextPoint(point p, direction d, int length);  //计算邻接结点坐标
	void getLinkPiece(int &length, point &linkPoint, int name, point p, direction d, int pn);  //获取一个坐标的邻接点用以计算连线长度
	void setDirMark(point p, char name,int dir, direction d,dirmark& mark);
	void getBoundary(char * left, point le, char * right, point ri, direction d, char name);      //获取连线方向两边4点以内的所有坐标信息
	sum situaltionAnalysis(int length, char name, char * left, char * right);                 //分析一点的棋型
	inline void intelligentRandom(point &bestMove, long value, priority_queue<waitPoint, vector<waitPoint>, cmp> &queue);  //AI智能随机化函数
	bool hasNeighbor(point p,int dir=0);
	bool hasNeighbor(point p, char name,int dir=0);
	set<point> inspireFind(point preMePoint,point preHePoint);                           //启发式搜索函数   AI性能提升的关键函数
	set<point> inspireFind();
	bool Fobidden();
	tuple<long,bool,bool> findCompMove(point &bestMove, int deep,int endDeep, int alpha, int beta,point preComMove,point preHumMove);                    //关键的AI对策分析函数1
	tuple<long,bool,bool> findHumanMove(point &bestMove, int deep,int endDeep, int alpha, int beta,point preComMove,point preHumMove);                   //关键的AI对策分析函数2
	point iterationDeep(point preCom,point preHum);   //迭代加深 优化速度
	/*函数用以排序的计算*/
	bool compareFuc(const tuple<point,long,bool>& p1,const tuple<point,long,bool>& p2);
	
};
#endif
