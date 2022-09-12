#ifndef _GOBANGC_
#define _GOGANGC_
#include "GoBang.h"
#include<iostream>
#include<algorithm>
#include<ctime>
#include<random>
using namespace std;
using namespace placeholders;

GoBang::GoBang()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			chessBoard[i][j] = ' ';
	/*chessBoard[2][2] = COMPTER;
	chessBoard[2][4] = HUMAN;
	chessBoard[3][2] = HUMAN;
	chessBoard[3][3] = COMPTER;
	chessBoard[3][5] = HUMAN;
	chessBoard[4][2] = COMPTER;
	chessBoard[4][3] = COMPTER;
	chessBoard[4][4] = HUMAN;
	chessBoard[4][5] = HUMAN;
	chessBoard[5][2] = COMPTER;
	chessBoard[6][6] = HUMAN;
	chessBoard[5][3] = COMPTER;
	chessBoard[5][5] = HUMAN;*/
	printChess();
	
}


GoBang::~GoBang()
{
}

void GoBang::addPiece(int x, int y)
{
	if (x > 15 || y > 15 || x < 1 || y < 1)
		throw out_of_range("坐标值无效！");
	else
	{
		if (!isEmpty(x - 1, y - 1))
			throw runtime_error("该坐标出已经有棋子了！");
		else
			place(x - 1, y - 1, HUMAN);
	}
	system("cls");
	printChess();
	if (judge(HUMAN))            //赢否
	{
		forbiddenHands();        //先禁手后判断
		throw COMPTER_LOSS;
	}
	if (fullBoard())
		throw DRAW;
	cout << "电脑的回合" << endl;
	immWin();                  //避免直接调用findCompMove,减少用时1
	if (immKill());            //避免直接调用findCompMove,减少用时2
	else
	{
		point bestMove;
		clock_t start = clock();
		findCompMove(bestMove, 1, COMPTER_LOSS, COMPTER_WIN);
		clock_t end = clock();
		place(bestMove.x, bestMove.y, COMPTER);
		system("cls");
		printChess();
		cout << "电脑落子坐标：" << bestMove.x + 1 << " " << bestMove.y + 1 << "   本次电脑思考耗时：" << double(end - start) / CLOCKS_PER_SEC << "秒" << endl;
	}
	if (judge(COMPTER))   //赢否
		throw COMPTER_WIN;
	if (fullBoard())
		throw DRAW;
}


inline GoBang::point GoBang::nextPoint(point p, direction d, int length)
{
	return { p.x + d.dx * length, p.y + d.dy * length };    //简单的数学计算
}

inline bool GoBang::fullBoard()
{
	if (prieceNumber == 225)
		return true;
	else
		return false;
}

bool GoBang::judge(char name)
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (!isEmpty(i, j)&&chessBoard[i][j]==name)
			{
				for (int z = 1; z <= 4; z++)
				{
					direction d = mapping(z);
					int length = 1;
					point le, ri;
					getLinkPiece(length, le, name, { i,j }, d, -1);
					getLinkPiece(length, ri, name, { i,j }, d, 1);
					if (length == 5)
						return true;
				}
			}
		}
	return false;
}

inline bool GoBang::isEmpty(const int x, const int y)
{
	if (chessBoard[x][y] != ' ')
		return false;
	else
		return true;
}

void GoBang::printChess()
{
	for (int j = 0; j < 15; j++)
	{
		if (j == 0)
			cout << "     " << j + 1;
		else
		{
			if (j < 9)
				cout << "   " << j + 1;
			else
				cout << "  " << j + 1;
		}
	}
	cout << endl;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (j == 0)
				cout << "   +---";
			else
				cout << "+---";
		}
		cout << "+" << endl;
		for (int j = 0; j < 15; j++)
		{
			if (j == 0)
			{
				if(i<9)
					cout << i + 1 << "  | " << chessBoard[i][j];
				else
					cout << i + 1 << " | " << chessBoard[i][j];
			}
			else
				cout << " | " << chessBoard[i][j];
		}
		cout << " |" << endl;
	}
	for (int j = 0; j < 15; j++)
	{
		if (j == 0)
			cout << "   +---";
		else
			cout << "+---";
	}
	cout << "+" << endl;
}

void GoBang::forbiddenHands()
{
	point bestMove;
	clock_t start = clock();
	try
	{
		immedicteWin(COMPTER, bestMove, 1);
	}
	catch (exception)
	{
		clock_t end = clock();
		place(bestMove.x, bestMove.y, COMPTER);
		system("cls");
		printChess();
		cout << "电脑落子坐标：" << bestMove.x + 1 << " " << bestMove.y + 1 << "   本次电脑思考耗时：" << double(end - start) / CLOCKS_PER_SEC << "秒" << endl;
		throw FORBIDDEN;
	}
}

bool GoBang::immKill()
{
	point bestMove;
	clock_t start = clock();
	try
	{
		immedicteWin(HUMAN, bestMove, 2);
	}
	catch (int &result)
	{
		if (result == IMMKILL)
		{
			clock_t end = clock();
			place(bestMove.x, bestMove.y, COMPTER);
			system("cls");
			printChess();
			cout << "电脑落子坐标：" << bestMove.x + 1 << " " << bestMove.y + 1 << "   本次电脑思考耗时：" << double(end - start) / CLOCKS_PER_SEC << "秒" << endl;
			return true;
		}
		else
			throw;
	}
	return false;
}

void GoBang::immWin()
{
	point bestMove;
	clock_t start = clock();
	try
	{
		immedicteWin(COMPTER, bestMove, 1);
	}
	catch (exception)
	{
		clock_t end = clock();
		place(bestMove.x, bestMove.y, COMPTER);
		system("cls");
		printChess();
		cout << "电脑落子坐标：" << bestMove.x + 1 << " " << bestMove.y + 1 << "   本次电脑思考耗时：" << double(end - start) / CLOCKS_PER_SEC << "秒" << endl;
		throw COMPTER_WIN;
	}
}

/*AI棋力的决定函数之一，评估函数的好坏决定AI棋力的因素之一*/
int GoBang::score(point p, int name)
{
	int totalScore = 0;
	sum result;
	sum temp;
	for (int i = 1; i <= 4; i++)
	{
		direction d = mapping(i);
		int length = 1;
		point le, ri, tempPoint;
		char left[5], right[5];
		getLinkPiece(length, le, name, p, d, -1);
		getLinkPiece(length, ri, name, p, d, 1);
		getBoundary(left, le, right, ri, d, name);
		temp= situaltionAnalysis(length, name, left, right);
		result += temp;
	}
	if (result.win5>=1)
		totalScore += 20000000;
	if (result.alive4 >= 1 || result.dalive4 >= 2 || (result.dalive4 >= 1 && result.alive3 >= 1) || result.alive3 >= 2)
		totalScore+= 10000000;      //绝杀局
	totalScore += result.dalive4 * 10000 + result.die4 * 5000 + result.alive3 * 10000 + result.dalive4 * 1000 + result.die3 * 500 + result.alive2 * 1000 + result.dalive2 * 100 + result.die2 * 50 + result.alive1 * 100 + result.dalive1 * 10 + result.die1 * 5;
	return totalScore;
}

inline GoBang::scoreValue GoBang::wholeValue(scoreValue &value)    //全局调用score
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (!isEmpty(i, j))
			{
				if (chessBoard[i][j] == COMPTER)
					value.compterValue += score({ i,j }, COMPTER);
				else
					value.humanValue += score({ i,j }, HUMAN);
			}
		}
	value.compterValue -= value.humanValue;
	return value;
}

inline bool GoBang::isInBoard(point p)
{
	if (p.x >= 0 && p.x <= 14 && p.y >= 0 && p.y <= 14)
		return true;
	else
		return false;
}

inline GoBang:: direction GoBang::mapping(int i)
{
	/*四个正方向*/
	const direction d1 = { 0, 1 };               
	const direction d2 = { 1, 0 };               
	const direction d3 = { 1, -1 };              
	const direction d4 = { 1, 1 };               
	switch (i)
	{
	case 1:return d1; break;
	case 2:return d2; break;
	case 3:return d3; break;
	case 4:return d4; break;
	}
}

void GoBang::getLinkPiece(int & length, point & linkPoint,int name, point p, direction d, int pn)
{
	point tempPoint = nextPoint(p, d, pn);
	linkPoint = p;
	while (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] == name)
	{
		linkPoint = tempPoint;
		tempPoint = nextPoint(tempPoint, d, pn);
		length++;
	}
}

void GoBang::getBoundary(char * left,point le, char * right,point ri,direction d,char name)
{
	char opp;
	if (name == COMPTER)
		opp = HUMAN;
	else
		opp = COMPTER;
	for (int i = 1; i <= 4; i++)
	{
		point tempPoint = nextPoint(le, d, -i);
		if (isInBoard(tempPoint))
			left[i] = chessBoard[tempPoint.x][tempPoint.y];
		else
			left[i] = opp;
		tempPoint = nextPoint(ri, d, i);
		if (isInBoard(tempPoint))
			right[i] = chessBoard[tempPoint.x][tempPoint.y];
		else
			right[i] = opp;
	}
}

/*复杂的棋型分析，分很多中情况*/
GoBang::sum GoBang::situaltionAnalysis(int length, char name, char * left, char * right)
{
	sum temp;
	if (length >= 5)
		temp.win5++;
	else if (length == 4)
	{
		if (left[1] == ' ' && right[1] == ' ')
			temp.alive4++;
		else if (left[1] == ' ' || right[1] == ' ')
			temp.dalive4++;
		else
			temp.die4;
	}
	else if (length == 3)
	{
		if ((left[1] == ' ' && left[2] == name) || (right[1] == ' ' && right[2] == name))
			temp.dalive4++;
		else if (left[1] == ' ' && right[1] == ' ' && (left[2] == ' ' || right[2] == ' '))
			temp.alive3++;
		else if ((left[1] == ' ' && left[2] == ' ') || (right[1] == ' ' && right[2] == ' '))
			temp.dalive3++;
		else
			temp.die3++;
	}
	else if (length == 2)
	{
		if ((left[1] == ' ' && left[2] == name && left[3] == name) &&
			(right[1] == ' ' && right[2] == name && right[3] == name))
			temp.dalive4++;
		else if (left[1] == ' ' && right[1] == ' ' &&
			((left[2] == name && left[3] == ' ') || (right[2] == name && right[3] == ' ')))
			temp.dalive3++;
		else if ((left[1] == ' ' && left[3] == ' ' && left[2] == name) ||
			(right[1] == ' ' && right[3] == ' ' && right[2] == name))
			temp.die3++;
		else if ((left[1] == ' ' && right[1] == ' ') &&
			(left[2] == name || right[2] == name))
			temp.die3++;
		else if ((left[1] == ' ' && left[2] == ' ' && left[3] == name) ||
			(right[1] == ' ' && right[2] == ' ' && right[3] == name))
			temp.die3++;
		else if ((left[1] == ' ' && right[1] == ' ' && right[2] == ' ' && right[3] == ' ') ||
			(left[1] == ' ' && left[2] == ' ' && right[1] == ' ' && right[2] == ' ') ||
			(left[1] == ' ' && left[2] == ' ' && left[3] == ' ' && right[1] == ' '))
			temp.alive2++;
		else if ((left[1] == ' ' && left[2] == ' ' && left[3] == ' ') ||
			(right[1] == ' ' && right[2] == ' ' && right[3] == ' '))
			temp.die2++;
	}
	else if (length == 1) {
		if ((left[1] == ' ' && left[2] == name && left[3] == name && left[4] == name) ||
			(right[1] == ' ' && right[2] == name && right[3] == name && right[4] == name))
			temp.dalive4++;
		else if ((left[1] == ' ' && right[1] == ' ') && ((left[2] == name && left[3] == name && left[4] == ' ') ||
			(right[2] == name && right[3] == name && right[4] == ' ')))
			temp.dalive3++;
		else if ((left[1] == ' ' && right[1] == ' ') &&
			((left[2] == name && left[3] == name) || (right[2] == name && right[3] == name)))
			temp.die3++;
		else if ((left[1] == ' ' && left[4] == ' ' && left[2] == name && left[3] == name) ||
			(right[1] == ' ' && right[4] == ' ' && right[2] == name && right[3] == name))
			temp.die3++;
		else if ((left[1] == ' ' && left[2] == ' ' && left[3] == name && left[4] == name) ||
			(right[1] == ' ' && right[2] == ' ' && right[3] == name && right[4] == name))
			temp.die3++;
		else if ((left[1] == ' ' && left[3] == ' ' && left[2] == name && left[4] == name) ||
			(right[1] == ' ' && right[3] == ' ' && right[2] == name && right[4] == name))
			temp.die3++;
		else if ((left[1] == ' ' && right[1] == ' ' && right[3] == ' ' && right[2] == name) && (left[2] == ' ' || right[4] == ' '))
			temp.dalive2++;
		else if ((right[1] == ' ' && left[1] == ' ' && left[3] == ' ' && left[2] == name) &&
			(right[2] == ' ' || left[4] == ' '))
			temp.dalive2++;
		else if ((left[1] == ' ' && right[1] == ' ' && right[2] == ' ' && right[4] == ' ' && right[3] == name) ||
			(right[1] == ' ' && left[1] == ' ' && left[2] == ' ' && left[4] == ' ' && left[3] == name))
			temp.dalive2++;
		else if ((left[1] == ' ' && left[3] == ' ' && left[4] == ' ' && left[2] == name) ||
			(right[1] == ' ' && right[3] == ' ' && right[4] == ' ' && right[2] == name))
			temp.die2++;
		else if ((left[1] == ' ' && right[1] == ' ' && right[2] == ' ' && left[2] == name) ||
			(right[1] == ' ' && left[1] == ' ' && left[2] == ' ' && right[2] == name))
			temp.die2++;
		else if ((left[1] == ' ' && left[2] == ' ' && left[4] == ' ' && left[3] == name) ||
			(right[1] == ' ' && right[2] == ' ' && right[4] == ' ' && right[3] == name))
			temp.die2++;
		else if ((left[1] == ' ' && left[2] == ' ' && right[1] == ' ' && left[3] == name) ||
			(right[1] == ' ' && right[2] == ' ' && left[1] == ' ' && right[3] == name))
			temp.die2++;
		else if ((left[1] == ' ' && left[2] == ' ' && left[3] == ' ' && left[4] == name) ||
			(right[1] == ' ' && right[2] == ' ' && right[3] == ' ' && right[4] == name))
			temp.die2++;
		else if (left[1] == ' '&&right[1] == ' ')
			temp.alive1++;
		else if (left[1] == ' ' || right[1] == ' ')
			temp.dalive1++;
		else
			temp.die1++;
	}
	return temp;
}

bool GoBang::immedicteWin(char name, point & bestMove,int flag)
{
	vector<point> waitTemp;
	for(int i=0;i<15;i++)
		for (int j = 0; j < 15; j++)
		{
			sum temp;
			sum result;
			if (isEmpty(i, j))
			{
				place(i, j, name);
				for (int z = 1; z <= 4; z++)
				{
					direction d = mapping(z);
					int length = 1;
					point le, ri;
					char left[5], right[5];
					getLinkPiece(length, le, name, { i,j }, d, -1);
					getLinkPiece(length, ri, name, { i,j }, d, 1);
					getBoundary(left, le, right, ri, d, name);
					temp = situaltionAnalysis(length, name, left, right);
					result += temp;
					if (result.win5 >= 1)
					{
						bestMove = { i,j };
						unPlace(i, j);
						if (flag==1)
							throw exception();   //用以跳转至处理禁手规则的函数
						if (flag == 2)
							throw IMMKILL;       //用以调转而加快计算机对必杀局的判断速度
						return true;
					}
				}
				if (result.alive4 >= 1 || result.dalive4 >= 2 || (result.dalive4 >= 1 && result.alive3 >= 1) || result.alive3 >= 2)
					waitTemp.push_back({ i,j });
				unPlace(i, j);
			}
		}
	default_random_engine e(time(0));
	if (waitTemp.empty())
		return false;
	else
	{
		uniform_int_distribution<unsigned> u(0, waitTemp.size()-1);
		bestMove = waitTemp[u(e)];
		return true;
	}
}
/*整个程序的关键部分 AI智能的来源 主要利用极大极小值算法加上动态规划，α-β裁剪 (其余四个对称函数就不再进行重复解释）*/
GoBang::scoreValue GoBang::findCompMove(point & bestMove,int deep, int alpha, int beta)
{
	point dc;
	scoreValue responseValue;
	scoreValue value;
	priority_queue<waitPoint, vector<waitPoint>, cmp> queue;
	if (fullBoard())
		return wholeValue(value);
	else
		if (immedicteWin(COMPTER, bestMove))   
			return { COMPTER_WIN,0 };            //递归出口：满盘，或者必杀棋出现停止
		else
		{
			vector<point> temp = inspireFind(COMPTER);   //搜索可能的落子位置
			//sortCompter(temp);
			value.compterValue = alpha; bestMove = temp[0];
			for (auto p : temp)
			{
				if (value.compterValue > beta)   //α裁剪 AI性能提升的关键1
					break;
				if (isEmpty(p.x, p.y))
				{
					place(p.x, p.y, COMPTER);   //落子
					responseValue = findHumanMove(dc, deep+1, value.compterValue, beta);  //递归
					unPlace(p.x, p.y);         //取子
					if (deep == 1)             //顶端findCompMove采用比较然后随机选取的方法
						queue.push(waitPoint(responseValue, { p.x,p.y }));
					else if (responseValue.compterValue > value.compterValue)  //其余全部的findComMove只存放对自己最有利的棋子位置
					{
						value = responseValue;
						bestMove = { p.x,p.y };
					}
				}
			}
		}
	if (deep == 1)
		intelligentRandom(bestMove, value, queue);  //顶端findComMove随机选取较好的结点
	return value;
}

/*避免AI的棋法单一*/
inline void GoBang::intelligentRandom(point & bestMove, scoreValue & value, priority_queue<waitPoint, vector<waitPoint>, cmp> &queue)
{
	waitPoint first = queue.top(); queue.pop();
	waitPoint second = queue.top();
	if (first.value.compterValue <= second.value.compterValue * 50)  //如果差太多了就取较大的
	{
		default_random_engine e(time(0));
		bernoulli_distribution u;
		if (u(e) == true)
		{
			bestMove = first.p;
			value = first.value;
		}
		else
		{
			bestMove = second.p;
			value = second.value;
		}
	}
	else
	{
		bestMove = first.p;
		value = first.value;
	}
}

GoBang::scoreValue GoBang::compKillBoard(point & bestMove, int deep, int alpha, int beta)
{
	point dc;
	scoreValue responseValue;
	scoreValue value;
	if (deep == 14 || fullBoard())
		return wholeValue(value);
	else
		if (immedicteWin(COMPTER, bestMove))
			return { COMPTER_WIN,0 };
		else
		{
			vector<point> emptyDefense;
			vector<point> temp = killFindComp(emptyDefense);
			if (temp.empty())
				return wholeValue(value);
			value.compterValue = alpha; bestMove = temp[0];
			for (auto p : temp)
			{
				if (value.compterValue > beta)
					break;
				if (isEmpty(p.x, p.y))
				{
					place(p.x, p.y, COMPTER);
					responseValue = humanKillBoard(dc, deep + 1, value.compterValue, beta,emptyDefense);
					unPlace(p.x, p.y);
					if (responseValue.compterValue > value.compterValue)
					{
						value = responseValue;
						bestMove = { p.x,p.y };
					}
				}
			}
		}
	return value;
}

GoBang::scoreValue GoBang::findHumanMove(point & bestMove,int deep, int alpha, int beta)
{
	point dc;
	scoreValue responseValue;
	scoreValue value;
	if (fullBoard())
		return wholeValue(value);
	if (deep == 4 && prieceNumber >= 5)  //棋子数大于五时才进行算杀 这是关键 否则开始时的两步计算机将要计算大量的时间
		return interfaceFunction(bestMove, deep, alpha, beta);
	else if (deep == 4)
		return wholeValue(value);    //棋子数少于五时就且深度为4时可以停止搜索，以节约时间
	if (immedicteWin(HUMAN, bestMove))
		return { COMPTER_LOSS,0 };
	else
	{
		vector<point> temp = inspireFind(HUMAN);
		//sortHuman(temp);
		value.compterValue = beta; bestMove = temp[0];
		for (auto p : temp)
		{
			if (alpha > value.compterValue)    //β裁剪 AI性能提升的关键2
				break;
			if (isEmpty(p.x, p.y))
			{
				place(p.x, p.y, HUMAN);
				responseValue = findCompMove(dc, deep + 1, alpha, value.compterValue);
				unPlace(p.x, p.y);
				if (responseValue.compterValue < value.compterValue)
				{
					value = responseValue;
					bestMove = { p.x,p.y };
				}
			}
		}
	}
	return value;
}

GoBang::scoreValue GoBang::humanKillBoard(point & bestMove, int deep, int alpha, int beta,vector<point> &defense)
{
	point dc;
	scoreValue responseValue;
	scoreValue value;
	if (deep == 14 || fullBoard())
		return wholeValue(value);
	else
		if (immedicteWin(HUMAN, bestMove))
			return { COMPTER_LOSS,0 };
		else
		{
			vector<point> temp = killFindHuman(defense);
			if (temp.empty())
				return wholeValue(value);
			value.compterValue = beta; bestMove = temp[0];
			for (auto p : temp)
			{
				if (alpha > value.compterValue)
					break;
				if (isEmpty(p.x, p.y))
				{
					place(p.x, p.y, HUMAN);
					responseValue = compKillBoard(dc, deep + 1, alpha, value.compterValue);
					unPlace(p.x, p.y);
					if (responseValue.compterValue < value.compterValue)
					{
						value = responseValue;
						bestMove = { p.x,p.y };
					}
				}
			}
		}
	return value;
}

/*对于电脑而言只查看活三，死四（冲四）棋，并把预防对策放入emptyDefense中提交给killFindHuman*/
vector<GoBang::point> GoBang::killFindComp(vector<point> &emptyDefense)
{
	vector<point> waitTemp;
	for(int i=0;i<15;i++)
		for (int j = 0; j < 15; j++)
		{
			if (isEmpty(i,j))
			{
				place(i, j, COMPTER);
				sum result;
				int flag = 1;
				for (int z = 1; z <= 4; z++)
				{
					direction d = mapping(z);
					int length = 1;
					point le, ri, tempPoint;
					char left[5], right[5];
					getLinkPiece(length, le, COMPTER, { i,j }, d, -1);
					getLinkPiece(length, ri, COMPTER, { i,j }, d, 1);
					getBoundary(left, le, right, ri, d, COMPTER);
					result = situaltionAnalysis(length, COMPTER, left, right);
					if (result.alive3 >= 1 || result.dalive4 >= 1||result.alive4>=1||result.win5>=1)
					{
						for (int k = 1; k <= 4; k++)
						{
							if (left[k] == ' ')
							{
								point temp = nextPoint(le, d, -k);
								emptyDefense.push_back(temp);
							}
							if (right[k] == ' ')
							{
								point temp = nextPoint(ri, d, k);
								emptyDefense.push_back(temp);
							}
						}
						if (flag)
						{
							waitTemp.push_back({ i,j });
							flag = 0;
						}
					}
				}
				unPlace(i, j);
			}
		}
	return waitTemp;
}

/*对于人而言只查看电脑的活三，死四的防御点和自己的活三、死四点（冲四）*/
vector<GoBang::point> GoBang::killFindHuman(vector<point> &defense)
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (isEmpty(i, j))
			{
				place(i, j, HUMAN);
				sum result;
				for (int z = 1; z <= 4; z++)
				{
					direction d = mapping(z);
					int length = 1;
					point le, ri, tempPoint;
					char left[5], right[5];
					getLinkPiece(length, le, HUMAN, { i,j }, d, -1);
					getLinkPiece(length, ri, HUMAN, { i,j }, d, 1);
					getBoundary(left, le, right, ri, d, HUMAN);
					result = situaltionAnalysis(length, HUMAN, left, right);
					if (result.alive3 >= 1 || result.alive4>=1||result.dalive4>=1||result.win5>=1)
					{
					    defense.push_back({ i,j });
						unPlace(i, j);   
					}
				}
				unPlace(i, j);
			}
		}
	return defense;
}

GoBang::scoreValue GoBang::interfaceFunction(point &bestMove,int deep,int aplha,int beta)
{
	vector<point> emptyDefense;
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (chessBoard[i][j] == COMPTER)
			{
				sum result;
				for (int z = 1; z <= 4; z++)
				{
					direction d = mapping(z);
					int length = 1;
					point le, ri, tempPoint;
					char left[5], right[5];
					getLinkPiece(length, le, COMPTER, { i,j }, d, -1);
					getLinkPiece(length, ri, COMPTER, { i,j }, d, 1);
					getBoundary(left, le, right, ri, d, COMPTER);
					result = situaltionAnalysis(length, COMPTER, left, right);
					if (result.alive3 >= 1 || result.alive4||result.dalive4)
					{
						for (int k = 1; k <= 4; k++)
						{
							if (left[k] == ' ')
							{
								point temp = nextPoint(le, d, -k);
								emptyDefense.push_back(temp);
							}
							if (right[k] == ' ')
							{
								point temp = nextPoint(ri, d, k);
								emptyDefense.push_back(temp);
							}
						}
					}
				}
			}
		}
	return humanKillBoard(bestMove, deep, aplha, beta, emptyDefense);
}

inline void GoBang::place(int x, int y, char name)
{
	chessBoard[x][y] = name;
	prieceNumber++;
}

inline void GoBang::unPlace(int x, int y)
{
	chessBoard[x][y] = ' ';
	prieceNumber--;
}

/*启发式搜索 AI性能提升的关键3*/
vector<GoBang::point> GoBang::inspireFind(char name)
{
	vector<point> temp;
	for(int i=0;i<15;i++)
		for (int j = 0; j < 15; j++)
			if (isEmpty(i, j) && hasNeighbor({ i,j }, name))  //只取两步之内有邻居的结点位置
				temp.push_back({ i,j });
	return temp;
}

bool GoBang::hasNeighbor(point p,char name)
{
	for (int i = 1; i <= 4; i++)
	{
		direction d = mapping(i);
		for (int j = 1; j <= 2; j++)
		{
			point tempPoint = nextPoint(p, d, j);
			if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] != ' ')
				return true;
			tempPoint = nextPoint(p, d, -j);
			if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] != ' ')
				return true;
		}
	}
	return false;
}

bool GoBang::compareHuman(point p1, point p2)
{
	return less<int>()(score(p1, HUMAN), score(p2, HUMAN));
}

void GoBang::sortCompter(vector<point>& p)
{
	int j;
	for (int i = 1; i < p.size(); i++)
	{
		point temp = p[i];
		for (j = i; j > 0 && compareCompter(temp, p[j - 1]); j--)
			p[j] = p[j - 1];
		p[j] = temp;
	}
}

void GoBang::sortHuman(vector<point>& p)
{
	int j;
	for (int i = 1; i < p.size(); i++)
	{
		point temp = p[i];
		for (j = i; j > 0 && compareHuman(temp, p[j - 1]); j--)
			p[j] = p[j - 1];
		p[j] = temp;
	}
}

bool GoBang::compareCompter(point p1, point p2)
{
	return greater<int>()(score(p1, COMPTER), score(p2, COMPTER));
}

#endif
