#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <vector>
#include <string>

#include "Player.h"

using namespace std;

class PlayerManager
{
private:
    vector<Player> players;
    string fileName;

public:
    PlayerManager();

    void loadPlayers();

    void savePlayers();

    int findPlayerIndex(string name);

    void addPlayerIfNotExist(string name);

    void updateWinLoss(string winnerName,
                       string loserName);

    void updateDraw(string player1Name,
                    string player2Name);

    void showAllPlayers();

    void searchPlayerByName();

    void findEquivalentPlayer();
};

#endif