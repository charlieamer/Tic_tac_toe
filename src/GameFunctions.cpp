#include "GameFunctions.h"

#define POCO_STATIC
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/NetException.h>

////////////////////////////////////////////////////////////
/// GLOBAL VARIABLES
////////////////////////////////////////////////////////////
#pragma region global_variables
int gameFieldStatus[4][4];
int score1 = 0, score2 = 0;
char buffer[displayX][displayY];
string name1, name2;
unsigned char oddGame = 0;
int netReady; // 0 - no, 1 - recieve, 2 - send
unsigned char netChar;
string lastIp, lastPort;
Poco::Net::TCPServer* server;
Poco::Net::StreamSocket* client;
#pragma endregion global_variables

////////////////////////////////////////////////////////////
/// INTERNET FUNCTIONS
////////////////////////////////////////////////////////////
#pragma region internet_functions

class HostConnection :public Poco::Net::TCPServerConnection
{
public:
	HostConnection(const Poco::Net::StreamSocket &s) : Poco::Net::TCPServerConnection(s) {};
	unsigned char getChar()
	{
		unsigned char buffer[1] = {'q'};
		socket().receiveBytes(buffer, 1);
		if (buffer[0] == 255) socket().close();
		return buffer[0];
	}

	void sendChar(unsigned char c)
	{
		unsigned char buffer[1];
		buffer[0] = c;
		socket().sendBytes(buffer,1);
	}

	void run()
	{
		sendChar(oddGame);
		for (;;)
		{
			if (netReady == 1)
			{
				netChar = getChar();
				netReady = 0;
			}
			if (netReady == 2)
			{
				sendChar(netChar);
				netReady = 0;
			}
			Sleep(100);
		}
		socket().close();
	}
};

void connectToServer()
{
	animateWindowIn(34, 7, 15);
	drawText(2, 2, "Press enter to use last values");
	drawBuffer();
	drawText(2, 3, "IP: ");
	drawBuffer();
	string ip;
	string port;
	getline(cin, ip);
	getline(cin, ip);
	if (ip=="") ip = lastIp;
	drawText(2, 4, "Port: ");
	drawBuffer();
	getline(cin, port);
	if (port == "") port = lastPort;
	lastPort = port;
	lastIp = ip;
	const Poco::Net::SocketAddress address(ip + ":" + port);
	client = new Poco::Net::StreamSocket(address);
	client->receiveBytes(&oddGame, 1);
	animateWindowOut(34, 6, 15);
	playGame(2);
}

void hostServer()
{
	Poco::Net::TCPServerParams *serverParams = new Poco::Net::TCPServerParams();
	serverParams->setMaxQueued(1);
	serverParams->setMaxThreads(1);
	Poco::Net::ServerSocket srvsocket(8080);
	server = new Poco::Net::TCPServer(new Poco::Net::TCPServerConnectionFactoryImpl<HostConnection>(), srvsocket, serverParams);
	server->start();
	animateWindowIn(14, 5, 15);
	drawText(2, 2, "Waiting...");
	drawBuffer();
	while (server->currentConnections() == 0)
	{
		Sleep(100);
	}
	animateWindowOut(14, 5, 15);
	playGame(1);
	srvsocket.close();
	delete server;
}
#pragma endregion internet_functions

////////////////////////////////////////////////////////////
/// GAME FUNCTIONS
////////////////////////////////////////////////////////////
#pragma region game_functions

char gameInput(int mode, int playerID)
{
	if (mode == 0 || (mode == 1 && playerID == 0) || (mode == 2 && playerID == 1))
	{
		char c = _getch();
		return c;
	}
	if (mode == 1)
	{
		netChar = 0;
		netReady = 1;
		while (netReady && netChar==0) Sleep(100);
		return netChar;
	}
	if (mode == 2)
	{
		char c = 27;
		client->receiveBytes(&c,1);
		return c;
	}
}

void inputNames()
{
	cout << "Input your name (player 1): ";
	cin >> name1;
	cout << "Input oponnent's name (player 2): ";
	cin >> name2;
	system("cls");
	
}

int mainMenu()
{
	animateWindowIn(20, 14, 15);
	animatedText(5, 2, "TIC", 30);
	animatedText(9, 3, "TAC", 30);
	animatedText(13, 4, "TOE", 30);
	Sleep(150);
	animatedText(2, 6, "(1) New game", 15);
	animatedText(2, 7, "(2) Help", 15);
	animatedText(2, 8, "(3) Connect", 15);
	animatedText(2, 9, "(4) Host", 15);
	animatedText(2, 10, "(5)(Q)(ESC) Quit", 15);
	animatedText(2, 11, name1 + ": " + int2str(score1), 10);
	animatedText(2, 12, name2 + ": " + int2str(score2), 10);
	bool work = true;
	int toRet = 0;
	while (work)
	{
		char c = _getch();
		switch (c)
		{
		case '5':
		case 'q':
		case 'Q':
		case 27:
			exit(0);
		case '1':
		case '2':
		case '3':
		case '4':
			work = false;
			toRet = c - '0'; // Converts an ASCII character to an integer representation
			break;
		default:
			drawText(2, 15, "Please press a number between 1 and 5");
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

void drawGameTable(int playerID, bool drawInfo, bool invertSign)
{
	clearBuffer();
	drawWindow(19, 13);
	// horizontal lines
	for (int i = 1; i < 18; i++)
	{
#ifdef _WIN32 || _WIN64
		buffer[i][4] = 205;
		buffer[i][8] = 205;
#else
		buffer[i][4] = '-';
		buffer[i][8] = '-';
#endif // _WIN32
	}
	// vertical lines
	for (int i = 1; i < 12; i++)
	{
#ifdef _WIN32 || _WIN64
		buffer[6][i] = 186;
		buffer[12][i] = 186;
#else
		buffer[6][i] = '|';
		buffer[12][i] = '|';
#endif
	}
#ifdef _WIN32 || _WIN64
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
#else
	// upper joints
	buffer[6][0] = '+';
	buffer[12][0] = '+';
	// bottom joints
	buffer[6][12] = '+';
	buffer[12][12] = '+';
	// left joints
	buffer[0][4] = '+';
	buffer[0][8] = '+';
	// right joints
	buffer[18][4] = '+';
	buffer[18][8] = '+';
	// middle joints
	buffer[6][4] = '+';
	buffer[6][8] = '+';
	buffer[12][4] = '+';
	buffer[12][8] = '+';
#endif // _WIN32
	// current game statuses
	int tx, ty;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fieldCoordinate(i, j, tx, ty);
			int stat = gameFieldStatus[i][j];
			if (oddGame && gameFieldStatus[i][j]) stat=(!(gameFieldStatus[i][j]-1))+1;
			switch (stat)
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
		drawText(1, 15, ((playerID == 0) ? name1 : name2) + "'s turn (playing as '" + ((playerID ^ oddGame ^ invertSign == 0) ? "X" : "O") + "')");
	}
	drawBuffer(10);
}

int gameBase(int mode)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			gameFieldStatus[i][j] = 0;
		}
	}
	animateWindowIn(19, 13, 10);
	int playerID = !oddGame;
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
			drawGameTable(playerID ^ (mode == 2), true, mode == 2);
		}
		char c = gameInput(mode, playerID);
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
			} else
			{
				if (mode == 1 && playerID == 0)
				{
					netChar = c;
					netReady = 2;
				}
				if (mode == 2 && playerID == 1)
				{
					client->sendBytes(&c, 1);
				}
			}
		}
	}
}

void playGame(int mode)
{
	int winner = gameBase(mode);
	if (winner)
	{
		if (mode == 2) winner = (!(winner-1))+1;
	}
	drawGameTable(0, false);
	if (winner != 0)
	{
		animatedText(2, 15, "Congratulations player " + int2str(winner) + " !!!", 15);
	}
	else
	{
		animatedText(2, 15, "Better luck next time ;)", 20);
	}
	oddGame = !oddGame;
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
	//system("pause");
	cout << "Press any key to continue ...";
	_getch();
	animateWindowOut(42, 19, 10);
}

void swapScores()
{
	swap(score1, score2);
}
#pragma endregion game_functions
