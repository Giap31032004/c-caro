#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

class Player
{
private:
    string name;
    char symbol;
    int win;
    int loss;
    int draw;

public:
    Player(string n = "Player", char s = 'X', int w = 0, int l = 0, int d = 0);

    void inputName();
    void inputMove(int &row, int &col);

    int getWin() const;
    int getLoss() const;
    int getDraw() const;
    int getTotalMatch() const;

    double getWinRate() const;

    string getName() const;
    char getSymbol() const;

    void setName(string n);
    void setSymbol(char s);
    void setWin(int w);
    void setLoss(int l);
    void setDraw(int d);
    void resetStatistic();

    void addWin();
    void addLoss();
    void addDraw();

    void displayInformation() const;
};

#endif