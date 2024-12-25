#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>

#include "CellType.h"
#include "Player.h"
#include "AiPlayer.h"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

void printBoard(brd localBoard) {
    std::cout << "  0 1 2 3 4 5 6 7\n";
    for (int i = 0; i < localBoard.size(); i++) {
        std::cout << i << ' ';
        for (int j = 0; j < localBoard[i].size(); j++) {
            std::cout << (char)localBoard[j][i] << ' ';
        }
        std::cout << std::endl;
    }
}

int isEndNow(brd& localBoard, CellType whosAsking, int moveCount) {
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

int visuals(brd& board, std::vector<std::string>& events) {

    bool shouldMakeMove = false;
    bool pickedFigure = false;
    CellType pickType = Empty;
    mv movesVec;

    int pickedFigureX, pickedFigureY;

    sf::RenderWindow window(sf::VideoMode(740, 740), "Ugolki", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::RectangleShape field;
    field.setSize(sf::Vector2f(720, 720));
    field.setPosition(sf::Vector2f(10, 10));
    field.setFillColor(sf::Color(100, 100, 100, 255));

    while (window.isOpen())
    {

        sf::Vector2i mpos = sf::Mouse::getPosition(window);
        int hoveredX = mpos.x / 87.5;
        int hoveredY = mpos.y / 87.5;


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (!(hoveredX >= 0 && hoveredX < 8 &&
                    hoveredY >= 0 && hoveredY < 8)) continue;


                for (int i = 0; movesVec.size() > 0 && i < movesVec.size(); i++) {
                    if (movesVec[i].first == hoveredX && movesVec[i].second == hoveredY &&
                        ((events.size() > 0 && events[0] != "made_move_1") || events.size() == 0)) {
                        board[hoveredX][hoveredY] = pickType;
                        board[pickedFigureX][pickedFigureY] = Empty;
                        shouldMakeMove = false;
                        pickedFigure = false;
                        pickType = Empty;
                        movesVec.clear();
                        events.push_back("made_move_1");
                        std::wcout << "sent\n";
                        for (auto i : events) {
                            std::cout << i << std::endl;
                        }
                        break;
                    }
                }

                if (board[hoveredX][hoveredY] != pickType || pickType == Empty) continue;
                pickedFigure = true;
                pickedFigureX = hoveredX;
                pickedFigureY = hoveredY;

                Player* tmpPlayer = new Player(board, pickType, "a");
                movesVec.clear();
                tmpPlayer->getMoves(movesVec, pickedFigureX, pickedFigureY);
                delete tmpPlayer;

            }
        }

        //field.move(sf::Vector2f(1, 0));

        window.clear(sf::Color(150, 150, 150, 255));
        window.draw(field);

        for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++) {

            bool isHovered = (x == hoveredX && y == hoveredY) ? true : false;
            
            sf::Color col1 = sf::Color(110, 100, 100, 255);
            sf::Color col2 = sf::Color(50 ,40, 40,255);
            sf::Color col3 = col2;
            int innerRadius = 15;

            if (board[x][y] == Black) { 
                std::swap(col1, col3);
                innerRadius -= 5;
            }

            // Field color
            sf::RectangleShape localRect;
            localRect.setSize(sf::Vector2f(87.5, 87.5));
            localRect.setPosition(sf::Vector2f(20 + x * 87.5, 20 + y * 87.5));
            if ((x + y) % 2 == 0)
                localRect.setFillColor(sf::Color(100, 0, 0, 255));
            else localRect.setFillColor(sf::Color(200, 200, 190, 255));

            if (isHovered) { 
                if ((x + y) % 2 == 0)
                    localRect.setFillColor(sf::Color(0, 0, 0, 150));
                else localRect.setFillColor(sf::Color(0, 0, 0, 150));
            }

            if (pickedFigure) {
                if (x == pickedFigureX && y == pickedFigureY)
                    localRect.setFillColor(sf::Color::Cyan);
            }

            window.draw(localRect);

            // Waypoints

            for (int i = 0; movesVec.size() > 0 && i < movesVec.size(); i++) {
                if (movesVec[i].first == x && movesVec[i].second == y) {
                    sf::CircleShape allowed;

                    //allowed.setFillColor(sf::Color::Green);
                    allowed.setOutlineColor(sf::Color(0, 150, 0, 255));
                    allowed.setFillColor(sf::Color::Transparent);
                    allowed.setOutlineThickness(10);
                    allowed.setRadius(87.5 / 8);
                    allowed.setPosition(sf::Vector2f(10 + x * 87.5 + 87.5/2, 10 + y * 87.5 + 87.5 / 2));

                    window.draw(allowed);
                    break;
                }
            }

            // Figure
            if (board[x][y] == Empty) continue;
            sf::CircleShape figure;
            figure.setFillColor(col1);
            figure.setOutlineColor(col2);
            figure.setOutlineThickness(7);
            figure.setRadius(87.5/2 - 7);
            figure.setPosition(sf::Vector2f(20 + x * 87.5 + 7, 20 + y * 87.5 + 7));
            window.draw(figure);
            // Figure inner shape
            figure.setFillColor(sf::Color(0,0,0,0));
            figure.setOutlineColor(col3);
            figure.setOutlineThickness(7);
            figure.setRadius(87.5 / 2 - 7 - innerRadius);
            figure.setPosition(sf::Vector2f(20 + x * 87.5 + 7 + innerRadius, 20 + y * 87.5 + 7 + innerRadius ));
            window.draw(figure);


        }

        window.display();

        // event dispatcher & wrapper

        if (events.size() == 0) continue;

        if (events.size() > 0 && events[0] == "makeMove") {
            pickType = Empty;
            events.erase(events.begin());
            shouldMakeMove = true;
            pickType = (events[0] == "o") ? White : Black;
            events.erase(events.begin());
            movesVec.clear();
            pickedFigure = false;
        }
    }

    return 0;
}


int main()
{
    setlocale(LC_ALL, "ru");


    // get player param's
    brd board;
    int boardSize = 8;
    int moveCount = 1;
    std::string name1, name2, entry1, entry2;

    Player* WhitePlayer;
    Player* BlackPlayer;

    std::vector<std::string> eventList;

    // Fill the board

    for (int i = 0; i < boardSize; i++) {
        std::vector<CellType> subBoard;
        for (int j = 0; j < boardSize; j++) {
            if (i < 3 && j < 3) {
                subBoard.push_back(Black);
            }
            else if (i > 4 && j > 4) {
                subBoard.push_back(White);
            }
            else
                subBoard.push_back(Empty);
        }
        board.push_back(subBoard);
    }


    int higlightedField = 0;
    int changeHighlightFrames = 0;
    int changeHighlightFrames2 = 0;
    bool p1flag = true;
    bool p2flag = true;
    bool p1isHuman = true;
    bool p2isHuman = true;
    sf::String p1String("");
    sf::String p2String("");

    sf::RenderWindow window(sf::VideoMode(320, 480), "Ugolki Setup", sf::Style::Close);

    while (window.isOpen())
    {

        sf::Vector2i mpos = sf::Mouse::getPosition(window);
        if (changeHighlightFrames > 0) changeHighlightFrames--;
        if (changeHighlightFrames2 > 0) changeHighlightFrames2--;
        bool isOver = false;
        int delta = 150;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (20 < mpos.x  && mpos.x < 300 && 50 < mpos.y && mpos.y < 90) {
                    higlightedField = 1;
                    p1flag = false;
                }
                else if (20 < mpos.x && mpos.x < 300 && 50 + delta < mpos.y && mpos.y < 90 + delta) {
                    higlightedField = 2;
                    p2flag = false;
                }
                else if (20 < mpos.x && mpos.x < 300 && 400 < mpos.y && mpos.y < 440) {
                    name1 = p1String;
                    name2 = p2String;

                    entry1 = (p1isHuman) ? "1" : "2";
                    entry2 = (p2isHuman) ? "1" : "2";

                    isOver = true;
                    window.close();
                    break;
                }

                else higlightedField = 0;
                if (200 < mpos.x && mpos.x < 300 && 100 < mpos.y && mpos.y < 120) {
                    p1isHuman = (p1isHuman) ? false : true;
                    changeHighlightFrames = 20;
                }
                else if (200 < mpos.x && mpos.x < 300 && 100 + delta < mpos.y && mpos.y < 120 + delta) {
                    p2isHuman = (p2isHuman) ? false : true;
                    changeHighlightFrames2 = 20;
                }
            }

            if (event.text.unicode == 8 && event.text.unicode != 105 && !p1flag && higlightedField == 1)
            {   
                if (p1String.end() != p1String.begin())
                p1String.erase(p1String.getSize() - 1);
            }
            else if (event.text.unicode == 8 && event.text.unicode != 105 && !p2flag && higlightedField == 2)
            {
                if (p2String.end() != p2String.begin())
                    p2String.erase(p2String.getSize() - 1);
            }

            else if (event.type == sf::Event::TextEntered && !p1flag && higlightedField == 1) {


                if (p1String.getSize() <= 14)
                    p1String += event.text.unicode;

            }
            else if (event.type == sf::Event::TextEntered && !p2flag && higlightedField == 2) {


                if (p2String.getSize() <= 14)
                    p2String += event.text.unicode;

            }
        }

        if (isOver) break;

        // drawing stuff


        // player 1 


        sf::RectangleShape p1Outline;
        p1Outline.setPosition(sf::Vector2f(10, 10));
        p1Outline.setFillColor(sf::Color::Transparent);
        p1Outline.setOutlineThickness(2);
        p1Outline.setOutlineColor(sf::Color(50, 50, 50, 150));
        p1Outline.setSize(sf::Vector2f(300, 130));

        sf::CircleShape firstPlayerIndicator;
        firstPlayerIndicator.setRadius(5);
        firstPlayerIndicator.setFillColor(sf::Color::White);
        firstPlayerIndicator.setOutlineThickness(2);
        firstPlayerIndicator.setOutlineColor(sf::Color(50, 50, 50, 150));
        firstPlayerIndicator.setPosition(sf::Vector2f(20, 22.5));

        sf::Font font;
        font.loadFromFile("InterVariable.ttf");
        sf::Text firstPlayerText("First Player", font);
        firstPlayerText.setCharacterSize(25);
        firstPlayerText.setPosition(sf::Vector2f(35, 10));
        firstPlayerText.setOutlineColor(sf::Color(50, 50, 50, 150));
        firstPlayerText.setOutlineThickness(2);

        sf::RectangleShape t1name;
        t1name.setFillColor(sf::Color::White);
        t1name.setOutlineColor(sf::Color(50, 50, 50, 150));
        t1name.setOutlineThickness(2);
        t1name.setSize(sf::Vector2f(280, 40));
        t1name.setPosition(sf::Vector2f(20, 50));
        if (higlightedField == 1)
            t1name.setOutlineColor(sf::Color::Cyan);

        sf::Text p1suggestion("Black Player", font);
        p1suggestion.setCharacterSize(25);
        p1suggestion.setPosition(sf::Vector2f(30, 53));
        p1suggestion.setFillColor(sf::Color(50, 50, 50, 150));
        if (!p1flag) p1suggestion.setString(p1String);

        sf::Text p1type("Type: ", font);
        if (p1isHuman) p1type.setString("Type: Human Player");
        else p1type.setString("Type: Computer Player");
        p1type.setCharacterSize(15);
        p1type.setPosition(sf::Vector2f(20, 100));
        p1type.setOutlineColor(sf::Color(50, 50, 50, 150));
        p1type.setOutlineThickness(2);

        sf::RectangleShape t1change;
        t1change.setFillColor(sf::Color::White);
        if (changeHighlightFrames > 0) t1change.setFillColor(sf::Color(50, 50, 50, 150));
        t1change.setOutlineColor(sf::Color(50, 50, 50, 150));
        t1change.setOutlineThickness(2);
        t1change.setSize(sf::Vector2f(100, 20));
        t1change.setPosition(sf::Vector2f(200, 100));

        sf::Text p1changeText("Change?", font);
        p1changeText.setCharacterSize(15);
        p1changeText.setPosition(sf::Vector2f(220, 100));
        p1changeText.setFillColor(sf::Color::Black);

        // player 2

        sf::RectangleShape p2Outline;
        p2Outline.setPosition(sf::Vector2f(10, 10 + delta));
        p2Outline.setFillColor(sf::Color::Transparent);
        p2Outline.setOutlineThickness(2);
        p2Outline.setOutlineColor(sf::Color(50, 50, 50, 150));
        p2Outline.setSize(sf::Vector2f(300, 130));

        sf::CircleShape secondPlayerIndicator;
        secondPlayerIndicator.setRadius(5);
        secondPlayerIndicator.setFillColor(sf::Color::White);
        secondPlayerIndicator.setOutlineThickness(2);
        secondPlayerIndicator.setFillColor(sf::Color::Black);
        secondPlayerIndicator.setOutlineColor(sf::Color(50, 50, 50, 150));
        secondPlayerIndicator.setPosition(sf::Vector2f(20, 22.5 + delta));

        
        sf::Text secondPlayerText("Second Player", font);
        secondPlayerText.setCharacterSize(25);
        secondPlayerText.setPosition(sf::Vector2f(35, 10 + delta));
        secondPlayerText.setOutlineColor(sf::Color(50, 50, 50, 150));
        secondPlayerText.setOutlineThickness(2);

        sf::RectangleShape t2name;
        t2name.setFillColor(sf::Color::White);
        t2name.setOutlineColor(sf::Color(50, 50, 50, 150));
        t2name.setOutlineThickness(2);
        t2name.setSize(sf::Vector2f(280, 40));
        t2name.setPosition(sf::Vector2f(20, 50 + delta));
        if (higlightedField == 2)
            t2name.setOutlineColor(sf::Color::Cyan);

        sf::Text p2suggestion("White Player", font);
        p2suggestion.setCharacterSize(25);
        p2suggestion.setPosition(sf::Vector2f(30, 53 + delta));
        p2suggestion.setFillColor(sf::Color(50, 50, 50, 150));
        if (!p2flag) p2suggestion.setString(p2String);

        sf::Text p2type("Type: ", font);
        if (p2isHuman) p2type.setString("Type: Human Player");
        else p2type.setString("Type: Computer Player");
        p2type.setCharacterSize(15);
        p2type.setPosition(sf::Vector2f(20, 100 + delta));
        p2type.setOutlineColor(sf::Color(50, 50, 50, 150));
        p2type.setOutlineThickness(2);

        sf::RectangleShape t2change;
        t2change.setFillColor(sf::Color::White);
        if (changeHighlightFrames2 > 0) t2change.setFillColor(sf::Color(50, 50, 50, 150));
        t2change.setOutlineColor(sf::Color(50, 50, 50, 150));
        t2change.setOutlineThickness(2);
        t2change.setSize(sf::Vector2f(100, 20));
        t2change.setPosition(sf::Vector2f(200, 100 + delta));

        sf::Text p2changeText("Change?", font);
        p2changeText.setCharacterSize(15);
        p2changeText.setPosition(sf::Vector2f(220, 100 + delta));
        p2changeText.setFillColor(sf::Color::Black);

        sf::RectangleShape setupComplete;
        setupComplete.setFillColor(sf::Color::White);
        setupComplete.setOutlineColor(sf::Color(50, 50, 50, 150));
        setupComplete.setOutlineThickness(2);
        setupComplete.setSize(sf::Vector2f(260, 40));
        setupComplete.setPosition(sf::Vector2f(30, 400));

        sf::Text sText("Start the game", font);
        sText.setCharacterSize(25);
        sText.setPosition(sf::Vector2f(70, 402));
        sText.setFillColor(sf::Color::Black);

        window.clear(sf::Color(100, 100, 100, 50));
        
        window.draw(p1Outline);
        window.draw(firstPlayerIndicator);
        window.draw(firstPlayerText);
        window.draw(t1name);
        window.draw(p1type);
        window.draw(t1change);
        window.draw(p1changeText);
        window.draw(p1suggestion);

        window.draw(p2Outline);
        window.draw(secondPlayerIndicator);
        window.draw(secondPlayerText);
        window.draw(t2name);
        window.draw(p2type);
        window.draw(t2change);
        window.draw(p2changeText);
        window.draw(p2suggestion);

        window.draw(setupComplete);
        window.draw(sText);

        window.display();
    }

    // Detach Visual Thread

    std::thread gfxThread(visuals, std::ref(board), std::ref(eventList));
    gfxThread.detach();


    if (entry1 == "1") {
        WhitePlayer = new Player(board, White, name1, &eventList);
    }
    else WhitePlayer = new AiPlayer(board, White, name1);

    if (entry2 == "1") {
        BlackPlayer = new Player(board, Black, name2, &eventList);
    }
    else BlackPlayer = new AiPlayer(board, Black, name2);

    Player* CurentPlayer = WhitePlayer;

    while (true)
    {
        int didHeWin = isEndNow(board, CurentPlayer->getType(), moveCount);
        if (isEndNow(board, CurentPlayer->getType(), moveCount) != 10) {
            sf::RenderWindow window(sf::VideoMode(200, 100), "Game ended!");
            sf::Font font;
            font.loadFromFile("InterVariable.ttf");
            sf::Text firstPlayerText("Player " + CurentPlayer->getName() + " won!", font);
            if (didHeWin == -1) firstPlayerText.setString("Player " + CurentPlayer->getName() + " lost!");
            else if (didHeWin == 0) firstPlayerText.setString("It's a draw!");
            firstPlayerText.setCharacterSize(25);
            firstPlayerText.setOutlineColor(sf::Color(50, 50, 50, 150));
            firstPlayerText.setOutlineThickness(2);

            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        exit(0);
                    }
                }

                window.clear();
                window.draw(firstPlayerText);
                window.display();
            }
        }
        //std::cout << "-------------------------------------------------\n";
        //std::cout << "Ход №" << moveCount << "\n\n";
        //std::cout << isEndNow(board, CurentPlayer->getType(), moveCount) << std::endl;
        //printBoard(board);
        //std::cout << std::endl;
        //std::cout << "Игрок " << CurentPlayer->getName() << '(' <<
        //    (char)CurentPlayer->getType() << ')' << ", ваш ход\n";
        CurentPlayer->makeMove();

        std::wcout << "seen\n";

        if (CurentPlayer->getType() == Black) moveCount++;
        CurentPlayer = (CurentPlayer == WhitePlayer) ? BlackPlayer : WhitePlayer;
    }

   
}


