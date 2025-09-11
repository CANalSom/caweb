#pragma once

class GameBoard
{
public:
	int m_board[3][3];
	GameBoard();
	int GetGameResult(int user, int computer);
};

