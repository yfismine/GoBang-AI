#include"GoBang.h"
#include<iostream>
#include<time.h>
using namespace std;
int main()
{
	int x, y;
	cout << "\t\t\t  **********************************************************" << endl;
	cout << "\t\t\t  *                                                        *" << endl;
	cout << "\t\t\t  *                 五子棋大师（规则）                     *" << endl;
	cout << "\t\t\t  *      1：任意一方达成五子连线即可胜利                   *" << endl;
	cout << "\t\t\t  *      2：玩家先行，但是玩家有简单的禁手规则             *" << endl;
	cout << "\t\t\t  *      3：禁手规则：如果玩家达成五子连线的同时           *" << endl;
	cout << "\t\t\t  *         电脑通过一步也可以达成五子连线则判定电脑获胜   *" << endl;
	cout << "\t\t\t  *      4:全屏游戏效果更佳                                *" << endl;
	cout << "\t\t\t  *                                                        *" << endl;
	cout << "\t\t\t  *                TimeConsume:3 d     by:yf    2019.7.24  *" << endl;
	cout << "\t\t\t  *                                                        *" << endl;
	cout << "\t\t\t  *                Last Update:2020.3.18                   *" << endl;
	cout << "\t\t\t  **********************************************************" << endl;
	cout << endl << endl;
	cout << "\t\t\t\t\t\t任意键开始游戏";
	getchar();
	system("cls");
	GoBang board;
	while (1)
	{
		cout << "请输入落子坐标(x或者y输入零视为悔棋）" << endl;
		cin >> x >> y;
		try
		{
			if (x == 0 || y == 0)
			{
				if (!board.regretChess())
					cout << "当前无法悔棋，请重新输入落子坐标" << endl;
				else
				{
					cout << "悔棋成功" << endl;
				}
			}
			else
				board.addPiece(x, y);
		}
		catch (out_of_range &e)
		{
			cout << e.what() << endl;
			cout << "请重新输入落子坐标" << endl;
		}
		catch (runtime_error &e)
		{
			cout << e.what() << endl;
			cout << "请重新输入落子坐标" << endl;
		}
		catch (int &result)
		{
			if (result == DRAW)
			{
				cout << "游戏结束：平局" << endl;
				break;
			}
			if (result == COMPTER_WIN)
			{
				cout << "游戏结束：电脑获胜" << endl;
				break;
			}
			if (result == COMPTER_LOSS)
			{
				cout << "游戏结束：玩家获胜" << endl;
				break;
			}
			if (result == FORBIDDEN)
			{
				cout << "玩家犯规 触发禁手" << endl;
				cout << "游戏结束：电脑获胜" << endl;
				break;
			}
		}
	}
	system("pause");
	return 0;
}
