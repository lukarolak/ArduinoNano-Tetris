#include <TFT.h>  
#include <SPI.h>
#include "consts.h"
#include "rendering.h"

// pin definition for Arduino UNO
#define cs   10
#define dc   9
#define rst  8

constexpr int playfieldHorizontalPadding = 5;
constexpr int playfieldVerticalPadding = 5;
constexpr int pixelDrawSize = 6;

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

uint16_t GetTetrominoColor(Tetromino type)
{
  //Add colors here
  switch(type)
  {
    case Tetromino::J:
      return 0xA00A;
    case Tetromino::L:
      return 0x0AA0;
    case Tetromino::Long:
      return 0x33A1FF;
    case Tetromino::S:
      return 0xFF33A1;
    case Tetromino::Square:
      return 0x008F;
    case Tetromino::T:
      return 0xF800;
    case Tetromino::Z:
      return 0xFF6347;
    case Tetromino::Empty:
      return 0x000000;
  }
}

void InitializeRendering()
{
  TFTscreen.begin();
  ClearScreen();
  TFTscreen.stroke(255,255,255);
  TFTscreen.setTextSize(2);
}

void DrawPixel(int posX, int posY, Tetromino type)
{
  posX = posX * pixelDrawSize + playfieldHorizontalPadding;
  posY = posY * pixelDrawSize + playfieldVerticalPadding;
  
  uint16_t color = GetTetrominoColor(type);

  for (int x = posX; x < posX+pixelDrawSize; x++)
  {
    for (int y = posY+pixelDrawSize; y > posY; y--)
    {
      TFTscreen.drawPixel(y,TFTscreen.height()-x, color);
    }
  }
}

void ClearScreen()
{
  TFTscreen.background(0, 0, 0);
}

void DrawPlayfieldBorder()
{
  constexpr uint16_t colorWhite = 0xFFFFFF;
  constexpr uint16_t xMax = PlayfieldWidth*pixelDrawSize+2;
  uint16_t x = TFTscreen.height() + 2 - (xMax+playfieldHorizontalPadding);
  constexpr uint16_t y = playfieldVerticalPadding-2;
  constexpr uint16_t yMax = PlayfieldHeight*pixelDrawSize+4;

  TFTscreen.drawRect(y, x, yMax, xMax, colorWhite);
}

void DisplayScore(unsigned int score)
{
  static unsigned int previousScore = 0;
  constexpr uint16_t x = PlayfieldWidth*pixelDrawSize+16;
  constexpr uint16_t y = playfieldVerticalPadding-2;

  TFTscreen.stroke(0,0,0);
  TFTscreen.setRotation(0);
  TFTscreen.text(String(previousScore).c_str(),x,y);
  TFTscreen.setRotation(1);

  constexpr uint8_t redColor = 255;
  constexpr uint8_t greenColor = 255;
  constexpr uint8_t blueColor = 255;

  TFTscreen.stroke(255,255,255);
  TFTscreen.setRotation(0);
  TFTscreen.text(String(score).c_str(),x,y);
  TFTscreen.setRotation(1);
  previousScore = score;
}