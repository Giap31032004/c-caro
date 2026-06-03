#ifndef MENU_H
#define MENU_H
#include<iostream>
#include<limits>
#include "../include/Game.h"
#include "../include/ReplayManager.h"
#include "../include/Player.h"
#include "../include/PlayerManager.h"

using namespace std;
int inputMenuChoice();
void menu();
void replayMenu();
void playVsBotMenu();
void botVsBotMenu();
void settingsMenu(GameSettings &settings);
BotLevel inputBotLevel();
BotStyle inputBotStyle();
void showGuide();

#endif
