#include "../include/Board.h"

#include <iomanip>

using namespace std;

Board::Board(int selectedSize, WinRule selectedRule)
{
    currentSize = defaultSize;
    winRule = selectedRule;
    setSize(selectedSize);
}

void Board::resetBoard()
{
    moveCount = 0;

    for (int row = 0; row < maxSize; row++)
    {
        for (int col = 0; col < maxSize; col++)
        {
            board[row][col] = emptyCell;
        }
    }
}

void Board::setSize(int selectedSize)
{
    if (selectedSize != 10 && selectedSize != 15 && selectedSize != 20)
    {
        selectedSize = defaultSize;
    }

    currentSize = selectedSize;
    resetBoard();
}

int Board::getSize() const
{
    return currentSize;
}

void Board::setWinRule(WinRule selectedRule)
{
    winRule = selectedRule;
}

WinRule Board::getWinRule() const
{
    return winRule;
}

void Board::displayBoard(int lastRow, int lastCol) const
{
    cout << "\nLast move is shown in brackets.\n\n";
    cout << "     ";

    for (int col = 0; col < currentSize; col++)
    {
        cout << setw(3) << col << ' ';
    }

    cout << '\n';
    printDivider();

    for (int row = 0; row < currentSize; row++)
    {
        cout << setw(3) << row << " |";

        for (int col = 0; col < currentSize; col++)
        {
            char cell = board[row][col];

            if (row == lastRow && col == lastCol && cell != emptyCell)
            {
                cout << '[' << cell << ']';
            }
            else
            {
                cout << ' ' << cell << ' ';
            }

            cout << '|';
        }

        cout << '\n';
        printDivider();
    }
}

void Board::printDivider() const
{
    cout << "    +";

    for (int col = 0; col < currentSize; col++)
    {
        cout << "---+";
    }

    cout << '\n';
}

bool Board::checkFull() const
{
    return moveCount >= currentSize * currentSize;
}

bool Board::isInsideBoard(int row, int col) const
{
    if (row < 0 || row >= currentSize || col < 0 || col >= currentSize)
    {
        cout << "Row or column is outside the board. Please enter a value from 0 to "
             << currentSize - 1 << ".\n";
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

    if (board[row][col] != emptyCell)
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

bool Board::removeMove(int row, int col)
{
    if (!isInsideBoard(row, col) || board[row][col] == emptyCell)
    {
        return false;
    }

    board[row][col] = emptyCell;
    moveCount--;

    return true;
}

char Board::getCell(int row, int col) const
{
    if (row < 0 || row >= currentSize || col < 0 || col >= currentSize)
    {
        return '\0';
    }

    return board[row][col];
}

void Board::setCellForSimulation(int row, int col, char symbol)
{
    if (row < 0 || row >= currentSize || col < 0 || col >= currentSize)
    {
        return;
    }

    if (board[row][col] == emptyCell && symbol != emptyCell)
    {
        moveCount++;
    }
    else if (board[row][col] != emptyCell && symbol == emptyCell)
    {
        moveCount--;
    }

    board[row][col] = symbol;
}

vector<pair<int, int>> Board::getEmptyCells() const
{
    vector<pair<int, int>> cells;

    for (int row = 0; row < currentSize; row++)
    {
        for (int col = 0; col < currentSize; col++)
        {
            if (board[row][col] == emptyCell)
            {
                cells.push_back({row, col});
            }
        }
    }

    return cells;
}

int Board::countDirection(int row, int col, char symbol, int dx, int dy) const
{
    int count = 0;
    int r = row + dx;
    int c = col + dy;

    while (r >= 0 && r < currentSize &&
           c >= 0 && c < currentSize &&
           board[r][c] == symbol)
    {
        count++;
        r += dx;
        c += dy;
    }

    return count;
}

bool Board::isBlockedAt(int row, int col, char symbol) const
{
    if (row < 0 || row >= currentSize || col < 0 || col >= currentSize)
    {
        return true;
    }

    return board[row][col] != emptyCell && board[row][col] != symbol;
}

bool Board::checkWin(int row, int col, char symbol) const
{
    if (getCell(row, col) != symbol)
    {
        return false;
    }

    int directions[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    for (int i = 0; i < 4; i++)
    {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int forward = countDirection(row, col, symbol, dx, dy);
        int backward = countDirection(row, col, symbol, -dx, -dy);
        int count = 1 + forward + backward;

        int frontRow = row + (forward + 1) * dx;
        int frontCol = col + (forward + 1) * dy;
        int backRow = row - (backward + 1) * dx;
        int backCol = col - (backward + 1) * dy;

        if (winRule == FiveOrMore && count >= 5)
        {
            return true;
        }

        if (winRule == ExactlyFive &&
            count == 5 &&
            getCell(frontRow, frontCol) != symbol &&
            getCell(backRow, backCol) != symbol)
        {
            return true;
        }

        if (winRule == NotBlockedBothEnds &&
            count >= 5 &&
            !(isBlockedAt(frontRow, frontCol, symbol) &&
              isBlockedAt(backRow, backCol, symbol)))
        {
            return true;
        }
    }

    return false;
}
