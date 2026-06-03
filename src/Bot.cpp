#include "../include/Bot.h"

#include <algorithm>
#include <cstdlib>
#include <limits>
#include <vector>

using namespace std;

Bot::Bot(BotLevel l, char s, char enemy, BotStyle selectedStyle)
{
    level = l;
    style = selectedStyle;
    symbol = s;
    enemySymbol = enemy;
    lastExplanation = "No move has been made yet.";
}

char Bot::getSymbol() const
{
    return symbol;
}

string Bot::getLastExplanation() const
{
    return lastExplanation;
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
    else if (level == Hard)
    {
        makeHardMove(board, row, col);
    }
    else
    {
        makeExpertMove(board, row, col);
    }
}

void Bot::makeEasyMove(Board &board, int &row, int &col)
{
    vector<pair<int, int>> cells = board.getEmptyCells();

    if (cells.empty())
    {
        row = -1;
        col = -1;
        return;
    }

    int index = rand() % cells.size();
    row = cells[index].first;
    col = cells[index].second;
    lastExplanation = "Easy bot selected a random empty cell.";
}

bool Bot::canWin(Board &board,
                 int row,
                 int col,
                 char checkSymbol)
{
    if (board.getCell(row, col) != Board::emptyCell)
    {
        return false;
    }

    board.setCellForSimulation(row, col, checkSymbol);
    bool result = board.checkWin(row, col, checkSymbol);
    board.setCellForSimulation(row, col, Board::emptyCell);

    return result;
}

void Bot::makeMediumMove(Board &board, int &row, int &col)
{
    int size = board.getSize();

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (canWin(board, i, j, symbol))
            {
                row = i;
                col = j;
                lastExplanation = "Medium bot found an immediate winning move.";
                return;
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (canWin(board, i, j, enemySymbol))
            {
                row = i;
                col = j;
                lastExplanation = "Medium bot blocked the opponent's immediate win.";
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
           r < board.getSize() &&
           c >= 0 &&
           c < board.getSize() &&
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
    if (board.getCell(row, col) != Board::emptyCell)
    {
        return -1;
    }

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

        int forward = countDirection(board, row, col, checkSymbol, dx, dy);
        int backward = countDirection(board, row, col, checkSymbol, -dx, -dy);
        int count = 1 + forward + backward;

        bool openForward = board.getCell(row + (forward + 1) * dx,
                                         col + (forward + 1) * dy) == Board::emptyCell;
        bool openBackward = board.getCell(row - (backward + 1) * dx,
                                          col - (backward + 1) * dy) == Board::emptyCell;
        int openEnds = static_cast<int>(openForward) + static_cast<int>(openBackward);

        if (count >= 5)
        {
            totalScore += 1000000;
        }
        else if (count == 4 && openEnds == 2)
        {
            totalScore += 100000;
        }
        else if (count == 4 && openEnds == 1)
        {
            totalScore += 20000;
        }
        else if (count == 3 && openEnds == 2)
        {
            totalScore += 8000;
        }
        else if (count == 3 && openEnds == 1)
        {
            totalScore += 1500;
        }
        else if (count == 2 && openEnds == 2)
        {
            totalScore += 500;
        }
        else if (count == 2 && openEnds == 1)
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
    int bestScore = numeric_limits<int>::min();
    row = -1;
    col = -1;

    int attackWeight = style == Aggressive ? 3 : 2;
    int defenseWeight = style == Defensive ? 3 : 2;

    for (const pair<int, int> &cell : board.getEmptyCells())
    {
        int attackScore = evaluateCell(board, cell.first, cell.second, symbol);
        int defenseScore = evaluateCell(board, cell.first, cell.second, enemySymbol);
        int totalScore = attackScore * attackWeight + defenseScore * defenseWeight;

        if (totalScore > bestScore)
        {
            bestScore = totalScore;
            row = cell.first;
            col = cell.second;
        }
    }

    if (row == -1 || col == -1)
    {
        makeEasyMove(board, row, col);
        return;
    }

    lastExplanation = "Hard bot chose the cell with the best attack/defense score.";
}

int Bot::evaluateBoard(Board &board)
{
    int score = 0;

    for (const pair<int, int> &cell : board.getEmptyCells())
    {
        score += evaluateCell(board, cell.first, cell.second, symbol);
        score -= evaluateCell(board, cell.first, cell.second, enemySymbol);
    }

    return score;
}

int Bot::minimax(Board &board,
                 int depth,
                 bool maximizing,
                 int alpha,
                 int beta)
{
    if (depth == 0 || board.checkFull())
    {
        return evaluateBoard(board);
    }

    vector<pair<int, int>> cells = board.getEmptyCells();

    if (cells.size() > 16)
    {
        sort(cells.begin(), cells.end(), [&](const pair<int, int> &a, const pair<int, int> &b) {
            return evaluateCell(board, a.first, a.second, symbol) +
                       evaluateCell(board, a.first, a.second, enemySymbol) >
                   evaluateCell(board, b.first, b.second, symbol) +
                       evaluateCell(board, b.first, b.second, enemySymbol);
        });
        cells.resize(16);
    }

    if (maximizing)
    {
        int best = numeric_limits<int>::min();

        for (const pair<int, int> &cell : cells)
        {
            board.setCellForSimulation(cell.first, cell.second, symbol);

            if (board.checkWin(cell.first, cell.second, symbol))
            {
                board.setCellForSimulation(cell.first, cell.second, Board::emptyCell);
                return 10000000 + depth;
            }

            best = max(best, minimax(board, depth - 1, false, alpha, beta));
            board.setCellForSimulation(cell.first, cell.second, Board::emptyCell);

            alpha = max(alpha, best);
            if (beta <= alpha)
            {
                break;
            }
        }

        return best;
    }

    int best = numeric_limits<int>::max();

    for (const pair<int, int> &cell : cells)
    {
        board.setCellForSimulation(cell.first, cell.second, enemySymbol);

        if (board.checkWin(cell.first, cell.second, enemySymbol))
        {
            board.setCellForSimulation(cell.first, cell.second, Board::emptyCell);
            return -10000000 - depth;
        }

        best = min(best, minimax(board, depth - 1, true, alpha, beta));
        board.setCellForSimulation(cell.first, cell.second, Board::emptyCell);

        beta = min(beta, best);
        if (beta <= alpha)
        {
            break;
        }
    }

    return best;
}

void Bot::makeExpertMove(Board &board, int &row, int &col)
{
    int bestScore = numeric_limits<int>::min();
    row = -1;
    col = -1;

    vector<pair<int, int>> cells = board.getEmptyCells();

    if (cells.empty())
    {
        return;
    }

    if (cells.size() > 20)
    {
        sort(cells.begin(), cells.end(), [&](const pair<int, int> &a, const pair<int, int> &b) {
            return evaluateCell(board, a.first, a.second, symbol) +
                       evaluateCell(board, a.first, a.second, enemySymbol) >
                   evaluateCell(board, b.first, b.second, symbol) +
                       evaluateCell(board, b.first, b.second, enemySymbol);
        });
        cells.resize(20);
    }

    for (const pair<int, int> &cell : cells)
    {
        board.setCellForSimulation(cell.first, cell.second, symbol);

        int score;
        if (board.checkWin(cell.first, cell.second, symbol))
        {
            score = 10000000;
        }
        else
        {
            score = minimax(board, 2, false,
                            numeric_limits<int>::min(),
                            numeric_limits<int>::max());
        }

        board.setCellForSimulation(cell.first, cell.second, Board::emptyCell);

        if (score > bestScore)
        {
            bestScore = score;
            row = cell.first;
            col = cell.second;
        }
    }

    lastExplanation = "Expert bot used minimax with alpha-beta pruning on the strongest candidate moves.";
}
