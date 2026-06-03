#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Board.h"
#include "Player.h"
#include "Bot.h"
#include "ReplayManager.h"
#include "PlayerManager.h"

using namespace std;

class Game
{
private:
    Board board;
    Player player1;
    Player player2;
    vector<Move> moveHistory;

public:
    Game();

    void startPvP();
    void startPlayerVsBot(BotLevel selectedLevel);
};

#endif