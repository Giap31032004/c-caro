#include "../include/Game.h"

#include <iostream>

using namespace std;

Game::Game()
    : player1("Player 1", 'X'),
      player2("Player 2", 'O')
{
}

void Game::startPvP()
{
    player1.inputName();
    player2.inputName();

    board.resetBoard();
    moveHistory.clear();

    Player *currentPlayer = &player1;

    while (true)
    {
        board.displayBoard();

        int row, col;

        currentPlayer->inputMove(row, col);

        if (!board.placeMove(row, col, currentPlayer->getSymbol()))
        {
            continue;
        }

        moveHistory.push_back(
            {row, col, currentPlayer->getSymbol()});

        if (board.checkWin(row, col, currentPlayer->getSymbol()))
        {
            board.displayBoard();

            cout << currentPlayer->getName() << " wins!\n";

            PlayerManager playerManager;

            if (currentPlayer == &player1)
            {
                playerManager.updateWinLoss(
                    player1.getName(),
                    player2.getName());
            }
            else
            {
                playerManager.updateWinLoss(
                    player2.getName(),
                    player1.getName());
            }

            char saveChoice;

            cout << "Do you want to save the game history? (y/n): ";
            cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y')
            {
                ReplayManager replayManager;
                replayManager.saveReplay(moveHistory);
            }

            break;
        }

        if (board.checkFull())
        {
            board.displayBoard();

            cout << "It's a draw!\n";

            PlayerManager playerManager;

            playerManager.updateDraw(
                player1.getName(),
                player2.getName());

            char saveChoice;

            cout << "Do you want to save the game history? (y/n): ";
            cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y')
            {
                ReplayManager replayManager;
                replayManager.saveReplay(moveHistory);
            }

            break;
        }

        if (currentPlayer == &player1)
        {
            currentPlayer = &player2;
        }
        else
        {
            currentPlayer = &player1;
        }
    }
}

void Game::startPlayerVsBot(BotLevel selectedLevel)
{
    player1.inputName();

    Bot bot(
        selectedLevel,
        'O',
        player1.getSymbol());

    board.resetBoard();
    moveHistory.clear();

    bool playerTurn = true;

    while (true)
    {
        board.displayBoard();

        int row, col;
        char currentSymbol;

        if (playerTurn)
        {
            player1.inputMove(row, col);
            currentSymbol = player1.getSymbol();

            if (!board.placeMove(row, col, currentSymbol))
            {
                continue;
            }
        }
        else
        {
            bot.makeMove(board, row, col);
            currentSymbol = bot.getSymbol();

            board.placeMove(row, col, currentSymbol);

            cout << "Bot moved at: " << row << " " << col << endl;
        }

        moveHistory.push_back(
            {row, col, currentSymbol});

        if (board.checkWin(row, col, currentSymbol))
        {
            board.displayBoard();

            if (playerTurn)
            {
                cout << player1.getName() << " wins!\n";
            }
            else
            {
                cout << "Bot wins!\n";
            }

            char saveChoice;

            cout << "Do you want to save the game history? (y/n): ";
            cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y')
            {
                ReplayManager replayManager;
                replayManager.saveReplay(moveHistory);
            }

            break;
        }

        if (board.checkFull())
        {
            board.displayBoard();

            cout << "It's a draw!\n";

            char saveChoice;

            cout << "Do you want to save the game history? (y/n): ";
            cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y')
            {
                ReplayManager replayManager;
                replayManager.saveReplay(moveHistory);
            }

            break;
        }

        playerTurn = !playerTurn;
    }
}