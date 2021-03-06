#pragma once

#include <list>
#include <vector>

#include "Util.h"

using namespace std;

enum class eBlockType {
    Imino,
    Omino,
    Zmino,
    Smino,
    Jmino,
    Lmino,
	Tmino,
};

enum class eBlockState
{
	none, //빈블럭
	draw, //그려진 블록
	beDraw, //그려져 있는 블록
	bebeDraw, //그려져 있는 블록에 값을 추가한 상태

	lock, //락 블럭
	wall, //벽 블럭

	Gnone = 10, //가이드 미노 지우기 위한 상태
	Gdraw, //가이드 미노 그려진 상태
	GbeDraw, //이미 그려진 가이드 미노 상태,
	GbebeDraw, // 그려져 있는 가이드에 값을 추가한 상태

	Die,
	EndDraw

};

class Block {
  public:
    inline Block() {}
    inline Block(int value) : BlockState(value) { }

    /// <summary> 블럭상태 1 2 3내려오는 현재블럭 상태, 10, 11, 12, 13 가이드 블럭 상태 4 락블럭 5 벽</summary>
    int BlockState;

    /// <summary> 블럭색 </summary>
    eColor forgColor = eColor::White;

    /// <summary> 블럭 백컬러 </summary>
    eColor backColor = eColor::Black;

    inline void Copy(Block block) {
        BlockState = block.BlockState;
        forgColor = block.forgColor;
        backColor = block.backColor;
    }

    Block operator = (Block value) {
        return this->BlockState = value.BlockState;
    }

    bool operator > (Block value) {
        return this->BlockState > value.BlockState;
    }

    bool operator < (Block value) {
        return this->BlockState < value.BlockState;
    }

    bool operator == (Block value) {
        return this->BlockState == value.BlockState;
    }

    Block& operator -- () {
        if(BlockState < 4) //블럭 락
            BlockState--;
        return *this;
    }

    Block operator -- (int) {
        --this->BlockState;
        return *this;
    }

    Block operator + (int value) {
        this->BlockState += value;
        return *this;
    }

    Block operator += (int value) {
        this->BlockState += value;
        return *this;
    }
};

class Tetrimino {
  public:
    inline Tetrimino() {
        x = 5;
		y = 0;
    }


    inline virtual ~Tetrimino() {
        delete[] blockBorad;
    }

    inline void Copy(Tetrimino& mino) {
        blockBorad = mino.blockBorad;
        x = mino.x;
        y = mino.y;
        nimoSize = mino.nimoSize;
        DirectionSize = mino.DirectionSize;
        CurrentDirection = mino.CurrentDirection;
        State = mino.State;
        forgColor = mino.forgColor;
        backColor = mino.backColor;
    }

    int* blockBorad;

    /// <summary> 블럭의 위치 x, y </summary>
    int x, y;

	/// <summary> 블럭의 종류 </summary>
	int minoNumber;

    /// <summary> 블럭의 크기 </summary>
    int nimoSize;

    /// <summary> 방향의 갯수 </summary>
    int DirectionSize = 0;

    /// <summary> 현재 방향 </summary>
    int CurrentDirection = 0;

    /// <summary> 1 : 락, 2 : 실패? </summary>
    int State = 0;

    /// <summary> 블럭색 </summary>
    eColor forgColor = eColor::White;

    /// <summary> 블럭 백컬러 </summary>
    eColor backColor = eColor::Black;


    inline void BlockLock() {
        State = 1;
		
    }

    inline void NextDirect() {
        CurrentDirection++;
        if (CurrentDirection >= DirectionSize) {
            CurrentDirection = 0;
        }
    }

    inline int GetPos(int x, int y) {
        return (CurrentDirection * nimoSize* nimoSize) + ((y * nimoSize) + x);
    }
};

class Imino : public Tetrimino {
  public:
    Imino() {
        DirectionSize = 2;
        nimoSize = 4;
        forgColor = eColor::LightBlue;
		minoNumber = (int)eBlockType::Imino;
        blockBorad =  (int*)(new int [2][4][4] {
            {
                { 2, 0, 0, 0 },
                { 2, 0, 0, 0 },
                { 2, 0, 0, 0 },
                { 2, 0, 0, 0 },
            }, {
                { 0, 0, 0, 0 },
                { 0, 0, 0, 0 },
                { 0, 0, 0, 0 },
                { 2, 2, 2, 2 },
            },
        });
    }

    inline virtual ~Imino() {}
};

class Omino : public Tetrimino {
  public:
    Omino() {
        DirectionSize = 1;
        nimoSize = 2;
		minoNumber = (int)eBlockType::Omino;
        forgColor = eColor::Yellow;
        blockBorad = (int*)(new int [1][2][2] { {
                { 2, 2 },
                { 2, 2 },
            },
        });
    }

    inline virtual ~Omino() {}
};

class Zmino : public Tetrimino {
  public:
    Zmino() {
        DirectionSize = 4;
        nimoSize = 3;
		minoNumber = (int)eBlockType::Zmino;
        forgColor = eColor::Red;
        blockBorad = (int*)(new int[4][3][3] { {
                { 0, 2, 0 },
                { 2, 2, 0 },
                { 2, 0, 0 },
            },{
                { 0, 0, 0 },
                { 2, 2, 0 },
                { 0, 2, 2 },
            },{
                { 0, 2, 0 },
                { 2, 2, 0 },
                { 2, 0, 0 },
            },{
                { 0, 0, 0 },
                { 2, 2, 0 },
                { 0, 2, 2 },
            },
        });
    }

    inline virtual ~Zmino() {}
};

class Smino : public Tetrimino {
  public:
    Smino() {
        DirectionSize = 4;
        nimoSize = 3;
		minoNumber = (int)eBlockType::Smino;
        forgColor = eColor::Green;
        blockBorad = (int*)(new int[4][3][3] { {
                { 2, 0, 0 },
                { 2, 2, 0 },
                { 0, 2, 0 },
            },{
                { 0, 0, 0 },
                { 0, 2, 2 },
                { 2, 2, 0 },
            },{
                { 2, 0, 0 },
                { 2, 2, 0 },
                { 0, 2, 0 },
            },{
                { 0, 0, 0 },
                { 0, 2, 2 },
                { 2, 2, 0 },
            }
        });
    }

    inline virtual ~Smino() {}
};

class Jmino : public Tetrimino {
  public:
    Jmino() {
        DirectionSize = 4;
        nimoSize = 3;
		minoNumber = (int)eBlockType::Jmino;
        forgColor = eColor::LightMagenta ;
        blockBorad = (int*)(new int[4][3][3] { {
                { 2, 2, 0 },
                { 2, 0, 0 },
                { 2, 0, 0 },
            },{
                { 0, 0, 0 },
                { 2, 0, 0 },
                { 2, 2, 2 },
            },{
                { 0, 2, 0 },
                { 0, 2, 0 },
                { 2, 2, 0 },
            },{
                { 0, 0, 0 },
                { 2, 2, 2 },
                { 0, 0, 2 },
            }
        });
    }
    inline virtual ~Jmino() {}
};

class Lmino : public Tetrimino {
  public:
    Lmino() {
        DirectionSize = 4;
        nimoSize = 3;
		minoNumber = (int)eBlockType::Lmino;
        forgColor = eColor::LightCyan;
        blockBorad = (int*)(new int[4][3][3] { {
                { 0, 0, 0 },
                { 0, 0, 2 },
                { 2, 2, 2 },
            },{
                { 2, 2, 0 },
                { 0, 2, 0 },
                { 0, 2, 0 },
            },{
                { 0, 0, 0 },
                { 2, 2, 2 },
                { 2, 0, 0 },
            },{
                { 2, 0, 0 },
                { 2, 0, 0 },
                { 2, 2, 0 },
            }
        });
    }
    inline virtual ~Lmino() {}
};

class Tmino : public Tetrimino {
public:
	Tmino() {
		DirectionSize = 4;
		nimoSize = 3;
		minoNumber = (int)eBlockType::Tmino;
		forgColor = eColor::White;
		blockBorad = (int*)(new int[4][3][3]{ {
				{ 0, 2, 0 },
				{ 2, 2, 2 },
				{ 0, 0, 0 },
			},{
				{ 0, 2, 0 },
				{ 0, 2, 2 },
				{ 0, 2, 0 },
			},{
				{ 0, 0, 0 },
				{ 2, 2, 2 },
				{ 0, 2, 0 },
			},{
				{ 0, 2, 0 },
				{ 2, 2, 0 },
				{ 0, 2, 0 },
			},
		});
	}

	inline virtual ~Tmino() {}
};