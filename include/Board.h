#ifndef BOARD_H
#define BOARD_H

#include<iostream>
class Board
{
public:
    static const int size = 10;

private:
    int moveCount;
    char board[size][size];

public:
    Board();

    void resetBoard();

    char getCell(int row, int col) const;

    void setCellForSimulation(int row, int col, char symbol);

    bool isInsideBoard(int row, int col) const;

    bool placeMove(int row, int col, char symbol);

    void displayBoard() const;

    bool checkFull() const;

    bool isEmpty(int row, int col) const;

    bool checkWin(int row, int col, char symbol) const;
};

#endif