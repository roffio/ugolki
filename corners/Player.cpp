#include "Player.h"


Player::Player(brd& setupBoard, CellType setupType, std::string setupName,
	std::vector<std::string>* setupList) :localBoard(setupBoard) {
	localType = setupType;
	name = setupName;
	localEvList = setupList;
}

CellType Player::getType() {
	return localType;
}

std::string Player::getName(){
	return this->name;
}

void Player::getMoves(mv& moves, int startingX, int startingY, int iterations) {
	if (iterations > 4) return;
	for (int x = startingX - 2; x <= startingX + 2; x++)
	for (int y = startingY - 2; y <= startingY + 2; y++) {
		if (x < 0 || y < 0 || x > 7 || y > 7) continue;
		if (localBoard[x][y] != Empty) continue;
		int dx = std::abs(startingX - x);
		int dy = std::abs(startingY - y);
		if ((dx == 1 && dy == 0 || dx == 0 && dy == 1) 
			&& iterations == 0) {
			moves.push_back(std::make_pair(x, y));
		}
		else if (dx == 2 && dy == 0) {
			int sign = (startingX - x) / dx;
			if (localBoard[x + sign][y] != Empty) {
				bool flag = false;

				if (iterations > 0) for (int i = 0; i < moves.size(); i++) {
					if (moves[i].first == x && moves[i].second == y) {
						flag = true;
						break;
					}
				}  if (flag) continue;

				moves.push_back(std::make_pair(x, y));
				getMoves(moves, x, y, iterations + 1);
			}
		}
		else if (dx == 0 && dy == 2) {
			int sign = (startingY - y) / dy;
			if (localBoard[x][y + sign] != Empty) {
				bool flag = false;

				if (iterations > 0) for (int i = 0; i < moves.size(); i++) {
					if (moves[i].first == x && moves[i].second == y) {
						flag = true;
						break;
					}
				}

				if (flag) continue;

				moves.push_back(std::make_pair(x, y));
				getMoves(moves, x, y, iterations + 1);
			}
		}
	}
}

void Player::chooseMove(mv& movesVec, int& x, int& y) {
	int choise;
	std::cout << "-1. Вернуться к выбору\n";
	for (int i = 0; i < movesVec.size(); i++) {
		std::cout << i << ". (" << movesVec[i].first << ',' << movesVec[i].second << ")\n";
	}
	std::cout << "Выберите ход из предложеных: ";
	std::cin >> choise;

	if (choise == -1) {
		selectPieces();
		return;
	}

	if (choise >= movesVec.size() || choise < -1) {
		std::cout << "Такого варианта нет. Попробуйте ещё раз.\n";
		chooseMove(movesVec, x, y);
		return;
	}
	std::pair<int, int> chosenPair = movesVec[choise];

	localBoard[chosenPair.first][chosenPair.second] = localType;
	localBoard[x][y] = Empty;
}

void Player::selectPieces() {

	localEvList->push_back("makeMove");
	char evChar = char(localType);
	localEvList->push_back(std::string(1, evChar));

	while (true)
	{
		if (localEvList->size() == 0 || localEvList->at(0) != "made_move_1") continue;

		std::cout << localEvList->at(0);
		localEvList->clear();
		std::wcout << "got\n";
		break;
		
	}
}

void Player::makeMove() {
	// This function is devided into multiple other functions
	// to prevent loop nesting 
	selectPieces();
}

Player& Player::operator= (const Player& p1) {
	this->localBoard = p1.localBoard;
	this->localType = p1.localType;
	return *this;
}

bool Player::operator== (const Player& p1) {
	if (this->localBoard == p1.localBoard && this->localType == p1.localType)
		return true;
	else
		return false;
}