#ifndef MOVE_H
#define MOVE_H

#include <string>

struct Move
{
    int row;
    int col;
    char symbol;
    std::string playerName;
    int turnNumber = 0;
};

#endif
