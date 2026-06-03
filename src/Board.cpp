#include "../include/Board.h"
using namespace std;

Board::Board()
{
    resetBoard();
}

void Board::resetBoard()
{
    moveCount = 0;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            board[row][col] = ' ';
        }
    }
}

void Board::displayBoard() const
{
    cout << "\n=> Player 1 = X, Player 2/Bot = O\n\n";

    cout << "    ";
    for (int col = 0; col < size; col++)
    {
        cout << " " << col << "  ";
    }
    cout << endl;

    for (int row = 0; row < size; row++)
    {
        cout << "   ";
        for (int col = 0; col < size; col++)
        {
            cout << "|---";
        }
        cout << "|\n";

        cout << " " << row << " ";
        for (int col = 0; col < size; col++)
        {
            cout << "| " << board[row][col] << " ";
        }
        cout << "|\n";
    }

    cout << "   ";
    for (int col = 0; col < size; col++)
    {
        cout << "|---";
    }
    cout << "|\n";
}

bool Board::checkFull() const
{
    return moveCount >= size * size;
}

bool Board::isInsideBoard(int row, int col) const
{
    if (row < 0 || row >= size || col < 0 || col >= size)
    {
        cout << "Row or column is outside the board. Please enter a value from 0 to 9.\n";
        return false;
    }

    return true;
}

bool Board::isEmpty(int row, int col) const
{
    if (!isInsideBoard(row, col))
    {
        return false;
    }

    if (board[row][col] != ' ')
    {
        cout << "This cell is already occupied. Please choose another cell.\n";
        return false;
    }

    return true;
}

bool Board::placeMove(int row, int col, char symbol)
{
    if (!isEmpty(row, col))
    {
        return false;
    }

    board[row][col] = symbol;
    moveCount++;

    return true;
}

char Board::getCell(int row, int col) const
{
    if (!isInsideBoard(row, col))
    {
        return '\0';
    }

    return board[row][col];
}

void Board::setCellForSimulation(int row, int col, char symbol)
{
    if (isInsideBoard(row, col))
    {
        board[row][col] = symbol;
    }
}

bool Board::checkWin(int row, int col, char symbol) const
{
    int directions[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    for (int i = 0; i < 4; i++)
    {
        int count = 1;

        int dx = directions[i][0];
        int dy = directions[i][1];

        int r = row + dx;
        int c = col + dy;

        while (r >= 0 && r < size &&
               c >= 0 && c < size &&
               board[r][c] == symbol)
        {
            count++;
            r += dx;
            c += dy;
        }

        r = row - dx;
        c = col - dy;

        while (r >= 0 && r < size &&
               c >= 0 && c < size &&
               board[r][c] == symbol)
        {
            count++;
            r -= dx;
            c -= dy;
        }

        if (count >= 5)
        {
            return true;
        }
    }

    return false;
}
