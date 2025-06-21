#pragma once

class GameInformation
{
public:
	GameInformation();
	void userWin();
	void computerWin();
	void allDraw();
	void printGame();

	int m_userWin;
	int m_computerWin;
	int m_Draw;
};

