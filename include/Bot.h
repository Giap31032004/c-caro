#ifndef BOT_H
#define BOT_H

#include "Board.h"
#include <string>

enum BotLevel
{
    Easy = 0,
    Medium,
    Hard,
    Expert
};

enum BotStyle
{
    Balanced = 0,
    Aggressive,
    Defensive
};

class Bot
{
private:
    BotLevel level;
    BotStyle style;
    char symbol;
    char enemySymbol;
    std::string lastExplanation;

public:
    Bot(BotLevel l, char s = 'O', char enemy = 'X', BotStyle selectedStyle = Balanced);

    char getSymbol() const;
    std::string getLastExplanation() const;

    void makeMove(Board &board, int &row, int &col);

private:
    void makeEasyMove(Board &board, int &row, int &col);
    void makeMediumMove(Board &board, int &row, int &col);
    void makeHardMove(Board &board, int &row, int &col);
    void makeExpertMove(Board &board, int &row, int &col);

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

    int minimax(Board &board,
                int depth,
                bool maximizing,
                int alpha,
                int beta);

    int evaluateBoard(Board &board);
};

#endif
