#ifndef BOT_H
#define BOT_H

#include "Board.h"

enum BotLevel
{
    Easy = 0,
    Medium,
    Hard
};

class Bot
{
private:
    BotLevel level;
    char symbol;
    char enemySymbol;

public:
    Bot(BotLevel l, char s = 'O', char enemy = 'X');

    char getSymbol() const;

    void makeMove(Board &board, int &row, int &col);

private:
    void makeEasyMove(Board &board, int &row, int &col);
    void makeMediumMove(Board &board, int &row, int &col);
    void makeHardMove(Board &board, int &row, int &col);

    bool canWin(Board &board,
                int row,
                int col,
                char checkSymbol);

    int countDirection(Board &board,
                       int row,
                       int col,
                       char checkSymbol,
                       int dx,
                       int dy);

    int evaluateCell(Board &board,
                     int row,
                     int col,
                     char checkSymbol);
};

#endif