#include "../include/Game.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

Game::Game(GameSettings selectedSettings)
    : board(selectedSettings.boardSize, selectedSettings.winRule),
      player1("Player 1", 'X'),
      player2("Player 2", 'O')
{
    settings = selectedSettings;
}

void Game::startPvP()
{
    player1.inputName();
    player2.inputName();

    char symbols[2] = {'X', 'O'};

    if (settings.chooseSymbol)
    {
        char choice;
        cout << player1.getName() << ", choose your symbol (X/O): ";
        cin >> choice;

        if (choice == 'O' || choice == 'o')
        {
            symbols[0] = 'O';
            symbols[1] = 'X';
        }
    }

    string names[2] = {player1.getName(), player2.getName()};
    bool isBot[2] = {false, false};
    BotLevel botLevels[2] = {Easy, Easy};

    runMatch(names, symbols, isBot, botLevels);
}

void Game::startPlayerVsBot(BotLevel selectedLevel)
{
    player1.inputName();

    char symbols[2] = {'X', 'O'};

    if (settings.chooseSymbol)
    {
        char choice;
        cout << player1.getName() << ", choose your symbol (X/O): ";
        cin >> choice;

        if (choice == 'O' || choice == 'o')
        {
            symbols[0] = 'O';
            symbols[1] = 'X';
        }
    }

    string names[2] = {player1.getName(), "Bot"};
    bool isBot[2] = {false, true};
    BotLevel botLevels[2] = {Easy, selectedLevel};

    runMatch(names, symbols, isBot, botLevels);
}

void Game::startBotVsBot(BotLevel bot1Level, BotLevel bot2Level)
{
    string names[2] = {"Bot 1", "Bot 2"};
    char symbols[2] = {'X', 'O'};
    bool isBot[2] = {true, true};
    BotLevel botLevels[2] = {bot1Level, bot2Level};

    runMatch(names, symbols, isBot, botLevels);
}

void Game::runMatch(const string names[2],
                    const char symbols[2],
                    const bool isBot[2],
                    const BotLevel botLevels[2])
{
    board.setSize(settings.boardSize);
    board.setWinRule(settings.winRule);
    moveHistory.clear();
    redoHistory.clear();

    int currentPlayerIndex = settings.randomFirstPlayer ? rand() % 2
                                                        : findPlayerIndexBySymbol(symbols, 'X');
    bool quitGame = false;

    while (!quitGame)
    {
        Move lastMove = moveHistory.empty() ? Move{-1, -1, ' ', "", 0} : moveHistory.back();

        clearScreen();
        cout << "Mode board: " << settings.boardSize << "x" << settings.boardSize << '\n';
        cout << "Turn: " << names[currentPlayerIndex]
             << " (" << symbols[currentPlayerIndex] << ")\n";
        board.displayBoard(lastMove.row, lastMove.col);

        int row = -1;
        int col = -1;
        char currentSymbol = symbols[currentPlayerIndex];

        auto startTime = chrono::steady_clock::now();

        if (isBot[currentPlayerIndex])
        {
            Bot bot(botLevels[currentPlayerIndex],
                    currentSymbol,
                    symbols[1 - currentPlayerIndex],
                    settings.botStyle);

            bot.makeMove(board, row, col);

            if (row == -1 || col == -1)
            {
                cout << "Bot could not find a move.\n";
                break;
            }

            board.placeMove(row, col, currentSymbol);

            cout << names[currentPlayerIndex] << " moved at: "
                 << row << ' ' << col << '\n';
            cout << bot.getLastExplanation() << '\n';

            if (isBot[0] && isBot[1])
            {
                cout << "Press Enter for next bot move...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        }
        else
        {
            if (!handleHumanTurn(names[currentPlayerIndex],
                                 currentSymbol,
                                 currentPlayerIndex,
                                 row,
                                 col,
                                 quitGame))
            {
                continue;
            }
        }

        auto endTime = chrono::steady_clock::now();
        int elapsedSeconds =
            static_cast<int>(chrono::duration_cast<chrono::seconds>(endTime - startTime).count());

        if (settings.timeLimitEnabled &&
            elapsedSeconds > settings.turnTimeLimitSeconds &&
            !isBot[currentPlayerIndex])
        {
            cout << names[currentPlayerIndex] << " exceeded the time limit and loses the game.\n";
            cout << names[1 - currentPlayerIndex] << " wins!\n";
            PlayerManager playerManager;
            playerManager.updateWinLoss(names[1 - currentPlayerIndex], names[currentPlayerIndex]);
            askSaveReplay();
            break;
        }

        Move move{row, col, currentSymbol, names[currentPlayerIndex],
                  static_cast<int>(moveHistory.size()) + 1};
        moveHistory.push_back(move);
        redoHistory.clear();

        if (board.checkWin(row, col, currentSymbol))
        {
            clearScreen();
            board.displayBoard(row, col);
            cout << names[currentPlayerIndex] << " wins!\n";

            PlayerManager playerManager;
            playerManager.updateWinLoss(names[currentPlayerIndex], names[1 - currentPlayerIndex]);

            askSaveReplay();
            break;
        }

        if (board.checkFull())
        {
            clearScreen();
            board.displayBoard(row, col);
            cout << "It's a draw!\n";

            PlayerManager playerManager;
            playerManager.updateDraw(names[0], names[1]);

            askSaveReplay();
            break;
        }

        currentPlayerIndex = 1 - currentPlayerIndex;
    }
}

bool Game::handleHumanTurn(const string &playerName,
                           char symbol,
                           int &currentPlayerIndex,
                           int &row,
                           int &col,
                           bool &quitGame)
{
    while (true)
    {
        string line;

        cout << "Enter move for " << playerName
             << " (" << symbol << ") [row col, or help]: ";
        getline(cin >> ws, line);

        if (line == "help")
        {
            showHelp();
            continue;
        }

        if (line == "history")
        {
            showHistory();
            continue;
        }

        if (line == "hint")
        {
            showHint(symbol, symbol == 'X' ? 'O' : 'X');
            continue;
        }

        if (line == "pause")
        {
            cout << "Game paused. Press Enter to resume...";
            cin.get();
            continue;
        }

        if (line == "quit")
        {
            quitGame = true;
            cout << "Game quit.\n";
            return false;
        }

        if (line == "undo")
        {
            if (moveHistory.empty())
            {
                cout << "No move to undo.\n";
                continue;
            }

            Move last = moveHistory.back();
            moveHistory.pop_back();
            board.removeMove(last.row, last.col);
            redoHistory.push_back(last);
            currentPlayerIndex = 1 - currentPlayerIndex;

            cout << "Move undone. It is now " << last.playerName << "'s turn again.\n";
            return false;
        }

        if (line == "redo")
        {
            if (redoHistory.empty())
            {
                cout << "No move to redo.\n";
                continue;
            }

            Move move = redoHistory.back();

            if (move.symbol != symbol)
            {
                cout << "Redo is available on " << move.playerName << "'s turn.\n";
                continue;
            }

            redoHistory.pop_back();

            if (!board.placeMove(move.row, move.col, move.symbol))
            {
                cout << "Redo failed because the cell is no longer available.\n";
                continue;
            }

            row = move.row;
            col = move.col;
            return true;
        }

        for (char &ch : line)
        {
            if (ch == ',' || ch == ';' || ch == '(' || ch == ')')
            {
                ch = ' ';
            }
        }

        stringstream ss(line);

        if (!(ss >> row >> col))
        {
            cout << "Invalid move. Enter two numbers, for example: 0 0.\n";
            continue;
        }

        if (!board.placeMove(row, col, symbol))
        {
            continue;
        }

        return true;
    }
}

void Game::showHistory() const
{
    cout << "\n===== MOVE HISTORY =====\n";

    if (moveHistory.empty())
    {
        cout << "No moves yet.\n";
        return;
    }

    for (const Move &move : moveHistory)
    {
        cout << move.turnNumber << ". "
             << move.playerName << " (" << move.symbol << ") -> "
             << move.row << ' ' << move.col << '\n';
    }
}

void Game::showHelp() const
{
    cout << "\nCommands:\n";
    cout << "- row col : place a move, for example 3 5\n";
    cout << "- undo    : undo the last move\n";
    cout << "- redo    : redo the last undone move\n";
    cout << "- history : show move history\n";
    cout << "- hint    : ask the bot for a suggested move\n";
    cout << "- pause   : pause the game\n";
    cout << "- quit    : quit the current match\n";
}

void Game::showHint(char symbol, char enemySymbol)
{
    int row;
    int col;
    Bot advisor(Expert, symbol, enemySymbol, settings.botStyle);

    advisor.makeMove(board, row, col);

    if (row == -1 || col == -1)
    {
        cout << "No hint available.\n";
        return;
    }

    cout << "Suggested move: " << row << ' ' << col << '\n';
    cout << advisor.getLastExplanation() << '\n';
}

void Game::askSaveReplay()
{
    char saveChoice;

    cout << "Do you want to save the game history? (y/n): ";
    cin >> saveChoice;

    if (saveChoice == 'y' || saveChoice == 'Y')
    {
        ReplayManager replayManager;
        replayManager.saveReplay(moveHistory);
    }
}

int Game::findPlayerIndexBySymbol(const char symbols[2], char symbol) const
{
    if (symbols[0] == symbol)
    {
        return 0;
    }

    return 1;
}

void Game::clearScreen() const
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
