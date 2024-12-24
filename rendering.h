#pragma once

#include "tetromino.h"

void InitializeRendering();
void DrawPixel(int posX, int posY, Tetromino type);
void ClearScreen();
void DrawPlayfieldBorder();
int GetDrawPixelCalls();
void DisplayScore(unsigned int score);