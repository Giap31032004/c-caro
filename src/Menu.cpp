#include "../include/Menu.h"

static GameSettings currentSettings;

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
        cout << "3. Bot vs Bot\n";
        cout << "4. Replay Game\n";
        cout << "5. Player Information\n";
        cout << "6. Search Player\n";
        cout << "7. Find Equivalent Player\n";
        cout << "8. Game Settings\n";
        cout << "9. Guide\n";
        cout << "10. Exit\n";
        cout << "================================\n";
        cout << "Enter your choice: ";

        choice = inputMenuChoice();

        switch (choice)
        {
        case 1:
        {
            Game game(currentSettings);
            game.startPvP();
            break;
        }
        case 2:
            playVsBotMenu();
            break;

        case 3:
            botVsBotMenu();
            break;

        case 4:
            replayMenu();
            break;

        case 5:
        {
            PlayerManager playerManager;
            playerManager.showAllPlayers();
            break;
        }

        case 6:
        {
            PlayerManager playerManager;
            playerManager.searchPlayerByName();
            break;
        }

        case 7:
        {
            PlayerManager playerManager;
            playerManager.findEquivalentPlayer();
            break;
        }

        case 8:
            settingsMenu(currentSettings);
            break;

        case 9:
            showGuide();
            break;

        case 10:
            cout << "Thank you for playing!\n";
            break;

        default:
            cout << "Invalid choice. Please try again!\n";
        }

    } while (choice != 10);
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
        cout << "Enter your choice: ";

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

        case 4:
            break;

        default:
            cout << "Invalid choice. Please try again!\n";
        }

    } while (choice != 4);
}

BotLevel inputBotLevel()
{
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "4. Expert\n";
    cout << "Enter bot level: ";

    int choice = inputMenuChoice();

    switch (choice)
    {
    case 1:
        return Easy;
    case 2:
        return Medium;
    case 3:
        return Hard;
    case 4:
        return Expert;
    default:
        cout << "Invalid level. Hard will be used.\n";
        return Hard;
    }
}

BotStyle inputBotStyle()
{
    cout << "1. Balanced\n";
    cout << "2. Aggressive\n";
    cout << "3. Defensive\n";
    cout << "Enter bot style: ";

    int choice = inputMenuChoice();

    switch (choice)
    {
    case 2:
        return Aggressive;
    case 3:
        return Defensive;
    default:
        return Balanced;
    }
}

void playVsBotMenu()
{
    cout << "\n===== PLAYER VS BOT =====\n";
    BotLevel level = inputBotLevel();
    currentSettings.botStyle = inputBotStyle();

    Game game(currentSettings);
    game.startPlayerVsBot(level);
}

void botVsBotMenu()
{
    cout << "\n===== BOT VS BOT =====\n";
    cout << "Bot 1:\n";
    BotLevel bot1Level = inputBotLevel();

    cout << "Bot 2:\n";
    BotLevel bot2Level = inputBotLevel();

    currentSettings.botStyle = inputBotStyle();

    Game game(currentSettings);
    game.startBotVsBot(bot1Level, bot2Level);
}

void settingsMenu(GameSettings &settings)
{
    int choice;

    do
    {
        cout << "\n===== GAME SETTINGS =====\n";
        cout << "1. Board size: " << settings.boardSize << "x" << settings.boardSize << '\n';
        cout << "2. Win rule: ";

        if (settings.winRule == FiveOrMore)
        {
            cout << "Five or more\n";
        }
        else if (settings.winRule == ExactlyFive)
        {
            cout << "Exactly five\n";
        }
        else
        {
            cout << "Not blocked at both ends\n";
        }

        cout << "3. Random first player: " << (settings.randomFirstPlayer ? "On" : "Off") << '\n';
        cout << "4. Choose X/O before match: " << (settings.chooseSymbol ? "On" : "Off") << '\n';
        cout << "5. Turn time limit: "
             << (settings.timeLimitEnabled ? to_string(settings.turnTimeLimitSeconds) + " seconds" : "Off")
             << '\n';
        cout << "6. Bot style\n";
        cout << "7. Back\n";
        cout << "Enter your choice: ";

        choice = inputMenuChoice();

        switch (choice)
        {
        case 1:
        {
            cout << "Choose board size (10/15/20): ";
            int size = inputMenuChoice();

            if (size == 10 || size == 15 || size == 20)
            {
                settings.boardSize = size;
            }
            else
            {
                cout << "Invalid board size.\n";
            }
            break;
        }

        case 2:
        {
            cout << "1. Five or more\n";
            cout << "2. Exactly five\n";
            cout << "3. Not blocked at both ends\n";
            cout << "Choose win rule: ";
            int rule = inputMenuChoice();

            if (rule == 1)
            {
                settings.winRule = FiveOrMore;
            }
            else if (rule == 2)
            {
                settings.winRule = ExactlyFive;
            }
            else if (rule == 3)
            {
                settings.winRule = NotBlockedBothEnds;
            }
            else
            {
                cout << "Invalid win rule.\n";
            }
            break;
        }

        case 3:
            settings.randomFirstPlayer = !settings.randomFirstPlayer;
            break;

        case 4:
            settings.chooseSymbol = !settings.chooseSymbol;
            break;

        case 5:
        {
            cout << "Enter seconds per turn (0 to disable): ";
            int seconds = inputMenuChoice();

            if (seconds <= 0)
            {
                settings.timeLimitEnabled = false;
            }
            else
            {
                settings.timeLimitEnabled = true;
                settings.turnTimeLimitSeconds = seconds;
            }
            break;
        }

        case 6:
            settings.botStyle = inputBotStyle();
            break;

        case 7:
            break;

        default:
            cout << "Invalid choice. Please try again!\n";
        }

    } while (choice != 7);
}

void showGuide()
{
    cout << "\n========== GAME GUIDE ==========\n";
    cout << "1. Choose board size from 10x10, 15x15, or 20x20 in Game Settings.\n";
    cout << "2. Player 1 can choose X/O, or the first player can be randomized.\n";
    cout << "3. Enter moves as row and column, for example: 3 5.\n";
    cout << "4. In a match, type help to see commands.\n";
    cout << "5. Commands include undo, redo, history, hint, pause, and quit.\n";
    cout << "6. Win rules include five or more, exactly five, and not blocked at both ends.\n";
    cout << "7. Bot levels include Easy, Medium, Hard, and Expert.\n";
    cout << "8. Expert bot uses minimax with alpha-beta pruning.\n";
    cout << "9. Replay lets you watch saved games by replay ID.\n";
    cout << "10. Player Information stores wins, losses, and draws.\n";
    cout << "================================\n";
}
