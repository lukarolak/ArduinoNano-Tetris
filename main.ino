#include "rendering.h"
#include "tetrominoimplementation.h"
#include "consts.h"
#include "scoring.h"
#include "input.h"

Tetromino Playfield[PlayfieldWidth][PlayfieldHeight];
static unsigned long gameSpeed = 1300;
static unsigned long timeSinceLastMoveDown = millis();
Scoring g_Scoring;

void(* resetFunc) (void) = 0;

void reset()
{
  resetFunc();
}

void ClearTetrominoStack()
{
  Tetromino* oldUpcomingTetronimos = GetTetrominoStack();
  for (int i = 0; i < TetrominoStackLength; i++)
  {
    DrawEmptyTetromino(i, oldUpcomingTetronimos[i]);
  }
}

void DrawTetrominoStack()
{
  Tetromino* upcomingTetronimos = GetTetrominoStack();
  for (int i = 0; i < TetrominoStackLength; i++)
  {
    DrawNextTetromino(i, upcomingTetronimos[i]);
  }
}

bool CreateNewTetronimo()
{
  ClearTetrominoStack();
  bool success = GenerateTetromino(Playfield);
  DrawTetrominoStack();
  return success;
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  InitializeRendering();
  InitializeTetronimoSystem();
  DrawPlayfieldBorder();
  InitializePlayfield();
  CreateNewTetronimo();
  DisplayScore(g_Scoring.GetScore());
  InitializeInput();
}

void loop() {
  ClearCompletedLines();
  UpdateTetromino();
}

void DrawField() {
  for (int x = 0; x < PlayfieldWidth; x++)
  {
    for (int y = 0; y < PlayfieldHeight; y++)
    {
      Tetromino blockType = Playfield[x][y];
      DrawPixel(x,y, blockType);
    }
  }
}

void DrawActiveTetromino()
{
  DrawTetromino(GetActiveTetromino());
}

void DrawTetromino(const GenericTetromino& tetromino)
{
  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    const Block& block = tetromino.blocks[i];
    DrawPixel(block.x, block.y, tetromino.type);
  }
}

void DrawActiveTetrominoAsBlank()
{
  GenericTetromino activeTetromino = GetActiveTetromino();
  activeTetromino.type = Tetromino::Empty;
  DrawTetromino(activeTetromino);
}

void InitializePlayfield()
{
  for (int x = 0; x < PlayfieldWidth; x++)
  {
    for (int y = 0; y < PlayfieldHeight; y++)
    {
      Playfield[x][y] = Tetromino::Empty;
    }
  }
}

void ClearCompletedLines()
{
  int linesCleared = 0;
  for (int y = PlayfieldHeight - 1; y >= 0; y--)
  {
    bool isCompleted = true;
    for (int x = 0; x < PlayfieldWidth; x++)
    {
      if (Playfield[x][y] == Tetromino::Empty)
      {
        isCompleted = false;
        break;
      }
    }

    if (isCompleted)
    {
      linesCleared++;
      for (int yy = y-1; yy >= 0; yy--)
      {
        for (int x = 0; x < PlayfieldWidth; x++)
        {
          Playfield[x][yy+1] = Playfield[x][yy];
        }
      }

      //Always clear top row
      for (int x = 0; x < PlayfieldWidth; x++)
      {
        Playfield[x][0] = Tetromino::Empty;
      }
      
      y++;
    }
  }

  if(linesCleared>0)
  {
    g_Scoring.ClearedLines(linesCleared, gameSpeed);
    DisplayScore(g_Scoring.GetScore());
    DrawField();
  }
}

void SpeedUpTheGame()
{
    if (gameSpeed > 50)
    {
      gameSpeed -= 1;
    }
}

void DropTetromino()
{
  while(MoveTetrominoDown())
  {

  }
}

void UpdateTetromino()
{
  InputActions actions = GetInput();

  if(actions.moveLeft)
  {
    MoveTetrominoLeft();
  }
  if(actions.moveRight)
  {
    MoveTetrominoRight();
  }
  
  //No button for this
  //DropTetromino();
  if(actions.moveDown)
  {
    MoveTetrominoDown();
  }
  if(actions.rotate)
  {
    RotateTetromino();
  }
  if(actions.hold)
  {
    DrawTetronimoHold();
  }
  
  unsigned long currentTime = millis();
  if (currentTime - timeSinceLastMoveDown >= gameSpeed)
  {
    SpeedUpTheGame();
    MoveTetrominoDown();
  }
}

void DrawTetronimoHold()
{
  DrawEmptyHold(GetHoldTetromino());
  ClearTetrominoStack();
  DrawActiveTetrominoAsBlank();
  HoldTetromino();
  DrawActiveTetromino();
  DrawTetrominoStack();
  DrawHoldStack(GetHoldTetromino());
}

void RotateTetromino()
{
  DrawActiveTetrominoAsBlank();
  RotateTetromino(Playfield);
  DrawActiveTetromino();
}

bool MoveTetrominoDown()
{
  DrawActiveTetrominoAsBlank();
  bool sucess = MoveTetrominoDown(Playfield);
  DrawActiveTetromino();

  timeSinceLastMoveDown = millis();

  if (!sucess)
  {
    CopyTetrominoToPlayfield();
    bool canCreateNewTetromino = CreateNewTetronimo();
    if (!canCreateNewTetromino)
    {
      reset();
    }
  }
  return sucess;
}

void MoveTetrominoLeft()
{
  DrawActiveTetrominoAsBlank();
  MoveTetrominoLeft(Playfield);
  DrawActiveTetromino();
}

void MoveTetrominoRight()
{
  DrawActiveTetrominoAsBlank();
  MoveTetrominoRight(Playfield);
  DrawActiveTetromino();
}

void CopyTetrominoToPlayfield()
{
  const GenericTetromino& activeTetromino = GetActiveTetromino();
  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    const Block& block = activeTetromino.blocks[i];
    Playfield[block.x][block.y] = activeTetromino.type;
  }
}

void DrawNextTetromino(int positionInQueue, Tetromino type)
{
  GenericTetromino tetromino = GenerateTetronimo(type);
  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    Block& block = tetromino.blocks[i];
    block.x += 8;
    block.y += 5+5*positionInQueue;
  }

  DrawTetromino(tetromino);
}

void DrawEmptyHold(Tetromino type)
{
  GenericTetromino tetromino = GenerateTetronimo(type);
  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    Block& block = tetromino.blocks[i];
    block.x += 13;
    block.y += 5;
  }
  tetromino.type = Tetromino::Empty;

  DrawTetromino(tetromino);
}

void DrawHoldStack(Tetromino type)
{
  GenericTetromino tetromino = GenerateTetronimo(type);
  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    Block& block = tetromino.blocks[i];
    block.x += 13;
    block.y += 5;
  }

  DrawTetromino(tetromino);
}

void DrawEmptyTetromino(int positionInQueue, Tetromino type)
{
  GenericTetromino tetromino = GenerateTetronimo(type);
  for (int i = 0; i < TetrominoBlockAmount; i++)
  {
    Block& block = tetromino.blocks[i];
    block.x += 8;
    block.y += 5+5*positionInQueue;
  }
  tetromino.type = Tetromino::Empty;

  DrawTetromino(tetromino);
}