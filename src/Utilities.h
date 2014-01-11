#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
using namespace std;

// Display Width and Height
#define displayX 60
#define displayY 20

// puts a cursor to a given location
void gotoxy(int x, int y);

// redraws the screen with possible delay
void drawBuffer(int delay = 0);

// clears output buffer
void clearBuffer();

// draws window with given width and height
void drawWindow(int width, int height);

// draws text at given coordinates
void drawText(int x, int y, string text);

// animates an opening window to width and height, and
// with given delay in milliseconds
void animateWindowIn(int width, int height, int delay);

// animates a closing window from width and height, and
// with given delay in milliseconds
void animateWindowOut(int width, int height, int delay);

// animates text at given coordinates (X x Y), there is
// delay given in milliseconds between each letter
void animatedText(int x, int y, string text, int delay);

// converts integer to string
string int2str(unsigned int t);