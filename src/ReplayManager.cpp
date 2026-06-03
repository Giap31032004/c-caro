#include "../include/ReplayManager.h"

#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

ReplayManager::ReplayManager()
{
    indexFileName = "replays_index.txt";
}

void ReplayManager::saveReplay(const vector<Move> &moveHistory)
{
    string replayName;

    cin.ignore();
    cout << "Enter replay name: ";
    getline(cin, replayName);

    int id = 1;

    ifstream in(indexFileName);
    string line;

    while (getline(in, line))
    {
        id++;
    }

    in.close();

    string replayFileName = "replay_" + to_string(id) + ".txt";

    ofstream replayFile(replayFileName);

    for (const Move &move : moveHistory)
    {
        replayFile << move.row << " "
                   << move.col << " "
                   << move.symbol << endl;
    }

    replayFile.close();

    ofstream indexFile(indexFileName, ios::app);

    indexFile << id << "|"
              << replayName << "|"
              << replayFileName << endl;

    indexFile.close();

    cout << "Replay saved successfully.\n";
}

void ReplayManager::showReplayList()
{
    ifstream file(indexFileName);

    if (!file)
    {
        cout << "No replay found.\n";
        return;
    }

    string line;

    cout << "\n===== REPLAY LIST =====\n";

    while (getline(file, line))
    {
        cout << line << endl;
    }

    file.close();
}

void ReplayManager::replayById()
{
    int targetId;

    cout << "Enter replay ID: ";
    cin >> targetId;

    ifstream indexFile(indexFileName);

    if (!indexFile)
    {
        cout << "No replay found.\n";
        return;
    }

    string line;
    string replayFileName;

    while (getline(indexFile, line))
    {
        size_t p1 = line.find('|');
        size_t p2 = line.rfind('|');

        if (p1 == string::npos || p2 == string::npos || p1 == p2)
        {
            continue;
        }

        int id = stoi(line.substr(0, p1));

        if (id == targetId)
        {
            replayFileName = line.substr(p2 + 1);
            break;
        }
    }

    indexFile.close();

    if (replayFileName.empty())
    {
        cout << "Replay not found.\n";
        return;
    }

    Board board;

    ifstream replayFile(replayFileName);

    if (!replayFile)
    {
        cout << "Replay file not found.\n";
        return;
    }

    Move move;

    cin.ignore();

    while (replayFile >> move.row >> move.col >> move.symbol)
    {
        board.placeMove(move.row, move.col, move.symbol);
        board.displayBoard();

        cout << "Press Enter...";
        cin.get();
    }

    replayFile.close();
}

void ReplayManager::deleteReplayById()
{
    int targetId;

    cout << "Enter replay ID: ";
    cin >> targetId;

    ifstream in(indexFileName);

    if (!in)
    {
        cout << "No replay found.\n";
        return;
    }

    ofstream out("temp.txt");

    string line;
    string fileNameToDelete;

    while (getline(in, line))
    {
        size_t p1 = line.find('|');
        size_t p2 = line.rfind('|');

        if (p1 == string::npos || p2 == string::npos || p1 == p2)
        {
            continue;
        }

        int id = stoi(line.substr(0, p1));

        if (id == targetId)
        {
            fileNameToDelete = line.substr(p2 + 1);
            continue;
        }

        out << line << endl;
    }

    in.close();
    out.close();

    remove(indexFileName.c_str());
    rename("temp.txt", indexFileName.c_str());

    if (!fileNameToDelete.empty())
    {
        remove(fileNameToDelete.c_str());
        cout << "Replay deleted.\n";
    }
    else
    {
        cout << "Replay not found.\n";
    }
}