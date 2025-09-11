#include "GameBoard.h"
///          °¡À§(0) ¹ÙÀ§(1) º¸(2)
// °¡À§(0) : ºñ±è2     Áü1   ÀÌ±è0
// ¹ÙÀ§(1) : ÀÌ±è0   ºñ±è2     Áü1
//   º¸(2) :   Áü1   ÀÌ±è0   ºñ±è2

GameBoard::GameBoard()
{
	m_board[0][0] = 2;
	m_board[0][1] = 1;
	m_board[0][2] = 0;

	m_board[1][0] = 0;
	m_board[1][1] = 2;
	m_board[1][2] = 1;

	m_board[2][0] = 1;
	m_board[2][1] = 0;
	m_board[2][2] = 2;
}

int GameBoard::GetGameResult(int user, int computer)
{
	return m_board[user][computer];
}