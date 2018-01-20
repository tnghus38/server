#pragma once

#include "Block.h"


class Tetrimino;

class Board {
  public:
    Board();
    int x, y;
    static const int BoardSizeX = 10, BoardSizeY = 20;
	int endline;
    Block BoardBlock[BoardSizeY + 1][BoardSizeX + 2];

    void DrawBoard();
    void DrawBlock(int x, int y, char* str, bool isEmpty = false);

	
    bool CheckLine();
    void DeletionLine(int line);
	void AddLine(int line);
    int CheckMino(Tetrimino& mino);
	
	void GameOver();
	void Endint();
	//void ResetGame();
    void DrawMino(Tetrimino& mino);

    void DrawLock(Tetrimino& mino);
    void DrawGuide(Tetrimino& mino);
};