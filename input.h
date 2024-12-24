#pragma once

struct InputActions {
  bool moveLeft;
  bool moveRight;
  bool rotate;
  bool moveDown;
  bool hold;
};

void InitializeInput();
InputActions GetInput();
