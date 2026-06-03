#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

#include "Board.h"
#include "Player.h"
#include "Bot.h"
#include "ReplayManager.h"
#include "PlayerManager.h"

using namespace std;

struct GameSettings
{
    int boardSize = Board::defaultSize;
    WinRule winRule = FiveOrMore;
    bool randomFirstPlayer = false;
    bool chooseSymbol = false;
    bool timeLimitEnabled = false;
    int turnTimeLimitSeconds = 30;
    BotStyle botStyle = Balanced;
};

class Game
{
private:
    Board board;
    Player player1;
    Player player2;
    vector<Move> moveHistory;
    vector<Move> redoHistory;
    GameSettings settings;

public:
    Game(GameSettings selectedSettings = GameSettings());

    void startPvP();
    void startPlayerVsBot(BotLevel selectedLevel);
    void startBotVsBot(BotLevel bot1Level, BotLevel bot2Level);

private:
    void runMatch(const string names[2],
                  const char symbols[2],
                  const bool isBot[2],
                  const BotLevel botLevels[2]);

    bool handleHumanTurn(const string &playerName,
                         char symbol,
                         int &currentPlayerIndex,
                         int &row,
                         int &col,
                         bool &quitGame);

    void showHistory() const;
    void showHelp() const;
    void showHint(char symbol, char enemySymbol);
    void askSaveReplay();
    int findPlayerIndexBySymbol(const char symbols[2], char symbol) const;
    void clearScreen() const;
};

#endif
