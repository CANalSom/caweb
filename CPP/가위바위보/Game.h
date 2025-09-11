#pragma once

#include "GameInformation.h"
#include "GameBoard.h";
#include "Random.h"

class Game : public GameBoard, public GameInformation
{
public:
	Game();
	int normalGame();
private:
	Random m_random;
};

