#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

enum WinRule
{
    FiveOrMore = 0,
    ExactlyFive,
    NotBlockedBothEnds
};

class Board
{
public:
    static const int minSize = 10;
    static const int maxSize = 20;
    static const int defaultSize = 10;
    static const char emptyCell = ' ';

private:
    int moveCount;
    int currentSize;
    WinRule winRule;
    char board[maxSize][maxSize];

public:
    Board(int selectedSize = defaultSize, WinRule selectedRule = FiveOrMore);

    void resetBoard();
    void setSize(int selectedSize);
    int getSize() const;
    void setWinRule(WinRule selectedRule);
    WinRule getWinRule() const;

    char getCell(int row, int col) const;

    void setCellForSimulation(int row, int col, char symbol);

    bool isInsideBoard(int row, int col) const;

    bool placeMove(int row, int col, char symbol);
    bool removeMove(int row, int col);

    void displayBoard(int lastRow = -1, int lastCol = -1) const;

    bool checkFull() const;

    bool isEmpty(int row, int col) const;

    bool checkWin(int row, int col, char symbol) const;
    std::vector<std::pair<int, int>> getEmptyCells() const;

private:
    int countDirection(int row, int col, char symbol, int dx, int dy) const;
    bool isBlockedAt(int row, int col, char symbol) const;
    void printDivider() const;
};

#endif
