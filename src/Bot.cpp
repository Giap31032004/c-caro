#include "../include/Bot.h"

#include <cstdlib>

Bot::Bot(BotLevel l, char s, char enemy)
{
    level = l;
    symbol = s;
    enemySymbol = enemy;
}

char Bot::getSymbol() const
{
    return symbol;
}

void Bot::makeMove(Board &board, int &row, int &col)
{
    if (level == Easy)
    {
        makeEasyMove(board, row, col);
    }
    else if (level == Medium)
    {
        makeMediumMove(board, row, col);
    }
    else
    {
        makeHardMove(board, row, col);
    }
}

void Bot::makeEasyMove(Board &board, int &row, int &col)
{
    do
    {
        row = rand() % Board::size;
        col = rand() % Board::size;
    } while (!board.isEmpty(row, col));
}

bool Bot::canWin(Board &board,
                 int row,
                 int col,
                 char checkSymbol)
{
    if (!board.isEmpty(row, col))
    {
        return false;
    }

    board.setCellForSimulation(
        row,
        col,
        checkSymbol);

    bool result =
        board.checkWin(
            row,
            col,
            checkSymbol);

    board.setCellForSimulation(
        row,
        col,
        ' ');

    return result;
}

void Bot::makeMediumMove(Board &board,
                         int &row,
                         int &col)
{
    // Bot thắng ngay

    for (int i = 0; i < Board::size; i++)
    {
        for (int j = 0; j < Board::size; j++)
        {
            if (canWin(board, i, j, symbol))
            {
                row = i;
                col = j;
                return;
            }
        }
    }

    // Chặn đối thủ

    for (int i = 0; i < Board::size; i++)
    {
        for (int j = 0; j < Board::size; j++)
        {
            if (canWin(board, i, j, enemySymbol))
            {
                row = i;
                col = j;
                return;
            }
        }
    }

    makeEasyMove(board, row, col);
}

int Bot::countDirection(Board &board,
                        int row,
                        int col,
                        char checkSymbol,
                        int dx,
                        int dy)
{
    int count = 0;

    int r = row + dx;
    int c = col + dy;

    while (r >= 0 &&
           r < Board::size &&
           c >= 0 &&
           c < Board::size &&
           board.getCell(r, c) == checkSymbol)
    {
        count++;

        r += dx;
        c += dy;
    }

    return count;
}

int Bot::evaluateCell(Board &board,
                      int row,
                      int col,
                      char checkSymbol)
{
    int directions[4][2] =
    {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    int totalScore = 0;

    for (int i = 0; i < 4; i++)
    {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int count =
            1 +
            countDirection(
                board,
                row,
                col,
                checkSymbol,
                dx,
                dy)
            +
            countDirection(
                board,
                row,
                col,
                checkSymbol,
                -dx,
                -dy);

        if (count >= 5)
        {
            totalScore += 100000;
        }
        else if (count == 4)
        {
            totalScore += 10000;
        }
        else if (count == 3)
        {
            totalScore += 1000;
        }
        else if (count == 2)
        {
            totalScore += 100;
        }
        else
        {
            totalScore += 10;
        }
    }

    return totalScore;
}

void Bot::makeHardMove(Board &board,
                       int &row,
                       int &col)
{
    int bestScore = -1;

    row = -1;
    col = -1;

    for (int i = 0; i < Board::size; i++)
    {
        for (int j = 0; j < Board::size; j++)
        {
            if (board.isEmpty(i, j))
            {
                int attackScore =
                    evaluateCell(
                        board,
                        i,
                        j,
                        symbol);

                int defenseScore =
                    evaluateCell(
                        board,
                        i,
                        j,
                        enemySymbol);

                int totalScore =
                    attackScore +
                    defenseScore * 2;

                if (totalScore > bestScore)
                {
                    bestScore = totalScore;
                    row = i;
                    col = j;
                }
            }
        }
    }

    if (row == -1 || col == -1)
    {
        makeEasyMove(board, row, col);
    }
}