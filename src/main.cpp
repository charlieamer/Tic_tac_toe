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
		}
	}
}
#pragma endregion main