#include "../include/PlayerManager.h"

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

PlayerManager::PlayerManager()
{
    fileName = "players.txt";
}

void PlayerManager::loadPlayers()
{
    players.clear();

    ifstream file(fileName);

    if (!file)
    {
        return;
    }

    string name;
    int wins, losses, draws;

    while (file >> name >> wins >> losses >> draws)
    {
        players.push_back(Player(name, ' ', wins, losses, draws));
    }

    file.close();
}

void PlayerManager::savePlayers()
{
    ofstream file(fileName);

    for (const Player &player : players)
    {
        file << player.getName() << " "
             << player.getWin() << " "
             << player.getLoss() << " "
             << player.getDraw() << endl;
    }

    file.close();
}

int PlayerManager::findPlayerIndex(string name)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].getName() == name)
        {
            return i;
        }
    }

    return -1;
}

void PlayerManager::addPlayerIfNotExist(string name)
{
    if (findPlayerIndex(name) == -1)
    {
        players.push_back(Player(name, ' ', 0, 0, 0));
    }
}

void PlayerManager::updateWinLoss(string winnerName, string loserName)
{
    loadPlayers();

    addPlayerIfNotExist(winnerName);
    addPlayerIfNotExist(loserName);

    int winnerIndex = findPlayerIndex(winnerName);
    int loserIndex = findPlayerIndex(loserName);

    players[winnerIndex].addWin();
    players[loserIndex].addLoss();

    savePlayers();
}

void PlayerManager::updateDraw(string player1Name, string player2Name)
{
    loadPlayers();

    addPlayerIfNotExist(player1Name);
    addPlayerIfNotExist(player2Name);

    int p1Index = findPlayerIndex(player1Name);
    int p2Index = findPlayerIndex(player2Name);

    players[p1Index].addDraw();
    players[p2Index].addDraw();

    savePlayers();
}

void PlayerManager::showAllPlayers()
{
    loadPlayers();

    if (players.empty())
    {
        cout << "No player information found.\n";
        return;
    }

    cout << "\n===== PLAYER INFORMATION =====\n";

    for (const Player &player : players)
    {
        player.displayInformation();
        cout << "----------------------\n";
    }
}

void PlayerManager::searchPlayerByName()
{
    loadPlayers();

    string name;

    cout << "Enter player name: ";
    cin >> name;

    int index = findPlayerIndex(name);

    if (index == -1)
    {
        cout << "Player not found.\n";
        return;
    }

    players[index].displayInformation();
}

void PlayerManager::findEquivalentPlayer()
{
    loadPlayers();

    string name;

    cout << "Enter player name: ";
    cin >> name;

    int index = findPlayerIndex(name);

    if (index == -1)
    {
        cout << "Player not found.\n";
        return;
    }

    int targetTotal = players[index].getWin() + players[index].getLoss();

    if (targetTotal == 0)
    {
        cout << "This player does not have enough data to calculate level.\n";
        return;
    }

    double targetLevel =
        static_cast<double>(players[index].getWin()) / targetTotal;

    int bestIndex = -1;
    double minDifference = 999999.0;

    for (int i = 0; i < players.size(); i++)
    {
        if (i == index)
        {
            continue;
        }

        int otherTotal = players[i].getWin() + players[i].getLoss();

        if (otherTotal == 0)
        {
            continue;
        }

        double otherLevel =
            static_cast<double>(players[i].getWin()) / otherTotal;

        double difference = abs(targetLevel - otherLevel);

        if (difference < minDifference)
        {
            minDifference = difference;
            bestIndex = i;
        }
    }

    if (bestIndex == -1)
    {
        cout << "No equivalent player found.\n";
        return;
    }

    cout << "\nEquivalent Player:\n";
    players[bestIndex].displayInformation();

    cout << "Target level: " << targetLevel * 100 << "%\n";

    int bestTotal =
        players[bestIndex].getWin() + players[bestIndex].getLoss();

    double bestLevel =
        static_cast<double>(players[bestIndex].getWin()) / bestTotal;

    cout << "Equivalent level: " << bestLevel * 100 << "%\n";
}