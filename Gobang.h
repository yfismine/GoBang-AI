#ifndef _GOBANGH_
#define _GOBANGH_
#pragma once
#include<vector>
#include<memory>
#include<functional>
#include<queue>
#define HUMAN 'O'
#define COMPTER 'X'
#define COMPTER_WIN 20000000
#define COMPTER_LOSS -20000000
#define FORBIDDEN 5
#define IMMKILL   6
#define DRAW 0
using namespace std;
class GoBang
{
public:
	GoBang();
	~GoBang();
	void addPiece(int x, int y);        //玩家可调用的落子函数
private:
	struct point                       //存放棋子坐标
	{
		int x = 0;
		int y = 0;
		bool operator==(point &rhs)
		{
			if (x == rhs.x&&y == rhs.y)
				return true;
			else
				return false;
		}
		point() = default;
		point(int row, int col)
			:x(row), y(col) {};
	};             
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
	struct scoreValue              //棋盘局势值统计
	{
		int compterValue=0;
		int humanValue=0;
	};
	struct waitPoint              //待选坐标点
	{
		scoreValue value;
		point p;
		waitPoint() = default;
		waitPoint(scoreValue v, point p1)
			:value(v), p(p1) {};
	};
	struct cmp                   //比较函数用以优先队列的比较
	{
		bool operator()(waitPoint &p1, waitPoint &p2)
		{
			return p1.value.compterValue < p2.value.compterValue;
		}
	};
	char chessBoard[15][15];      //棋盘
	int prieceNumber = 0;         //棋盘中的棋子数
	inline point nextPoint(point p, direction d, int length);  //计算邻接结点坐标
	inline bool fullBoard();    //判断是否满盘
	bool judge(char name);      //判断是否一方胜利
	inline bool isEmpty(const int x, const int y);  //判断一点是否为空位置
	int score(point p, int name);  //单点评估函数
	inline scoreValue wholeValue(scoreValue &value); //全盘评估函数
	inline void printChess();   //打印棋盘
	void forbiddenHands();      //判断是否玩家触发禁手规则
	bool immKill();             //判断玩家的该棋是否为必防棋
	void immWin();              //判断电脑是否可以通过一步立刻获胜，缩小计算机的计算时间
	inline bool isInBoard(point p);  //判断该坐标是否合法
	inline direction mapping(int i);  //映射方向
	void getLinkPiece(int &length, point &linkPoint,int name, point p, direction d, int pn);  //获取一个坐标的邻接点用以计算连线长度
	void getBoundary(char * left,point le, char * right,point ri,direction d,char name);      //获取连线方向两边4点以内的所有坐标信息
	sum situaltionAnalysis(int length, char name, char * left, char * right);                 //分析一点的棋型
	bool immedicteWin(char name, point &bestMove, int flag=0);                                //通用的统计是否是杀棋
	scoreValue findCompMove(point &bestMove,int deep,int alpha, int beta);                    //关键的AI对策分析函数1
	inline void intelligentRandom(point &bestMove, scoreValue &value,priority_queue<waitPoint,vector<waitPoint>,cmp> &queue);  //AI智能随机化函数
	scoreValue compKillBoard(point &bestMove, int deep, int alpha, int beta);                  //算杀  关键的AI棋力提升函数1
	scoreValue findHumanMove(point &bestMove,int deep, int alpha, int beta);                   //关键的AI对策分析函数2
	scoreValue humanKillBoard(point &bestMove, int deep, int alpha, int beta, vector<point> &defense); //算杀  关键的AI棋力提升函数2
	vector<point> killFindComp(vector<point> &defense);             //AI的算杀启发式搜索函数1
	vector<point> killFindHuman(vector<point> &emptyDefense);       //AI的算杀启发式搜索函数2
	scoreValue interfaceFunction(point &bestMove,int deep,int alpha,int beta);     //算杀与全局推断的接口
	inline void place(int x, int y, char name);                     //实际的放置棋子函数
	inline void unPlace(int x, int y);                              //取消棋子函数
	vector<point> inspireFind(char name);                           //启发式搜索函数   AI性能提升的关键函数
	bool hasNeighbor(point p,char name);
	/*后面四个函数用以排序的计算*/
	bool compareCompter(point p1, point p2);                       
	bool compareHuman(point p1, point p2);
	void sortCompter(vector<point> &p);
	void sortHuman(vector<point> &p);
};
#endif

