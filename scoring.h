#pragma once

class Scoring
{
  public:
    void ClearedLines(int linesCleared, unsigned long gameSpeed);
    unsigned int GetScore() { return m_Score; }
  private:
    unsigned int m_Score = 0;
};