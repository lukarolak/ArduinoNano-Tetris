#include <Arduino.h>
#include "tetrominoimplementation.h"
#include "rendering.h"

GenericTetromino g_Tetromino;
Tetromino g_TetrominoStack[TetrominoStackLength] = {Tetromino::Empty, Tetromino::Empty, Tetromino::Empty, Tetromino::Empty}; 
Tetromino g_HoldTetromino = Tetromino::Empty;
bool g_TetrominoExchangedWithHold = false;

Tetromino PickRandomTetronimo()
{
  int type = random(6);
  if(type == 0)   
  {
    return Tetromino::Long;
  }
  else if(type == 1)
  {
    return Tetromino::Square;
  }
  else if(type == 2)
  {
    return Tetromino::L;
  }
  else if(type == 3)
  {
    return Tetromino::T;
  }
  else if(type == 4)
  {
    return Tetromino::J;
  }
  else if(type == 5)
  {
    return Tetromino::Z;
  }
}

void InitializeTetronimoStack()
{
  g_TetrominoStack[0] = PickRandomTetronimo();
  g_TetrominoStack[1] = PickRandomTetronimo();
  g_TetrominoStack[2] = PickRandomTetronimo();
  g_TetrominoStack[3] = PickRandomTetronimo();
}

void InitializeTetronimoSystem()
{
  InitializeTetronimoStack();
}

Tetromino* GetTetrominoStack()
{
  return g_TetrominoStack;
}

Tetromino GetTetrominoFromStack()
{
  Tetromino tetronimo = g_TetrominoStack[0];
  g_TetrominoStack[0] = g_TetrominoStack[1];
  g_TetrominoStack[1] = g_TetrominoStack[2];
  g_TetrominoStack[2] = g_TetrominoStack[3];
  g_TetrominoStack[3] = PickRandomTetronimo();
  return tetronimo;
}

void HoldTetromino()
{
  if(g_TetrominoExchangedWithHold)
  {
    return;
  }

 if(g_HoldTetromino == Tetromino::Empty)
 {
  g_HoldTetromino = GetTetrominoFromStack();
 }
 else
 {
  Tetromino temp = g_HoldTetromino;
  g_HoldTetromino = g_TetrominoStack[0];
  g_TetrominoStack[0] = temp;
 }

 g_Tetromino = GenerateTetronimo(GetTetrominoFromStack());
 g_TetrominoExchangedWithHold = true;
}

Tetromino GetHoldTetromino()
{
  return g_HoldTetromino;
}

GenericTetromino GenerateTetronimo(Tetromino type)
{
  GenericTetromino tetronimo;
  tetronimo.type = type;
  if (type == Tetromino::Long)
  {
            tetronimo.blocks[0].x = 4;
            tetronimo.blocks[1].x = 4;
            tetronimo.blocks[2].x = 4;
            tetronimo.blocks[3].x = 4;
            tetronimo.blocks[0].y = 1;
            tetronimo.blocks[1].y = 2;
            tetronimo.blocks[2].y = 3;
            tetronimo.blocks[3].y = 4;
  }
  else if(type == Tetromino::Square)
  {
            tetronimo.blocks[0].x = 3;
            tetronimo.blocks[1].x = 3;
            tetronimo.blocks[2].x = 4;
            tetronimo.blocks[3].x = 4;
            tetronimo.blocks[0].y = 1;
            tetronimo.blocks[1].y = 2;
            tetronimo.blocks[2].y = 1;
            tetronimo.blocks[3].y = 2;
  }
  else if(type == Tetromino::L)
  {
            tetronimo.blocks[0].x = 3;
            tetronimo.blocks[1].x = 4;
            tetronimo.blocks[2].x = 5;
            tetronimo.blocks[3].x = 3;
            tetronimo.blocks[0].y = 1;
            tetronimo.blocks[1].y = 2;
            tetronimo.blocks[2].y = 2;
            tetronimo.blocks[3].y = 2;
  }
  else if(type == Tetromino::T)
  {
            tetronimo.blocks[0].x = 4;
            tetronimo.blocks[1].x = 4;
            tetronimo.blocks[2].x = 5;
            tetronimo.blocks[3].x = 3;

            tetronimo.blocks[0].y = 1;
            tetronimo.blocks[1].y = 2;
            tetronimo.blocks[2].y = 2;
            tetronimo.blocks[3].y = 2;
  }
  else if(type == Tetromino::J)
  {
            tetronimo.blocks[0].x = 5;
            tetronimo.blocks[1].x = 4;
            tetronimo.blocks[2].x = 5;
            tetronimo.blocks[3].x = 3;
            tetronimo.blocks[0].y = 1;
            tetronimo.blocks[1].y = 2;
            tetronimo.blocks[2].y = 2;
            tetronimo.blocks[3].y = 2;
  }
  else
  {
            tetronimo.blocks[0].x = 5;
            tetronimo.blocks[1].x = 4;
            tetronimo.blocks[2].x = 4;
            tetronimo.blocks[3].x = 3;
            tetronimo.blocks[0].y = 2;
            tetronimo.blocks[1].y = 1;
            tetronimo.blocks[2].y = 2;
            tetronimo.blocks[3].y = 1;
  }
  return tetronimo;
}

bool TetrominoInValidLocation(const GenericTetromino& tetromino, const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight])
{
  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    const Block& block = tetromino.blocks[i];
    
    if (block.x >= PlayfieldWidth || block.y >= PlayfieldHeight)
    {
      return false;
    }
    if (block.x < 0 || block.y <= 0)
    {
      return false;
    }
    if (playfield[block.x][block.y] != Tetromino::Empty)
    {
      return false;
    }
  }
  return true;
}

bool GenerateTetromino(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight])
{
  g_TetrominoExchangedWithHold = false;

  g_Tetromino = GenerateTetronimo(GetTetrominoFromStack());
  return TetrominoInValidLocation(g_Tetromino, playfield);
}

void RotateTetromino(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight], const int loopCounter = 0){

  GenericTetromino tetrominoBeforeRotation = g_Tetromino;

  if(tetrominoBeforeRotation.type == Tetromino::Long) {
      //Vertical
      if(tetrominoBeforeRotation.blocks[0].x == g_Tetromino.blocks[1].x) {
          int newY = tetrominoBeforeRotation.blocks[2].y ;
          int newX = tetrominoBeforeRotation.blocks[2].x;
          for(int i = 0 ; i < 4; i++)
              tetrominoBeforeRotation.blocks[i].y = newY;
          tetrominoBeforeRotation.blocks[3].x = newX - 1;
          tetrominoBeforeRotation.blocks[1].x = newX +1;
          tetrominoBeforeRotation.blocks[0].x = newX +2; 
          return;
      }

      //Horizontal
      if(tetrominoBeforeRotation.blocks[0].y == tetrominoBeforeRotation.blocks[1].y)  {
          int newX = tetrominoBeforeRotation.blocks[1].x;
          int newY = tetrominoBeforeRotation.blocks[1].y;
          for(int i = 0 ; i < 4; i++)
              tetrominoBeforeRotation.blocks[i].x = newX;
          tetrominoBeforeRotation.blocks[0].y = newY + 1;
          tetrominoBeforeRotation.blocks[2].y = newY -1 ;
          tetrominoBeforeRotation.blocks[3].y = newY - 2;   
      } 
  }
      
  if(tetrominoBeforeRotation.type == Tetromino::T || tetrominoBeforeRotation.type == Tetromino::L || tetrominoBeforeRotation.type == Tetromino::J){
      for( int i = 0 ; i < 4 ; i++ ){
          int prev_x = tetrominoBeforeRotation.blocks[i].x;
          int prev_y = tetrominoBeforeRotation.blocks[i].y;
          tetrominoBeforeRotation.blocks[i].x  = prev_y   -  tetrominoBeforeRotation.blocks[1].y +tetrominoBeforeRotation.blocks[1].x ;
          tetrominoBeforeRotation.blocks[i].y = -prev_x  + tetrominoBeforeRotation.blocks[1].x + tetrominoBeforeRotation.blocks[1].y ;
      }
  }

  if(tetrominoBeforeRotation.type == Tetromino::S || tetrominoBeforeRotation.type == Tetromino::Z) {
      int newX = tetrominoBeforeRotation.blocks[2].x;
      int newY = tetrominoBeforeRotation.blocks[2].y;
      
      if(tetrominoBeforeRotation.blocks[1].y == tetrominoBeforeRotation.blocks[3].y) {
          tetrominoBeforeRotation.blocks[3].x = newX + 1;
          tetrominoBeforeRotation.blocks[3].y = newY - 1;
      }
      else if(tetrominoBeforeRotation.blocks[0].x == tetrominoBeforeRotation.blocks[3].x){
          tetrominoBeforeRotation.blocks[3].x = newX - 1;
          tetrominoBeforeRotation.blocks[3].y = newY + 1;
      }
  }

  if (TetrominoInValidLocation(tetrominoBeforeRotation, playfield))
  {
    g_Tetromino = tetrominoBeforeRotation;
  }
  else
  {
    //Try all possible rotations
    if(loopCounter == 2)
    {
      return;
    }
    RotateTetromino(playfield, loopCounter+1);
  }
}

GenericTetromino GetActiveTetromino()
{
  return g_Tetromino;
}

bool MoveTetrominoDown(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight])
{
  GenericTetromino tetrominoBeforeRotation = g_Tetromino;

  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    tetrominoBeforeRotation.blocks[i].y++;
  }

  if (TetrominoInValidLocation(tetrominoBeforeRotation, playfield))
  {
    g_Tetromino = tetrominoBeforeRotation;
    return true;
  }
  else
  {
    return false;
  }
}

void MoveTetrominoLeft(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight])
{
  GenericTetromino tetrominoBeforeRotation = g_Tetromino;

  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    tetrominoBeforeRotation.blocks[i].x--;
  }

  if (TetrominoInValidLocation(tetrominoBeforeRotation, playfield))
  {
    g_Tetromino = tetrominoBeforeRotation;
  }
}

void MoveTetrominoRight(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight])
{
  GenericTetromino tetrominoBeforeRotation = g_Tetromino;

  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    tetrominoBeforeRotation.blocks[i].x++;
  }

  if (TetrominoInValidLocation(tetrominoBeforeRotation, playfield))
  {
    g_Tetromino = tetrominoBeforeRotation;
  }
}