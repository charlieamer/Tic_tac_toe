#pragma once
#include "Utilities.h"
#include <conio.h>

// draws and controls main menu
int mainMenu();

// outputs (outX x outY) a location in window of a given
// game field (inX x inY)// checks if game has been finished
void fieldCoordinate(int inX, int inY, int &outX, int &outY);

// return values meaning:
// 0 - game is tie
// 1 - X won
// 2 - Y won
// 3 - game is not finished
int checkGame();

// Draws game table, displaying which player is currently
// playing, it has also option if you want to display
// additional game info
void drawGameTable(int playerID, bool drawInfo = true);

// Controls game logic and input, returns which user won.
// If it is tie, or user quit, it returns 0
int gameBase();

// Fires up gameBase function and displays an output of a
// finished game (e.g. Player 1 won)
void playGame();

// Displays the aim of this game and how to use this program
void displayHelp();

// Swaps scores of the two players
void swapScores();
