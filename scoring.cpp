#include "Scoring.h"

void Scoring::ClearedLines(int linesCleared, unsigned long gameSpeed)
{
  int baseValue = 0;
  if (linesCleared == 1)
  {
    baseValue=1;
  } 
  else if(linesCleared == 2)
  {
    baseValue=3;
  }
  else if(linesCleared == 3)
  {
    baseValue=5;
  }
  else if(linesCleared == 4)
  {
    baseValue=8;
  }

  if(gameSpeed < 100)
  {
    baseValue*=4;
  } 
  else if(gameSpeed < 200)
  {
    baseValue*=3;
  }
  else if(gameSpeed < 300)
  {
    baseValue*=2;
  }

  m_Score += baseValue;
}
