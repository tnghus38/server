#pragma once

#include <random>
#include <chrono>
#include <ctime>
#include "Board.h"
#include "Block.h"
#include "DrawString.h"
#include "Util.h"
enum class eKey {
	SpaceBar = 32,
	LEFT = 75,
	RIGHT = 77,
	UP = 72,
	DOWN = 80,
	TAP = 48,
};
enum class PlayerState {
	UnReady,
	Ready,
	Play,
	End,
};
class Game {
  private:
    Draw backGround;
    Board board;
	//Ŭ�� ����
	PlayerState clientState;
	//���� 
	int GameScore;
	//�޺� 
	int Combo;
	
	//���
	int Rank;

	//Ŭ�� ��ġ 
	int boardpos;
	// ������ ��ġ �����ִ� ���  
    Tetrimino  GuideMino;
	//���� ��� ���� �������� ���� ���� ���� ��� 
    Tetrimino  FutureMino;
	// ���� ��� 
    Tetrimino* CurrentMino = nullptr;
	//���� ��� 
    Tetrimino* NextMino = nullptr;

	//���� ���� 
    std::mt19937_64 ranNum;

  private:
	  //ó�� ��ϻ��� 
    void Setup();
	 //ó�� �麸�� ���� 
    void InitbackGround();

	//���°� �ʱ�ȭ
	void init(int n);
	//���� ��� ���� 
    void CreateNextMino(int n=-1);

	//���� ��� �׸���
	void DrawNext();

	//���̵� ��� ���� 
    int CreateGuideMino();

	//������ ������ 
    int GetRandom();
  public:
	int time;
	int tick;
    Game();
	Game(int n);
    ~Game();


	//���� ����� ��������� ����� �־��ְ� ���� ��� ���� 
    void CurrentBlockCheck();

	//�ǽð����� ���������� 
    void Update();
	
	//���߰�
	void LineAdd(int n);

	//Ű������ ���� �Լ� 
    void PreesKey(int key);

	//�׸�
    void DrawBackground();
    void DrawBoard(int checkstate = 0);
    void DrawBlock();
};

