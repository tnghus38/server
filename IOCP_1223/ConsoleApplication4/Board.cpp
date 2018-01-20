#include "stdafx.h"
#include "Board.h"

#include <iostream>

#include "Util.h"
#include "Block.h"

Board::Board() {
    y = 1;
	x = 40;
    for (int y = 0; y < BoardSizeY + 1; ++y) {
        for (int x = 0; x < BoardSizeX + 2; ++x) {
            BoardBlock[y][x].backColor = eColor::Black;
            BoardBlock[y][x].forgColor = eColor::Black;
            if(y == BoardSizeY || x == 0 || x == BoardSizeX + 1)
                BoardBlock[y][x] = (int)eBlockState::wall;
            else
                BoardBlock[y][x] = (int)eBlockState::none;
        }
    }
}/*
void Board::ResetGame()
{
	y = 1;
	x = 40;
	for (int y = 0; y < BoardSizeY + 1; ++y) {
		for (int x = 0; x < BoardSizeX + 2; ++x) {
			BoardBlock[y][x].backColor = eColor::Black;
			BoardBlock[y][x].forgColor = eColor::Black;
			if (y == BoardSizeY || x == 0 || x == BoardSizeX + 1)
				BoardBlock[y][x] = (int)eBlockState::wall;
			else
				BoardBlock[y][x] = (int)eBlockState::none;
			if (BoardBlock[y][x] == (int)eBlockState::none) {
				DrawBlock(x, y, "  ", true);
			}
		}
	}
}*/
void Board::Endint()
{
	endline = BoardSizeY;
	for (int y = 1; y < 6; ++y) {
		BoardBlock[y][3] = (int)eBlockState::EndDraw;
	}
	for (int x = 3; x < 9; ++x) {
		BoardBlock[1][x] = (int)eBlockState::EndDraw;
		BoardBlock[3][x] = (int)eBlockState::EndDraw;
		BoardBlock[5][x] = (int)eBlockState::EndDraw;
	}
	for (int y = 7; y < 12; ++y) {
		BoardBlock[y][3] = (int)eBlockState::EndDraw;
		BoardBlock[y][8] = (int)eBlockState::EndDraw;
	}
	BoardBlock[8][4] = (int)eBlockState::EndDraw;
	BoardBlock[9][5] = (int)eBlockState::EndDraw;
	BoardBlock[10][6] = (int)eBlockState::EndDraw;
	BoardBlock[11][7] = (int)eBlockState::EndDraw;
	for (int y = 13; y < 19; ++y) {
		BoardBlock[y][3] = (int)eBlockState::EndDraw;
	}
	for (int y = 14; y < 18; ++y) {
		BoardBlock[y][8] = (int)eBlockState::EndDraw;
	}
	for (int x = 4; x < 8; ++x) {
		BoardBlock[13][x] = (int)eBlockState::EndDraw;
		BoardBlock[18][x] = (int)eBlockState::EndDraw;
	}
	
}
void Board::DrawBoard() {
    for (int y = 0; y < BoardSizeY + 1; ++y) {
        for (int x = 0; x < BoardSizeX + 1; ++x) {

            //가이드 블럭 그리기
            if (BoardBlock[y][x] == (int)eBlockState::Gdraw) {
                DrawBlock(x, y, "□");
                BoardBlock[y][x]--;
                continue;
            }
            //가이드 블럭 삭제
            else if(BoardBlock[y][x] == (int)eBlockState::Gnone) {
                DrawBlock(x, y, "  ", true);
                BoardBlock[y][x] = (int)eBlockState::none;
                continue;
            }
			
		
            //일반 블럭 특수 블럭 확인
			if (BoardBlock[y][x] > (int)eBlockState::none && BoardBlock[y][x] < (int)eBlockState::lock)
			{
				BoardBlock[y][x]--;
			}
			else {
                //특수 블럭 아무것도 안함.
                continue;
            }

            //원래 그려져있던 블럭 안그리고 1로만듬.
            if (BoardBlock[y][x] == (int)eBlockState::beDraw) {
                BoardBlock[y][x]--;
            } else {
                //일반 블럭 그리기
                if (BoardBlock[y][x] == (int)eBlockState::draw) {
                    DrawBlock(x, y, "■");
                } else {
                    //빈자리 지우기
                    DrawBlock(x, y, "  ", true);
                }

            }
			
        }
    }
}

void Board::DrawBlock(int x, int y, char* str, bool isEmpty) {

    if(isEmpty)
        ClearConsoleToColors((int)eColor::Black, (int)eColor::Black);
    else
        ClearConsoleToColors((int)BoardBlock[y][x].forgColor, (int)BoardBlock[y][x].backColor);
    gotoxy(this->x + (x * 2), this->y + y);

    std::cout << str;
}

bool Board::CheckLine() {
	//지워지는여부확인 
	bool flag = false;
    for (int y = BoardSizeY; y > 0; --y) {
        int Count = 0;
        for (int x = 1; x < BoardSizeX + 1; ++x) {
            if (BoardBlock[y][x] == (int)eBlockState::lock)
                Count++;
            else
                break;
        }

        if (Count == BoardSizeX) {
            DeletionLine(y);
            y--;
			flag = true;
        }
    }
	return flag;
}


void Board::AddLine(int line)
{
	int lineX=0;//올라올때 빈여백 
	for (int y = 0; y < BoardSizeY- line; ++y) {
		int Count = 0;
		for (int x = 1; x < BoardSizeX + 1; ++x) {
			BoardBlock[y][x].Copy(BoardBlock[y + line][x]);
			if (BoardBlock[y][x] >(int)eBlockState::none) {
				DrawBlock(x, y, "■");
				Count++;
			}
			else {
				DrawBlock(x, y, "  ", true);
			}
		}
		if (Count == 0)
		{
			lineX++;
		}
	
	}
	for (int y = BoardSizeY - line; y <= BoardSizeY - 1; ++y) {
	
		lineX += 3;
		lineX %= 10;
		
		for (int x = 1; x < BoardSizeX+1; ++x) {
			if (x == lineX)
			{
				BoardBlock[y][x] = (int)eBlockState::none;
			}
			else
			{
				BoardBlock[y][x] = (int)eBlockState::lock;
				BoardBlock[y][x].forgColor = eColor::DarkGray;
				BoardBlock[y][x].backColor = eColor::Black;
			}
			if (BoardBlock[y][x] > (int)eBlockState::none) {
				DrawBlock(x, y, "■");

			}
			else {
				DrawBlock(x, y, "  ", true);
			}
			
		}

	}
}
void Board::DeletionLine(int line) {
    for (int y = line; y > 0; --y) {
        int Count = 0;
        for (int x = 1; x < BoardSizeX + 1; ++x) {
            BoardBlock[y][x].Copy(BoardBlock[y - 1][x]);
            if (BoardBlock[y][x] >(int)eBlockState::none) {
                DrawBlock(x, y, "■");
                Count++;
            } else {
                DrawBlock(x, y, "  ", true);
            }
        }
        if (Count == 0)
            break;
    }
}

int Board::CheckMino(Tetrimino & mino) {
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {

			int tempPos = mino.GetPos(x, y);
			
            if (mino.blockBorad[tempPos] > 0) {

				//블럭이 일반 상태인지 확인
				if (BoardBlock[mino.y + y][mino.x + x] >(int)eBlockState::bebeDraw && BoardBlock[mino.y + y][mino.x + x] < (int)eBlockState::Gnone)
				{
					if (mino.y == 1)
					{
					
						return (int)eBlockState::Die;
						
					}
					return BoardBlock[mino.y + y][mino.x + x].BlockState;
				}
            }

        }
    }
    return 0;
}

void Board::DrawMino(Tetrimino& mino) {
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {
            int tempPos = mino.GetPos(x, y);
            if (mino.blockBorad[tempPos] != 0) {
                //가이드 블럭이 있으면 제거
                if (BoardBlock[mino.y + y][mino.x + x] > (int)eBlockState::Gnone -1)
                    BoardBlock[mino.y + y][mino.x + x] = (int)eBlockState::none;

                BoardBlock[mino.y + y][mino.x + x].forgColor = mino.forgColor;
                BoardBlock[mino.y + y][mino.x + x].backColor = mino.backColor;
                BoardBlock[mino.y + y][mino.x + x] += mino.blockBorad[tempPos];
            }
        }
    }
}


void Board::GameOver()
{
	for (int y = BoardSizeY-1; y >= endline; --y) {
	
		for (int x = 1; x < BoardSizeX+1; ++x) {
			if (BoardBlock[y][x] == (int)eBlockState::EndDraw)
			{
				BoardBlock[y][x].forgColor = eColor::LightRed;
				BoardBlock[y][x].backColor = eColor::Yellow;
			}
			else
			{
				BoardBlock[y][x].forgColor = eColor::DarkGray;
				BoardBlock[y][x].backColor = eColor::Green;
			}
			DrawBlock(x, y, "■");
				
		}
	
	}
	if (endline > 0 )
	{
		endline--;
	}
}
void Board::DrawLock(Tetrimino & mino) {
	//내려놓으면 색 변함 
	mino.forgColor = eColor::DarkGray;
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {
            int tempPos = mino.GetPos(x, y);
            if (mino.blockBorad[tempPos] != 0) {
				BoardBlock[mino.y + y][mino.x + x].forgColor = mino.forgColor;
				BoardBlock[mino.y + y][mino.x + x].backColor = mino.backColor;
                BoardBlock[mino.y + y][mino.x + x] = (int)eBlockState::lock;
				if (BoardBlock[mino.y + y][mino.x + x] > 0) {
					DrawBlock(mino.x + x, mino.y + y, "■");

				}
				else {
					DrawBlock(mino.x + x, mino.y + y, "  ", true);
				}
            }
        }
    }
}

void Board::DrawGuide(Tetrimino & mino) {
    for (int y = 0; y < mino.nimoSize; y++) {
        for (int x = 0; x < mino.nimoSize; x++) {
            int tempPos = mino.GetPos(x, y);
            if (mino.blockBorad[tempPos] != 0) {
                BoardBlock[mino.y + y][mino.x + x].forgColor = mino.forgColor;
                BoardBlock[mino.y + y][mino.x + x].backColor = mino.backColor;
                BoardBlock[mino.y + y][mino.x + x] = 11;
            }
        }
    }
}
