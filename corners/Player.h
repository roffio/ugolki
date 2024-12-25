#pragma once

#include "CellType.h"

#include <vector>
#include <iostream>
#include <memory>
#include <string>

typedef std::vector<std::vector<CellType>> brd;
typedef std::vector <std::pair<int, int>> mv;

class Player
{
private:
	std::string name;
	brd& localBoard;
	std::vector<std::string>* localEvList;
	CellType localType;
	void selectPieces();
	void chooseMove(mv& movesVec, int& x, int& y);
public:
	void getMoves(mv& moves, int startingX, int startingY, int iterations = 0);
	bool isAi = false;
	Player(brd& setupBoard, CellType setupType, std::string setupName,
		std::vector<std::string>* setupList = nullptr);
	virtual ~Player() = default;
	std::string getName();
	virtual void makeMove();
	CellType getType();

	Player& operator= (const Player& p);
	bool operator== (const Player& p1);
};

