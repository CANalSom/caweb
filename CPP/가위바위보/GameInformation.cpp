#include "GameInformation.h"
#include <iostream>

GameInformation::GameInformation()
{
	m_userWin = 0;
	m_computerWin = 0;
	m_Draw = 0;
}

void GameInformation::userWin()
{
	m_userWin++;
}

void GameInformation::computerWin()
{
	m_computerWin++;
}

void GameInformation::allDraw()
{
	m_Draw++;
}

void GameInformation::printGame()
{
	printf("전체 [ %d ] : 사용자 [ %d ] 컴퓨터 [ %d ] 비김 [ %d ]\n", m_userWin + m_computerWin + m_Draw, m_userWin, m_computerWin, m_Draw);
	printf("나의 승률 : %.1f%%\n\n", (float)m_userWin / (m_userWin + m_computerWin + m_Draw) * 100);
	printf("*********************************************************************\n");
}
