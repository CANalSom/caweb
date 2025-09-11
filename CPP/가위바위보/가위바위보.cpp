#include <iostream>
#include "Game.h"
#include <time.h>

using namespace std;

int main()
{
	int user = 0;
	int computer = 0;
	int result = 0;
	int count = 0;
	
	Game game;

	while (true)
	{
		cout << "가위 (0), 바위 (1), 보(2) 중 하나를 선택하여 입력하세요 : ";
		cin >> user;

		computer = game.normalGame();

		result = game.GetGameResult(user, computer);

		printf("사용자 : %d 컴퓨터 %d\n", user, computer);

		if (user >= 3)
		{
			cout << "프로그램 종료" << endl;
			break;
		}

		switch (result)
		{
		case 0:
			cout << "결과 : 당신이 이겼습니다.\n" << endl;
			game.userWin();
			break;
		case 1:
			cout << "결과 : 당신이 졌습니다.\n" << endl;
			game.computerWin();
			break;
		case 2:
			cout << "결과 : 당신이 비겼습니다.\n" << endl;
			game.allDraw();
			break;
		}
		game.printGame();
	}
	
}