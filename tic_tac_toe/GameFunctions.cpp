#include "GameFunctions.h"

////////////////////////////////////////////////////////////
/// GLOBAL VARIABLES
////////////////////////////////////////////////////////////
#pragma region global_variables
int gameFieldStatus[4][4];
int score1 = 0, score2 = 0;
char buffer[displayX][displayY];
#pragma endregion global_variables

////////////////////////////////////////////////////////////
/// GAME FUNCTIONS
////////////////////////////////////////////////////////////
#pragma region game_functions
int mainMenu()
{
	animateWindowIn(20, 14, 15);
	animatedText(5, 2, "TIC", 40);
	animatedText(9, 3, "TAC", 40);
	animatedText(13, 4, "TOE", 40);
	Sleep(200);
	animatedText(2, 6, "(1) New game", 20);
	animatedText(2, 7, "(2) Help", 20);
	animatedText(2, 8, "(3) Swap scores", 20);
	animatedText(2, 9, "(4)(Q)(ESC) Quit", 20);
	if (score1 || score2)
	{
		animatedText(1, 11, "Player 1 score: " + int2str(score1), 10);
		animatedText(1, 12, "Player 2 score: " + int2str(score2), 10);
	}
	bool work = true;
	int toRet = 0;
	while (work)
	{
		char c = _getch();
		switch (c)
		{
		case '4':
		case 'q':
		case 'Q':
		case 27:
			exit(0);
		case '1':
		case '2':
		case '3':
			work = false;
			toRet = c - '0'; // Converts an ASCII character to an integer representation
			break;
		default:
			drawText(2, 15, "Please press a number between 1 and 4");
			drawBuffer();
		}
	}
	animateWindowOut(20, 14, 15);
	return toRet;
}

void fieldCoordinate(int inX, int inY, int &outX, int &outY)
{
	outX = inX * 6 + 3;
	outY = inY * 4 + 2;
}

int checkGame()
{
	// check for horizontal win
	if ((gameFieldStatus[0][0] == gameFieldStatus[1][0] && gameFieldStatus[1][0] == gameFieldStatus[2][0]) && gameFieldStatus[0][0])
		return gameFieldStatus[0][0];
	if ((gameFieldStatus[0][1] == gameFieldStatus[1][1] && gameFieldStatus[1][1] == gameFieldStatus[2][1]) && gameFieldStatus[0][1])
		return gameFieldStatus[0][1];
	if ((gameFieldStatus[0][2] == gameFieldStatus[1][2] && gameFieldStatus[1][2] == gameFieldStatus[2][2]) && gameFieldStatus[0][2])
		return gameFieldStatus[0][2];
	// check for vertical win
	if ((gameFieldStatus[0][0] == gameFieldStatus[0][1] && gameFieldStatus[0][1] == gameFieldStatus[0][2]) && gameFieldStatus[0][0])
		return gameFieldStatus[0][0];
	if ((gameFieldStatus[1][0] == gameFieldStatus[1][1] && gameFieldStatus[1][1] == gameFieldStatus[1][2]) && gameFieldStatus[1][0])
		return gameFieldStatus[1][0];
	if ((gameFieldStatus[2][0] == gameFieldStatus[2][1] && gameFieldStatus[2][1] == gameFieldStatus[2][2]) && gameFieldStatus[2][0])
		return gameFieldStatus[2][0];
	// check for diagonal win
	if ((gameFieldStatus[0][0] == gameFieldStatus[1][1] && gameFieldStatus[1][1] == gameFieldStatus[2][2]) && gameFieldStatus[0][0])
		return gameFieldStatus[0][0];
	if ((gameFieldStatus[2][0] == gameFieldStatus[1][1] && gameFieldStatus[1][1] == gameFieldStatus[0][2]) && gameFieldStatus[1][1])
		return gameFieldStatus[1][1];
	// check if all fields are used
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// if there is a field that is not used, it means game is not finished yet
			if (!gameFieldStatus[i][j]) return 3;
		}
	}
	// all fields are used, it is tie
	return 0;
}

void drawGameTable(int playerID, bool drawInfo)
{
	clearBuffer();
	drawWindow(19, 13);
	// horizontal lines
	for (int i = 1; i < 18; i++)
	{
		buffer[i][4] = 205;
		buffer[i][8] = 205;
	}
	// vertical lines
	for (int i = 1; i < 12; i++)
	{
		buffer[6][i] = 186;
		buffer[12][i] = 186;
	}
	// upper joints
	buffer[6][0] = 203;
	buffer[12][0] = 203;
	// bottom joints
	buffer[6][12] = 202;
	buffer[12][12] = 202;
	// left joints
	buffer[0][4] = 204;
	buffer[0][8] = 204;
	// right joints
	buffer[18][4] = 185;
	buffer[18][8] = 185;
	// middle joints
	buffer[6][4] = 206;
	buffer[6][8] = 206;
	buffer[12][4] = 206;
	buffer[12][8] = 206;
	// current game statuses
	int tx, ty;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fieldCoordinate(i, j, tx, ty);
			switch (gameFieldStatus[i][j])
			{
			case 0:
				buffer[tx][ty] = (j * 3) + i + '1';
				break;
			case 1:
				buffer[tx][ty] = 'X';
				buffer[tx+1][ty+1] = 'X';
				buffer[tx-1][ty-1] = 'X';
				buffer[tx-1][ty+1] = 'X';
				buffer[tx+1][ty-1] = 'X';
				break;
			case 2:
				buffer[tx+1][ty] = 'O';
				buffer[tx-1][ty] = 'O';
				buffer[tx][ty+1] = 'O';
				buffer[tx][ty-1] = 'O';
				break;
			}
		}
	}
	if (drawInfo)
	{
		drawText(1, 14, "Select a field where to play. Press Q or ESC to quit");
		drawText(1, 15, "Player " + int2str(playerID + 1) + " (playing as '" + ((playerID == 0) ? "X" : "O") + "')");
	}
	drawBuffer(10);
}

int gameBase()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			gameFieldStatus[i][j] = 0;
		}
	}
	animateWindowIn(19, 13, 10);
	int playerID = 1;
	bool toUpdate = true;
	int setX = 3, setY = 3;
	while (true)
	{
		if (toUpdate)
		{
			gameFieldStatus[setX][setY] = playerID + 1;
			int g = checkGame();
			if (g != 3)
			{
				if (g == 1) score1++;
				if (g == 2) score2++;
				return g;
			}
			playerID = !playerID;
			drawGameTable(playerID);
		}
		char c = _getch();
		if (c == 'Q' || c == 'q' || c == 27) return 0;
		toUpdate = true;
		if (c < '1' || c > '9')
		{
			drawText(1, 17, "Please press a number between 1 and 9");
			drawBuffer();
			toUpdate = false;
		}
		else {
			setX = (c - '0' - 1) % 3;
			setY = (c - '0' - 1) / 3;
			if (gameFieldStatus[setX][setY])
			{
				drawText(1, 19, "That field is already used");
				drawBuffer();
				toUpdate = false;
			}
		}
	}
}

void playGame()
{
	int winner = gameBase();
	drawGameTable(0, false);
	if (winner != 0)
	{
		animatedText(2, 15, "Congratulations player " + int2str(winner) + " !!!", 15);
	}
	else
	{
		animatedText(2, 15, "Better luck next time ;)", 20);
	}
	Sleep(700);
	animateWindowOut(19, 13, 10);
}

void displayHelp()
{
	/*
	This is a two player tic-tac-toe game.
	The goal is to fill a column, row or a
	diagonal with X or O.
	One player is playing as X and the
	other one is playing as O.
	Press numbers on your keyboard to set
	the selected field with your sign.
	If you want to swap signs, you can do
	so by selecting "swap scores" in the
	main menu.
	You can also press Q or ESC on your
	keyboard to return to previous menu.
	*/
	animateWindowIn(42, 19, 10);
	int delay_speed = 5;
	animatedText(2, 2, "This is a two player tic-tac-toe game.", delay_speed);
	animatedText(2, 3, "The goal is to fill a column, row or a", delay_speed);
	animatedText(2, 4, "diagonal with X or O.", delay_speed);
	animatedText(2, 5, "One player is playing as X and the", delay_speed);
	animatedText(2, 6, "other one is playing as O.", delay_speed);
	animatedText(2, 7, "Press numbers on your keyboard to set", delay_speed);
	animatedText(2, 8, "the selected field with your sign.", delay_speed);
	animatedText(2, 9, "If you want to swap signs, you can do", delay_speed);
	animatedText(2, 10, "so by selecting \"swap scores\" in the", delay_speed);
	animatedText(2, 11, "main menu.", delay_speed);
	animatedText(2, 12, "You can also press Q or ESC on your", delay_speed);
	animatedText(2, 13, "keyboard to return to previous menu.", delay_speed);
	gotoxy(4, 15);
	system("pause");
	animateWindowOut(42, 19, 10);
}

void swapScores()
{
	swap(score1, score2);
}
#pragma endregion game_functions