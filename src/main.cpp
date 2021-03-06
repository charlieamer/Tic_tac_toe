#include "Utilities.h"
#include "GameFunctions.h"

////////////////////////////////////////////////////////////
/// MAIN
////////////////////////////////////////////////////////////
#pragma region main
int main()
{
	inputNames();
	while (true)
	{
		switch (mainMenu())
		{
		case 1:
			//New game
			playGame();
			break;
		case 2:
			//Help
			displayHelp();
			break;
		case 3:
			//Connect
			connectToServer();
			break;
		case 4:
			//Host a server
			hostServer();
			break;
		}
	}
}
#pragma endregion main