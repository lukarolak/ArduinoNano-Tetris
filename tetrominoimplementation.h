#pragma once
    
#include "tetromino.h"
#include "consts.h"

constexpr int TetrominoStackLength = 4;

typedef struct {
    int x,y;
} Block;

constexpr int TetrominoBlockAmount = 4;

typedef struct {
  Block blocks[TetrominoBlockAmount];
  Tetromino type;
} GenericTetromino;

Tetromino* GetTetrominoStack();
void InitializeTetronimoSystem();
GenericTetromino GenerateTetronimo(Tetromino type);
bool GenerateTetromino(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight]);
bool MoveTetrominoDown(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight]);
void MoveTetrominoLeft(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight]);
void MoveTetrominoRight(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight]);
void RotateTetromino(const Tetromino (&playfield)[PlayfieldWidth][PlayfieldHeight], const int loopCounter = 0);
GenericTetromino GetActiveTetromino();
Tetromino GetHoldTetromino();
void HoldTetromino();
