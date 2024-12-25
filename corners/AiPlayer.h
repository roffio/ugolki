#pragma once
#include "Player.h"
#include <random>
#include <tuple>

class AiPlayer : public Player
{
private:
	int moveCount;
	std::string name;
	brd& localBoard;
	CellType localType;
	void getMoves(mv& moves, int startingX, int startingY, int iterations = 0);
	std::tuple<int, int, int> evaluator(brd& lBoard);
	void makeBrdSimular(brd& wrongBrd, brd& exampleBrd);
	int countHomePieces();
	int evaluatePosition(brd& lBoard);
public:
	bool isAi = true;
	AiPlayer(brd& setupBoard, CellType setupType, std::string setupName);
	void makeMove() override;
	void makeRndMove();
};

