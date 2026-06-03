#include "../include/Player.h"
#include <iostream>
#include <limits>

using namespace std;

Player::Player(string n, char s, int w, int l, int d)
    : name(n),
      symbol(s),
      win(w),
      loss(l),
      draw(d)
{
}

void Player::inputName()
{
    cout << "Enter player name: ";
    getline(cin >> ws, name);
}

void Player::inputMove(int &row, int &col)
{
    while (true)
    {
        cout << "Enter position (row and column): ";
        if (!(cin >> row >> col))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        if (row >= 0 && row <= 9 && col >= 0 && col <= 9)
        {
            break;
        }

        cout << "Coordinates must be inside the board. Please try again.\n";
    }
}

int Player::getWin() const
{
    return win;
}

int Player::getLoss() const
{
    return loss;
}

int Player::getDraw() const
{
    return draw;
}

int Player::getTotalMatch() const
{
    return win + loss + draw;
}

double Player::getWinRate() const
{
    if (win + loss == 0)
    {
        return 0.0;
    }

    return static_cast<double>(win) / (win + loss);
}

string Player::getName() const
{
    return name;
}

char Player::getSymbol() const
{
    return symbol;
}

void Player::setName(string n)
{
    name = n;
}

void Player::setSymbol(char s)
{
    symbol = s;
}

void Player::setWin(int w)
{
    win = w;
}

void Player::setLoss(int l)
{
    loss = l;
}

void Player::setDraw(int d)
{
    draw = d;
}

void Player::addWin()
{
    win++;
}

void Player::addLoss()
{
    loss++;
}

void Player::addDraw()
{
    draw++;
}

void Player::resetStatistic()
{
    win = 0;
    loss = 0;
    draw = 0;
}

void Player::displayInformation() const
{
    cout << "\n===== PLAYER INFORMATION =====\n";
    cout << "Name      : " << name << endl;
    cout << "Symbol    : " << symbol << endl;
    cout << "Wins      : " << win << endl;
    cout << "Losses    : " << loss << endl;
    cout << "Draws     : " << draw << endl;
    cout << "Total     : " << getTotalMatch() << endl;
    cout << "Win Rate  : " << getWinRate() * 100 << "%" << endl;
}
