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
	//클라 상태
	PlayerState clientState;
	//점수 
	int GameScore;
	//콤보 
	int Combo;
	
	//등수
	int Rank;

	//클라 위치 
	int boardpos;
	// 떨어질 위치 보여주는 블록  
    Tetrimino  GuideMino;
	//다음 명령 실행 가능한지 먼저 던저 보는 블록 
    Tetrimino  FutureMino;
	// 현재 블록 
    Tetrimino* CurrentMino = nullptr;
	//다음 블록 
    Tetrimino* NextMino = nullptr;

	//랜덤 숫자 
    std::mt19937_64 ranNum;

  private:
	  //처음 블록생성 
    void Setup();
	 //처음 백보드 생성 
    void InitbackGround();

	//상태값 초기화
	void init(int n);
	//다음 블록 생성 
    void CreateNextMino(int n=-1);

	//다음 블록 그리기
	void DrawNext();

	//가이드 블록 생성 
    int CreateGuideMino();

	//랜덤값 가져옴 
    int GetRandom();
  public:
	int time;
	int tick;
    Game();
	Game(int n);
    ~Game();


	//현재 블록이 없으면다음 블록을 넣어주고 다음 블록 생성 
    void CurrentBlockCheck();

	//실시간으로 떨어지게함 
    void Update();
	
	//줄추가
	void LineAdd(int n);

	//키누를때 실행 함수 
    void PreesKey(int key);

	//그림
    void DrawBackground();
    void DrawBoard(int checkstate = 0);
    void DrawBlock();
};

