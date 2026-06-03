#ifndef REPLAY_MANAGER_H
#define REPLAY_MANAGER_H

#include <string>
#include <vector>

#include "Board.h"
#include "Move.h"

using namespace std;

class ReplayManager
{
private:
    string indexFileName;

public:
    ReplayManager();

    void saveReplay(const vector<Move> &moveHistory);
    void showReplayList();
    void replayById();
    void deleteReplayById();
};

#endif
