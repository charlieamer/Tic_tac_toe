#include "Utilities.h"

////////////////////////////////////////////////////////////
/// GLOBAL VARIABLES
////////////////////////////////////////////////////////////
#pragma region global_variables
extern char buffer[displayX][displayY];
char lastBuffer[displayX][displayY];
#pragma endregion global_variables

////////////////////////////////////////////////////////////
/// UTILITY FUNCTIONS
////////////////////////////////////////////////////////////
#pragma region utility_functions
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void drawBuffer(int delay)
{
	for (int i = 0; i < displayX; i++)
	{
		for (int j = 0; j < displayY; j++)
		{
			if (buffer[i][j] == lastBuffer[i][j]) continue; // Makes drawing faster by ignoring characters that haven't changed
			gotoxy(i, j);
			cout << buffer[i][j];
			Sleep(delay);
		}
	}
	memcpy(lastBuffer, buffer, displayX*displayY);
}

// clears output buffer
void clearBuffer()
{
	for (int i = 0; i < displayX; i++)
	{
		for (int j = 0; j < displayY; j++)
		{
			buffer[i][j] = 0;
		}
	}
}

// draws window with given width and height
void drawWindow(int width, int height)
{
	// Upper and lower part
	for (int i = 0; i < width; i++)
	{
		buffer[i][0] = 205;
		buffer[i][height - 1] = 205;
	}

	// Left and right part
	for (int i = 0; i < height; i++)
	{
		buffer[0][i] = 186;
		buffer[width - 1][i] = 186;
	}
	buffer[0][0] = 201;
	buffer[0][height - 1] = 200;
	buffer[width - 1][0] = 187;
	buffer[width - 1][height - 1] = 188;
}

// draws text at given coordinates
void drawText(int x, int y, string text)
{
	for (int i = x; i < min(displayX, (int)(x + text.length())); i++)
	{
		buffer[i][y] = text[i - x];
	}
}

// animates an opening window from width and height, and with given delay in milliseconds
void animateWindowIn(int width, int height, int delay)
{
	for (int i = 1; i <= width; i++)
	{
		clearBuffer();
		drawWindow(i, 1);
		drawBuffer();
		Sleep(delay);
	}
	for (int i = 1; i <= height; i++)
	{
		clearBuffer();
		drawWindow(width, i);
		drawBuffer();
		Sleep(delay * 16 / 10);
	}
}

// animates a closing window from width and height, and with given delay in milliseconds
void animateWindowOut(int width, int height, int delay)
{
	for (int i = height; i > 0; i--)
	{
		clearBuffer();
		drawWindow(width, i);
		drawBuffer();
		Sleep(delay * 16 / 10);
	}
	for (int i = width; i > 0; i--)
	{
		clearBuffer();
		drawWindow(i, 1);
		drawBuffer();
		Sleep(delay);
	}
}

// animates text at given coordinates (X x Y), there is delay given in milliseconds between each letter
void animatedText(int x, int y, string text, int delay)
{
	for (int i = x; i < min(displayX, (int)(x + text.length())); i++)
	{
		buffer[i][y] = text[i - x];
		drawBuffer();
		Sleep(delay);
	}
}

// converts integer to string
string int2str(unsigned int t)
{
	if (t == 0) return "0";
	string ret;
	while (t)
	{
		ret = (char)((t % 10) + '0') + ret;
		t /= 10;
	}
	return ret;
}
#pragma endregion utility_functions