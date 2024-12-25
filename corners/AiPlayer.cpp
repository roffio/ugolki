#include "AiPlayer.h"

AiPlayer::AiPlayer(brd& setupBoard, CellType setupType, std::string setupName) :
	Player(setupBoard, setupType, setupName), localBoard(setupBoard){
	localType = setupType;
	name = setupName;
	moveCount = 0;
}

void AiPlayer::getMoves(mv& moves, int startingX, int startingY, int iterations) {
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

void AiPlayer::makeBrdSimular(brd& wrongBrd, brd& exampleBrd) {
	// This is NOT optimized, don't use vectors this much when dealing
	// with constant size arrays
	wrongBrd.clear();
	for (int x = 0; x < 8; x++) {
		std::vector<CellType> tempBrd;
		for (int y = 0; y < 8; y++) {
			tempBrd.push_back(exampleBrd[x][y]);
		}
		wrongBrd.push_back(tempBrd);
	}
}

int isEndNow2(brd& localBoard, CellType whosAsking, int moveCount) {
	// 1 = win; -1 = loss; 0 = draw; 10 = no wincon met

	if (moveCount > 80) {
		int whiteCount = 0;
		int blackCount = 0;
		for (int x = 0; x < localBoard.size(); x++)
			for (int y = 0; y < localBoard.size(); y++) {
				if (x < 3 && y < 3 && localBoard[x][y] == White)
					whiteCount++;
				if (x > 4 && y > 4 && localBoard[x][y] == Black)
					blackCount++;
			}
		if (whiteCount > blackCount)
			return (whosAsking == White) ? 1 : -1;
		if (whiteCount < blackCount)
			return (whosAsking == White) ? 1 : -1;
		return 0;
	}
	if (moveCount > 40) {
		bool whiteLost = false;
		bool blackLost = false;
		for (int x = 0; x < localBoard.size(); x++)
			for (int y = 0; y < localBoard.size(); y++) {
				if (x < 3 && y < 3 && localBoard[x][y] == Black)
					blackLost = true;
				if (x > 4 && y > 4 && localBoard[x][y] == Black)
					whiteLost = true;
			}
		if (whiteLost && blackLost) return 0;
		if (whiteLost)
			return (whosAsking == White) ? -1 : 1;
		if (blackLost)
			return (whosAsking == Black) ? -1 : 1;

	}
	return 10;
}

int AiPlayer::countHomePieces() {
	int p = 0;
	if (localType == Black) {
		for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			if (localBoard[i][j] == Black)
				p += 100;
		}
	}
	if (localType == White) {
		for (int i = 5; i < 8; i++)
		for (int j = 5; j < 8; j++) {
			if (localBoard[i][j] == White)
				p += 100;
		}
	}
	return p;
}

int AiPlayer::evaluatePosition(brd& lBoard) {
	int score = 0;
	score -= countHomePieces();
	AiPlayer* localPlayerWhite = new AiPlayer(lBoard, White, "mrWhite");
	AiPlayer* localPlayerBlack = new AiPlayer(lBoard, Black, "mrBlack");
	AiPlayer* currPlayer = localPlayerWhite;
	int moveCount = 0;

	while (true)
	{
		currPlayer->makeRndMove();
		int ein = isEndNow2(lBoard, localType, moveCount);
		if (ein != 10) {
			score += ein * 100;
			break;
		}
		moveCount++;
		currPlayer = (currPlayer == localPlayerWhite) ? localPlayerBlack : localPlayerWhite;
	}

	delete currPlayer;
	delete localPlayerWhite;

	return score;
}

std::tuple<int, int, int> AiPlayer::evaluator(brd& lBoard) {
	int bestX, bestY, bestM;
	int iterations = 10000;
	int rating = -10000000; 

	brd evalBord;
	makeBrdSimular(evalBord, lBoard);
	
	for (int x = 0; x < 8; x++)
	for (int y = 0; y < 8; y++) {
		int localRating = 0;
		
		if (lBoard[x][y] != localType) continue;

		mv newMoves;

		getMoves(newMoves, x, y);

		for (int j = 0; j < newMoves.size(); j++) {
			for (int i = 0; i < iterations; i++) {


				evalBord[x][y] = Empty;
				evalBord[newMoves[j].first][newMoves[j].second] = localType;

				int e = evaluatePosition(evalBord);

				localRating += e;

				makeBrdSimular(evalBord, lBoard);
			}

			makeBrdSimular(evalBord, lBoard);
			if (localRating > rating) {
				rating = localRating;
				bestX = x;
				bestY = y;
				bestM = j;
			}
		}
	}

	return std::make_tuple(bestX, bestY, bestM);
}

void AiPlayer::makeMove() {
	moveCount++;
	std::tuple<int, int, int> bestMove = evaluator(localBoard);
	mv moves;

	getMoves(moves, std::get<0>(bestMove), std::get<1>(bestMove));

	localBoard[std::get<0>(bestMove)][std::get<1>(bestMove)] = Empty;
	localBoard[moves[std::get<2>(bestMove)].first][moves[std::get<2>(bestMove)].second] = localType;

}

void AiPlayer::makeRndMove() {
	mv moves;
	int x, y;
	while (true) {
		x = std::rand() % 8;
		y = std::rand() % 8;

		if (localBoard[x][y] != localType) {
			continue;
		}

		getMoves(moves, x, y);
		if (moves.size() > 0) break;
	}

	int chosenMove = rand() % (moves.size());

	localBoard[x][y] = Empty;
	localBoard[moves[chosenMove].first][moves[chosenMove].second] = localType;
}
