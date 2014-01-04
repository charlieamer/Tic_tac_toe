#include "Utilities.h"
#include "GameFunctions.h"

////////////////////////////////////////////////////////////
/// MAIN
////////////////////////////////////////////////////////////
#pragma region main
int main()
{
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
			//Swap scores
			swapScores();
			break;
		}
	}
}
#pragma endregion main