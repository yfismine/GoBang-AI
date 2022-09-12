#ifndef _GOBANGC_
#define _GOGANGC_
#include "GoBang.h"
#include<iostream>
#include<algorithm>
#include<random>
using namespace std;
using namespace placeholders;


bool operator==(const point &p1, const point&p2)
{
	if (p1.x == p2.x&&p1.y == p2.y)
		return true;
	else
		return false;
}
bool operator<(const point &p1, const point&p2)
{
	return p1.x * 16 + p1.y < p2.x * 16 + p2.y;
}
size_t hasher(const point & p)
{
	return p.x * 16 + p.y;
}
GoBang::GoBang()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			chessBoard[i][j] = ' ';
	zobristKey = 0;
	mt19937_64 e;
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			for (int z = 0; z < 3; z++)
				randomArray[i][j].push_back(e());
			zobristKey ^= randomArray[i][j][0];
		}
	number = 0;
	maxDeep = 0;
	currentValue = 0;
	extension = false;
	printChess();
}


GoBang::~GoBang()
{
}

void GoBang::addPiece(int x, int y)
{
	static point preCom(-1,-1), preHum(-1,-1);
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
	preHum = { x - 1,y - 1 };
	if (judge({ x - 1,y - 1 }, HUMAN))
	{
		if (Fobidden())
		{
			system("cls");
			printChess();
			throw FORBIDDEN;
		}
		throw COMPTER_LOSS;
	}
	if (fullBoard())
		throw DRAW;
	currentValue = wholeValue(currentValue, { x - 1,y - 1 });
	cout << "电脑的回合" << endl;
	clock_t start = clock();
	point bestMove = iterationDeep(preCom,preHum);
	clock_t end = clock();
	place(bestMove.x, bestMove.y, COMPTER);
	currentValue = wholeValue(currentValue, bestMove);
	system("cls");
	printChess();
	cout << "电脑落子坐标：" << bestMove.x + 1 << " " << bestMove.y + 1 << "   本次电脑思考耗时：" << double(end - start) / CLOCKS_PER_SEC << "秒" << endl;
	cout << "电脑思考最大深度为：" << maxDeep << ends << ends<<ends << "当前局面得分：" << currentValue << endl;
	preCom = bestMove;
	points.push({ { x - 1,y - 1 },currentValue });
	points.push({ bestMove,currentValue });
	if (judge(bestMove, COMPTER))
		throw COMPTER_WIN;
	if (fullBoard())
		throw DRAW;
}


inline point GoBang::nextPoint(point p, direction d, int length)
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
				if (i < 9)
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

/*AI棋力的决定函数之一，评估函数的好坏决定AI棋力的因素之一*/
long GoBang::score(point p, int name,dirmark& mark)
{
	long totalScore = 0;
	sum result;
	sum temp;
	if (mark.find(p) == mark.end())
		mark.insert(make_pair(p, vector<bool>(5, false)));
	for (int i = 1; i <= 4; i++)
	{
		if (mark[p][i])
			continue;
		direction d = mapping(i);
		int length = 1;
		point le, ri, tempPoint;
		char left[5], right[5];
		getLinkPiece(length, le, name, p, d, -1);
		getLinkPiece(length, ri, name, p, d, 1);
		getBoundary(left, le, right, ri, d, name);
		temp = situaltionAnalysis(length, name, left, right);
		setDirMark(p, name, i, d, mark);
		result += temp;
		point lePoint=nextPoint(p,d,-1), riPoint=nextPoint(p,d,1);
		for (int j = 1; j <= 4; j++)
		{
			if (chessBoard[lePoint.x][lePoint.y] == name)
			{
				if (mark.find(lePoint) != mark.end())
					mark[lePoint][i] = true;
				else
				{
					mark.insert(make_pair(lePoint, vector<bool>(5, false)));
					mark[lePoint][i] = true;
				}
			}
			lePoint = nextPoint(lePoint, d, -1);
		}
		for (int j = 1; j <= 4; j++)
		{
			if (chessBoard[riPoint.x][riPoint.y] == name)
			{
				if (mark.find(riPoint) != mark.end())
					mark[riPoint][i] = true;
				else
				{
					mark.insert(make_pair(riPoint, vector<bool>(5, false)));
					mark[riPoint][i] = true;
				}
			}
			riPoint = nextPoint(riPoint, d, -1);
		}
		mark[p][i] = true;
	}
	if (result.win5 >= 1 || result.alive4 >= 1 || result.dalive4 >= 2 || (result.dalive4 >= 1 && result.alive3 >= 1) || result.alive3 >= 2)
		totalScore += 20000000;      //绝杀局
	/*else if (result.dalive4 >= 1 && result.alive2 == 0 && result.dalive2 == 0 && result.die2 == 0 && result.dalive3 == 0 && result.die3 == 0)
		totalScore += 1000;*/
	else
		totalScore += result.dalive4 * 10000 + result.die4 * 5000 + result.alive3 * 10000 + result.dalive4 * 1000 + result.die3 * 500 + result.alive2 * 1000 + result.dalive2 * 100 + result.die2 * 50 + result.alive1 * 100 + result.dalive1 * 10 + result.die1 * 5;
	return totalScore;
}

long GoBang::score(point p, int name,bool &hasddie4)
{
	long totalScore = 0;
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
		temp = situaltionAnalysis(length, name, left, right);
		result += temp;
	}
	if (result.dalive4 >= 1)
		hasddie4 = true;
	if (result.win5 >= 1 || result.alive4 >= 1 || result.dalive4 >= 2 || (result.dalive4 >= 1 && result.alive3 >= 1) || result.alive3 >= 2)
		totalScore += 20000000;      //绝杀局
	/*else if (result.dalive4 >= 1 && result.alive2 == 0 && result.dalive2 == 0 && result.die2 == 0 && result.dalive3 == 0 && result.die3 == 0)
		totalScore += 1000;*/
	else
		totalScore += result.dalive4 * 10000 + result.die4 * 5000 + result.alive3 * 10000 + result.dalive4 * 1000 + result.die3 * 500 + result.alive2 * 1000 + result.dalive2 * 100 + result.die2 * 50 + result.alive1 * 100 + result.dalive1 * 10 + result.die1 * 5;
	return totalScore;
}

inline long GoBang::wholeValue()    //全局调用score
{
	dirmark mark(59,hasher);
	long computerValue=0, humanValue=0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (chessBoard[i][j] == COMPTER)
				computerValue += score({ i,j }, COMPTER, mark);
			else if (chessBoard[i][j] == HUMAN)
				humanValue += score({ i,j }, HUMAN, mark);
		}
	}
	return computerValue - humanValue;
}

/*inline long GoBang::wholeValue(point remove,char name, point add, long curValue)
{
	char addName = chessBoard[add.x][add.y];
	chessBoard[add.x][add.y] = ' ';
	long value = curValue;
	dirmark changeList(59,hasher);
	for (int i = 1; i <= 4; i++)
	{
		direction d = mapping(i);
		point tempPoint = nextPoint(remove, d, 1);
		vector<bool> temp(5, true);
		for (int j = 1; j <= 4; j++)
		{
			if (j != i)
				continue;
			temp[j] = false;
		}
		while (isInBoard(tempPoint))
		{
			if (!isEmpty(tempPoint.x, tempPoint.y))
			{
				changeList.insert(make_pair(tempPoint, temp));
			}
			tempPoint = nextPoint(tempPoint, d, 1);
		}
		tempPoint = nextPoint(remove, d, -1);
		while (isInBoard(tempPoint))
		{
			if (!isEmpty(tempPoint.x, tempPoint.y))
			{
				changeList.insert(make_pair(tempPoint, temp));
			}
			tempPoint = nextPoint(tempPoint, d, -1);
		}
	}
	auto tempList = changeList;
	chessBoard[remove.x][remove.y] = name;
	if (name == HUMAN)
		value += score(remove, HUMAN,changeList);
	else
	{
		value -= score(remove, COMPTER,changeList);
	}
	for (auto node : changeList)
	{
		if (chessBoard[node.first.x][node.first.y] == HUMAN)
			value += score(node.first, HUMAN,changeList);
		else
		{
			value -= score(node.first, COMPTER,changeList);
		}
	}
	chessBoard[remove.x][remove.y] = ' ';
	for (auto node : tempList)
	{
		if (chessBoard[node.first.x][node.first.y] == HUMAN)
			value -= score(node.first, HUMAN, tempList);
		else
		{
			value += score(node.first, COMPTER, tempList);
		}
	}
	dirmark mark(59,hasher);
	mark[add] = vector<bool>(5, false);
	for (int i = 1; i <= 4; i++)
	{
		vector<bool> temp(5, true);
		for (int j = 1; j <= 4; j++)
		{
			if (j != i)
				continue;
			temp[j] = false;
		}
		direction d = mapping(i);
		point tempPoint = nextPoint(add, d, 1);
		while (isInBoard(tempPoint))
		{
			if (!isEmpty(tempPoint.x, tempPoint.y))
			{
				mark.insert(make_pair(tempPoint, temp));
			}
			tempPoint = nextPoint(tempPoint, d, 1);
		}
		tempPoint = nextPoint(add, d, -1);
		while (isInBoard(tempPoint))
		{
			if (!isEmpty(tempPoint.x, tempPoint.y))
			{
				mark.insert(make_pair(tempPoint, temp));
			}
			tempPoint = nextPoint(tempPoint, d, -1);
		}
	}
	/*dirmark repeat(59, hasher);
	for (auto node : changeList)   //计算重复点，目的是避免重复计算结点分值
	{
		if (mark.find(node.first) != mark.end())
		{
			repeat.insert(*mark.find(node.first));
		}
	}
	for (auto node : repeat)  //删除重复计算点的值 切记不能和上面的合并
	{
		if (chessBoard[node.first.x][node.first.y] == HUMAN)
			value += score(node.first, HUMAN, repeat);
		else
		{
			value -= score(node.first, COMPTER, repeat);
		}
	}
	chessBoard[add.x][add.y] = addName;
	for (auto node : mark)
	{
		if (chessBoard[node.first.x][node.first.y] == HUMAN)
			value -= score(node.first, HUMAN, mark);
		else
		{
			value += score(node.first, COMPTER, mark);
		}
	}
	return value;
}*/

inline long GoBang::wholeValue(long preValue, point addPoint)
{
	dirmark changeList(59, hasher);
	for (int i = 1; i <= 4; i++)
	{
		direction d = mapping(i);
		point tempPoint = nextPoint(addPoint, d, 1);
		vector<bool> temp(5, true);
		for (int j = 1; j <= 4; j++)
		{
			if (j != i)
				continue;
			temp[j] = false;
		}
		while (isInBoard(tempPoint))
		{
			if (!isEmpty(tempPoint.x, tempPoint.y))
			{
				changeList.insert(make_pair(tempPoint, temp));
			}
			tempPoint = nextPoint(tempPoint, d, 1);
		}
		tempPoint = nextPoint(addPoint, d, -1);
		while (isInBoard(tempPoint))
		{
			if (!isEmpty(tempPoint.x, tempPoint.y))
			{
				changeList.insert(make_pair(tempPoint, temp));
			}
			tempPoint = nextPoint(tempPoint, d, -1);
		}
	}
	char name = chessBoard[addPoint.x][addPoint.y];
	chessBoard[addPoint.x][addPoint.y] = ' ';
	auto copyList = changeList;
	for (auto node : changeList)
	{
		if (chessBoard[node.first.x][node.first.y] == HUMAN)
			preValue += score(node.first, HUMAN, changeList);
		else
		{
			preValue -= score(node.first, COMPTER, changeList);
		}
	}
	copyList.insert(make_pair(addPoint, vector<bool>(5, false)));
	chessBoard[addPoint.x][addPoint.y] = name;
	for (auto node : copyList)
	{
		if (chessBoard[node.first.x][node.first.y] == HUMAN)
			preValue -= score(node.first, HUMAN, copyList);
		else
		{
			preValue += score(node.first, COMPTER, copyList);
		}
	}
	return preValue;
}

inline bool GoBang::isInBoard(point p)
{
	if (p.x >= 0 && p.x <= 14 && p.y >= 0 && p.y <= 14)
		return true;
	else
		return false;
}

inline GoBang::direction GoBang::mapping(int i)
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

void GoBang::getLinkPiece(int & length, point & linkPoint, int name, point p, direction d, int pn)
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

void GoBang::setDirMark(point p, char name,int dir, direction d,dirmark& mark)
{
	point tempPoint = nextPoint(p, d, -1);
	while (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] == name)
	{
		if (mark.find(tempPoint) != mark.end())
			mark[tempPoint][dir] = true;
		else
		{
			mark.insert(make_pair(tempPoint,vector<bool>(5, false)));
			mark[tempPoint][dir]=true;
		}
		tempPoint = nextPoint(tempPoint, d, -1);
	}
	tempPoint = nextPoint(p, d, 1);
	while (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] == name)
	{
		if (mark.find(tempPoint) != mark.end())
			mark[tempPoint][dir] = true;
		else
		{
			mark.insert(make_pair(tempPoint, vector<bool>(5, false)));
			mark[tempPoint][dir] = true;
		}
		tempPoint = nextPoint(tempPoint, d, 1);
	}
}

void GoBang::getBoundary(char * left, point le, char * right, point ri, direction d, char name)
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
	if (length == 5)
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


/*整个程序的关键部分 AI智能的来源 主要利用极大极小值算法加上动态规划，α-β裁剪 (其余四个对称函数就不再进行重复解释）*/
tuple<long,bool,bool> GoBang::findCompMove(point  &bestMove, int deep,int endDeep, int alpha, int beta,point preComMove,point preHumMove)
{
	if (endDeep > maxDeep-1)
		maxDeep = endDeep-1;
	endTime = clock();
	if (endTime - beginTime >= 1500)
		return { 0,false,true };
	point dc;
	long responseValue;
	long value;
	long preValue;
	bool isbreak = false;
	//bool reBreak = false;
	//priority_queue<waitPoint, vector<waitPoint>, cmp> queue;
	if (fullBoard())
		return { wholeValue(),false,false };
	if (deep == endDeep)
		return { wholeValue(),false,false };
	else
	{
		set<point> m_temp = inspireFind(preComMove, preHumMove);  //搜索可能的落子位置
		vector<tuple<point, long,bool>> temp;
		for (auto node : m_temp)
		{
			bool hasddie4 = false;
			long pointValue = score(node, COMPTER, hasddie4);
			temp.push_back(make_tuple(node, pointValue, hasddie4));
		}
		auto com = bind(&GoBang::compareFuc, this, _1, _2);
		sort(temp.begin(), temp.end(), com);
		value = alpha; bestMove = get<0>(temp[0]);
		if (deep == endDeep - 1)
			preValue = wholeValue();
		for (auto p : temp)
		{
			if (value >= beta)   //α裁剪 AI性能提升的关键1
			{
				isbreak = true;
				break;
			}
			if (isEmpty(get<0>(p).x, get<0>(p).y))
			{
				if (get<1>(p) >= 10000000)
				{
					bestMove = get<0>(p);
					return { 10000000,false,false };
				}
				int nextEndDeep;
				if (get<2>(p)&&endDeep<=10&&extension)  //冲四外延2层
					nextEndDeep = endDeep+2;
				else
					nextEndDeep = endDeep;
				place(get<0>(p).x, get<0>(p).y, COMPTER);
				if (zobristMap.find(zobristKey) != zobristMap.end())
				{
					responseValue = zobristMap[zobristKey];
					number++;
				}
				else
				{
					tuple<long,bool,bool> reTuple; //2: isbreak 3 hasddie4
					if (deep != nextEndDeep - 1)
						reTuple = findHumanMove(dc, deep + 1, nextEndDeep, value, beta, get<0>(p),preHumMove);
					else
						reTuple = { wholeValue(preValue,get<0>(p)),false,false };
					//reBreak = get<1>(reTuple);
					if (get<2>(reTuple))
					{
						unPlace(get<0>(p).x, get<0>(p).y);
						return reTuple;
					}
					responseValue = get<0>(reTuple);
					if(!get<1>(reTuple))
						zobristMap.insert(make_pair(zobristKey, responseValue));
				}
				unPlace(get<0>(p).x, get<0>(p).y);         //取子
				/*if (deep == 1&&!reBreak)             //顶端findCompMove采用比较然后随机选取的方法
					queue.push(waitPoint(responseValue, { get<0>(p).x,get<0>(p).y }));*/
				if (responseValue > value)  //其余全部的findComMove只存放对自己最有利的棋子位置
				{
					value = responseValue;
					bestMove = { get<0>(p).x,get<0>(p).y };
				}
				if (responseValue >= 10000000)
					return { value,false,false };
			}
		}
	}
	//if (deep == 1)
		//intelligentRandom(bestMove, value, queue);
	return { value,isbreak,false };
}

tuple<long,bool,bool> GoBang::findHumanMove(point & bestMove, int deep,int endDeep, int alpha, int beta,point preComMove,point preHumMove)
{
	if (endDeep > maxDeep-1)
		maxDeep = endDeep-1;
	endTime = clock();
	if (endTime - beginTime >= 1500)
		return { 0,false,true };
	point dc;
	long responseValue;
	long value;
	long preValue;
	bool isbreak = false;
	if (fullBoard())
		return { wholeValue(),false,false };
	if (deep == endDeep)
		return { wholeValue(),false,false };
	else
	{
		set<point> m_temp = inspireFind(preHumMove, preComMove);  //搜索可能的落子位置
		vector<tuple<point, long,bool>> temp;
		for (auto node : m_temp)
		{
			bool hasddie4 = false;
			long pointValue = score(node, HUMAN, hasddie4);
			temp.push_back(make_tuple(node, pointValue,hasddie4));
		}
		auto com = bind(&GoBang::compareFuc, this, _1, _2);
		sort(temp.begin(), temp.end(), com);
		value = beta; bestMove = get<0>(temp[0]);
		if (deep == endDeep - 1)
			preValue = wholeValue();
		for (auto p : temp)
		{
			if (get<1>(p) <= -10000000)
			{
				bestMove = get<0>(p);
				return { -10000000,false,false };
			}
			if (alpha >= value)    //β裁剪 AI性能提升的关键2
			{
				isbreak = true;
				break;
			}
			if (isEmpty(get<0>(p).x, get<0>(p).y))
			{
				int nextEndDeep;
				if (get<2>(p)&&endDeep<=10&&extension)  //冲四外延2层
					nextEndDeep = endDeep+2;
				else
					nextEndDeep = endDeep;
				place(get<0>(p).x, get<0>(p).y, HUMAN);
				if (zobristMap.find(zobristKey) != zobristMap.end())
				{
					responseValue = zobristMap[zobristKey];
					number++;
				}
				else
				{
					tuple<long, bool,bool> reTuple;
					if (deep != nextEndDeep - 1)
						reTuple = findCompMove(dc, deep + 1, nextEndDeep, alpha, value,preComMove, get<0>(p));
					else
						reTuple = { wholeValue(preValue,get<0>(p)),false,false };
					responseValue = get<0>(reTuple);
					if (get<2>(reTuple))
					{
						unPlace(get<0>(p).x, get<0>(p).y);
						return reTuple;
					}
					if(!get<1>(reTuple))
						zobristMap.insert(make_pair(zobristKey, responseValue));
				}
				unPlace(get<0>(p).x, get<0>(p).y);
				if (responseValue < value)
				{
					value = responseValue;
					bestMove = { get<0>(p).x,get<0>(p).y };
				}
				if (responseValue <= -10000000)
					return { value,false,false };
			}
		}
	}
	return { value,isbreak,false };
}

point GoBang::iterationDeep(point preCom,point preHum)
{
	long value;
	point bestMove;
	int curMaxDeep;
	if (points.size() <= 8)
		curMaxDeep = 6;
	else
		curMaxDeep = 6;
	beginTime = clock();
	for (int i = 2; i <= curMaxDeep; i = i + 2)
	{
		if (i == 6)
			extension = true;
		else
			extension = false;
		maxDeep = i;
		point movePoint;
		zobristMap.clear();
		auto reTuple=findCompMove(movePoint, 1, i+1, -100000000, 100000000,preCom,preHum);
		if (get<2>(reTuple))
			break;
		value = get<0>(reTuple);
		bestMove = movePoint;
		if (value >= COMPTER_WIN)
			break;
	}
	return bestMove;
}

inline void GoBang::place(int x, int y, char name)
{
	chessBoard[x][y] = name;
	prieceNumber++;
	if (name == COMPTER)
		zobristKey ^= randomArray[x][y][1];
	else
		zobristKey ^= randomArray[x][y][2];
}

inline void GoBang::unPlace(int x, int y)
{
	if (chessBoard[x][y] == COMPTER)
		zobristKey ^= randomArray[x][y][1];
	else
		zobristKey ^= randomArray[x][y][2];
	chessBoard[x][y] = ' ';
	prieceNumber--;
}

 bool GoBang::regretChess()
{
	 if (points.empty())
		 return false;
	for (int i = 1; i <= 2; i++)
	{
		unPlace(points.top().first.x, points.top().first.y);
		points.pop();
	}
	currentValue = points.top().second;
	system("cls");
	printChess();
	return true;
}

inline bool GoBang::judge(point p, char name)
{
	int i = p.x;
	int j = p.y;
	if (!isEmpty(i, j) && chessBoard[i][j] == name)
	{
		for (int z = 1; z <= 4; z++)
		{
			direction d = mapping(z);
			int length = 1;
			point le, ri;
			getLinkPiece(length, le, name, { i,j }, d, -1);
			getLinkPiece(length, ri, name, { i,j }, d, 1);
			if (length >= 5)
				return true;
		}
	}
	return false;
}

/*启发式搜索 AI性能提升的关键3*/
set<point> GoBang::inspireFind(point preMePoint, point preHePoint)
{
	sum me, he;
	sum temp;
	bool flag = true;
	char name = chessBoard[preHePoint.x][preHePoint.y];
	set<point> Points;
	if (isInBoard(preMePoint))
	{
		chessBoard[preHePoint.x][preHePoint.y] = ' ';
		for (int i = 1; i <= 4; i++)
		{
			direction d = mapping(i);
			int length = 1;
			point le, ri, tempPoint;
			char left[5], right[5];
			getLinkPiece(length, le, chessBoard[preMePoint.x][preMePoint.y], preMePoint, d, -1);
			getLinkPiece(length, ri, chessBoard[preMePoint.x][preMePoint.y], preMePoint, d, 1);
			getBoundary(left, le, right, ri, d, chessBoard[preMePoint.x][preMePoint.y]);
			temp = situaltionAnalysis(length, chessBoard[preMePoint.x][preMePoint.y], left, right);
			me += temp;
		}
		chessBoard[preHePoint.x][preHePoint.y] = name;
		if (me.alive2>=1||me.alive3 >= 1||me.alive4>=1 || me.dalive4 >= 1)
		{
			for (int i = 1; i <= 4; i++)
			{
				direction d = mapping(i);
				point tempPoint = nextPoint(preMePoint, d, 1);
				while (isInBoard(tempPoint))
				{
					if (isEmpty(tempPoint.x, tempPoint.y) && hasNeighbor(tempPoint))
					{
						Points.insert(tempPoint);
					}
					tempPoint = nextPoint(tempPoint, d, 1);
				}
				tempPoint = nextPoint(preMePoint, d, -1);
				while (isInBoard(tempPoint))
				{
					if (isEmpty(tempPoint.x, tempPoint.y)&&hasNeighbor(tempPoint))
					{
						Points.insert(tempPoint);
					}
					tempPoint = nextPoint(tempPoint, d, -1);
				}
			}
			flag = false;
		}
	}
	if (isInBoard(preHePoint))
	{
		bool isempty = points.empty();
		for (int i = 1; i <= 4; i++)
		{
			direction d = mapping(i);
			int length = 1;
			point le, ri, tempPoint;
			char left[5], right[5];
			getLinkPiece(length, le, chessBoard[preHePoint.x][preHePoint.y], preHePoint, d, -1);
			getLinkPiece(length, ri, chessBoard[preHePoint.x][preHePoint.y], preHePoint, d, 1);
			getBoundary(left, le, right, ri, d, chessBoard[preHePoint.x][preHePoint.y]);
			temp = situaltionAnalysis(length, chessBoard[preHePoint.x][preHePoint.y], left, right);
			if ((temp.alive3 >= 1 || temp.alive4 >= 1 || temp.dalive4 >= 1)&&isempty)
			{
				direction d = mapping(i);
				point tempPoint = nextPoint(preHePoint, d, 1);
				while (isInBoard(tempPoint))
				{
					if (isEmpty(tempPoint.x, tempPoint.y) && hasNeighbor(tempPoint,name,i))
					{
						Points.insert(tempPoint);
					}
					tempPoint = nextPoint(tempPoint, d, 1);
				}
				tempPoint = nextPoint(preHePoint, d, -1);
				while (isInBoard(tempPoint))
				{
					if (isEmpty(tempPoint.x, tempPoint.y) && hasNeighbor(tempPoint,name,i))
					{
						Points.insert(tempPoint);
					}
					tempPoint = nextPoint(tempPoint, d, -1);
				}
			}
			he += temp;
		}
		if (isempty&&!Points.empty())
			return Points;
		if (he.alive2>=1||he.alive3 >= 1 || he.alive4 >= 1 || he.dalive4 >= 1)
		{
			for (int i = 1; i <= 4; i++)
			{
				direction d = mapping(i);
				point tempPoint = nextPoint(preHePoint, d, 1);
				while (isInBoard(tempPoint))
				{
					if (isEmpty(tempPoint.x, tempPoint.y) && hasNeighbor(tempPoint))
					{
						Points.insert(tempPoint);
					}
					tempPoint = nextPoint(tempPoint, d, 1);
				}
				tempPoint = nextPoint(preHePoint, d, -1);
				while (isInBoard(tempPoint))
				{
					if (isEmpty(tempPoint.x, tempPoint.y) && hasNeighbor(tempPoint))
					{
						Points.insert(tempPoint);
					}
					tempPoint = nextPoint(tempPoint, d, -1);
				}
			}
			flag = false;
		}
	}
	if(flag)
	{
		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				if (isEmpty(i, j) && hasNeighbor({ i,j }))  //只取两步之内有邻居的结点位置
					Points.insert({ i,j });
	}
	return Points;
}

set<point> GoBang::inspireFind()
{
	set<point> Points;
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			if (isEmpty(i, j) && hasNeighbor({ i,j }))  //只取两步之内有邻居的结点位置
				Points.insert({ i,j });
	return Points;
}

bool GoBang::Fobidden()
{
	for(int i=0;i<15;i++)
		for (int j = 0; j < 15; j++)
		{
			if (isEmpty(i, j) && hasNeighbor({ i,j }, COMPTER))
			{
				place(i, j, COMPTER);
				for (int z = 1; z <= 4; z++)
				{
					direction d = mapping(z);
					int length = 1;
					point le, ri, tempPoint;
					getLinkPiece(length, le, COMPTER, { i,j }, d, -1);
					getLinkPiece(length, ri, COMPTER, { i,j }, d, 1);
					if (length >= 5)
						return true;
				}

				unPlace(i, j);
			}
		}
	return false;
}

bool GoBang::hasNeighbor(point p,int dir)
{
	if (dir == 0)
	{
		for (int i = 1; i <= 4; i++)
		{
			direction d = mapping(i);
			for (int j = 1; j <= 1; j++)
			{
				point tempPoint = nextPoint(p, d, j);
				if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] != ' ')
					return true;
				tempPoint = nextPoint(p, d, -j);
				if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] != ' ')
					return true;
			}
		}
	}
	else
	{
		direction d = mapping(dir);
		for (int j = 1; j <= 1; j++)
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

bool GoBang::hasNeighbor(point p, char name,int dir)
{
	if (dir == 0)
	{
		for (int i = 1; i <= 4; i++)
		{
			direction d = mapping(i);
			for (int j = 1; j <= 2; j++)
			{
				point tempPoint = nextPoint(p, d, j);
				if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] == name)
					return true;
				tempPoint = nextPoint(p, d, -j);
				if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] == name)
					return true;
			}
		}
	}
	else
	{
		direction d = mapping(dir);
		for (int i = 1; i <= 2; i++)
		{
			point tempPoint = nextPoint(p, d, i);
			if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] == name)
				return true;
			tempPoint = nextPoint(p, d, -i);
			if (isInBoard(tempPoint) && chessBoard[tempPoint.x][tempPoint.y] == name)
				return true;
		}
	}
	return false;
}

/*避免AI的棋法单一*/
inline void GoBang::intelligentRandom(point & bestMove, long value, priority_queue<waitPoint, vector<waitPoint>, cmp> &queue)
{
	waitPoint first = queue.top();
	vector<waitPoint> waitPoints;
	while (!queue.empty()&&queue.top().value <= first.value + 300)  //如果差太多了就取较大的
	{
		waitPoints.push_back(queue.top());
		queue.pop();
	}
	default_random_engine e(time(NULL));
	uniform_int_distribution<int> u(0, waitPoints.size() - 1);
	bestMove = waitPoints[u(e)].p;
}

bool GoBang::compareFuc(const tuple<point,long,bool>& p1,const tuple<point,long,bool>& p2)
{
	return get<1>(p1) > get<1>(p2);
}

#endif
