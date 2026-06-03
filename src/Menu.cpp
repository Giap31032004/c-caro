#include "../include/Menu.h"



int inputMenuChoice()
{
    int choice;

    while (true)
    {
        if (cin >> choice)
        {
            return choice;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
        cout << "Enter your choice: ";
    }
}

void menu()
{
    int choice;

    do
    {
        cout << "\n========== CARO GAME ==========\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs Bot\n";
        cout << "3. Replay Game\n";
        cout << "4. Player Information\n";
        cout << "5. Search Player\n";
        cout << "6. Find Equivalent Player\n";
        cout << "7. Guide\n";
        cout << "8. Exit\n";
        cout << "================================\n";

        cout << "Enter your choice: ";
        choice = inputMenuChoice();

        switch (choice)
        {
        case 1:
        {
            Game game;
            game.startPvP();
            break;
        }
        case 2:
        {
            playVsBotMenu();
            break;
        }
        case 3:
        {
            replayMenu();
            break;
        }

        case 4:
        {
            PlayerManager playerManager;
            playerManager.showAllPlayers();
            break;
        }

        case 5:
        {
            PlayerManager playerManager;
            playerManager.searchPlayerByName();
            break;
        }

        case 6:
        {
            PlayerManager playerManager;
            playerManager.findEquivalentPlayer();
            break;
        }

        case 7:
        {
            showGuide();
            break;
        }

        case 8:
            cout << "Thank you for playing!\n";
            break;

        default:
            cout << "Invalid choice. Please try again!\n";
        }

    } while (choice != 8);
}

void replayMenu()
{

    ReplayManager replayManager;

    int choice;

    do
    {
        cout << "\n===== REPLAY MENU =====\n";
        cout << "1. Show Replay List\n";
        cout << "2. Watch Replay By ID\n";
        cout << "3. Delete Replay By ID\n";
        cout << "4. Back\n";

        choice = inputMenuChoice();

        switch (choice)
        {
        case 1:
            replayManager.showReplayList();
            break;

        case 2:
            replayManager.replayById();
            break;

        case 3:
            replayManager.deleteReplayById();
            break;
        }

    } while (choice != 4);
}

void playVsBotMenu()
{
    int choice;

    do
    {
        cout << "\n===== BOT MENU =====\n";
        cout << "1. Easy Bot\n";
        cout << "2. Medium Bot\n";
        cout << "3. Hard Bot\n";
        cout << "4. Back\n";
        cout << "Enter your choice: ";
        choice = inputMenuChoice();

        switch (choice)
        {
        case 1:
        {
            Game game;
            game.startPlayerVsBot(Easy);
            break;
        }

        case 2:
        {
            Game game;
            game.startPlayerVsBot(Medium);
            break;
        }

        case 3:
        {
            Game game;
            game.startPlayerVsBot(Hard);
            break;
        }

        case 4:
            break;

        default:
            cout << "Invalid choice. Please try again!\n";
        }

    } while (choice != 4);
}

void showGuide()
{
    cout << "\n========== GAME GUIDE ==========\n";
    cout << "1. Board size: 10x10.\n";
    cout << "2. Player 1 uses X, Player 2/Bot uses O.\n";
    cout << "3. Enter move by row and column, for example: 3 5.\n";
    cout << "4. Row and column must be from 0 to 9.\n";
    cout << "5. You cannot choose a cell that already has X or O.\n";
    cout << "6. The first player who has 5 symbols in a row wins.\n";
    cout << "7. Winning lines can be horizontal, vertical, or diagonal.\n";
    cout << "8. If the board is full and nobody wins, the game is draw.\n";
    cout << "9. Replay lets you watch saved games by replay ID.\n";
    cout << "10. Player Information stores wins, losses, and draws.\n";
    cout << "================================\n";
}