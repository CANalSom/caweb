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
	printf("��ü [ %d ] : ����� [ %d ] ��ǻ�� [ %d ] ��� [ %d ]\n", m_userWin + m_computerWin + m_Draw, m_userWin, m_computerWin, m_Draw);
	printf("���� �·� : %.1f%%\n\n", (float)m_userWin / (m_userWin + m_computerWin + m_Draw) * 100);
	printf("*********************************************************************\n");
}
