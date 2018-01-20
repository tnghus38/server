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
	none, //���
	draw, //�׷��� ���
	beDraw, //�׷��� �ִ� ���
	bebeDraw, //�׷��� �ִ� ��Ͽ� ���� �߰��� ����

	lock, //�� ��
	wall, //�� ��

	Gnone = 10, //���̵� �̳� ����� ���� ����
	Gdraw, //���̵� �̳� �׷��� ����
	GbeDraw, //�̹� �׷��� ���̵� �̳� ����,
	GbebeDraw, // �׷��� �ִ� ���̵忡 ���� �߰��� ����

	Die,
	EndDraw

};

class Block {
  public:
    inline Block() {}
    inline Block(int value) : BlockState(value) { }

    /// <summary> ������ 1 2 3�������� ����� ����, 10, 11, 12, 13 ���̵� �� ���� 4 ���� 5 ��</summary>
    int BlockState;

    /// <summary> ���� </summary>
    eColor forgColor = eColor::White;

    /// <summary> �� ���÷� </summary>
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
        if(BlockState < 4) //�� ��
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

    /// <summary> ���� ��ġ x, y </summary>
    int x, y;

	/// <summary> ���� ���� </summary>
	int minoNumber;

    /// <summary> ���� ũ�� </summary>
    int nimoSize;

    /// <summary> ������ ���� </summary>
    int DirectionSize = 0;

    /// <summary> ���� ���� </summary>
    int CurrentDirection = 0;

    /// <summary> 1 : ��, 2 : ����? </summary>
    int State = 0;

    /// <summary> ���� </summary>
    eColor forgColor = eColor::White;

    /// <summary> �� ���÷� </summary>
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