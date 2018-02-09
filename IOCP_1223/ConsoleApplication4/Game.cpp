#include "stdafx.h"



#include "Game.h"

void Game::Setup() {
    SetCursor(false);
    CreateNextMino();
    CurrentBlockCheck();

}

void Game::InitbackGround() {
    string out[] = {
        "■■■■■■■■■■■■■■■■■■",
        "■                    ■   NEXT   ■",
        "■                    ■          ■",
        "■                    ■          ■",
        "■                    ■          ■",
        "■                    ■          ■",
        "■                    ■          ■",
        "■                    ■■■■■■■",
		"■                    ■   점수   ■",
		"■                    ■          ■",
		"■                    ■   콤보   ■",
		"■                    ■          ■",
		"■                    ■■■■■■■",
		"■                    ■          ■",
		"■                    ■          ■",
		"■                    ■          ■",
		"■                    ■■■■■■■",
		"■                    ■",
		"■                    ■",
        "■                    ■",
        "■                    ■",
        "■■■■■■■■■■■■",
    };
    backGround.SetString(out, sizeof(out) / sizeof(out[0]));
    backGround.SetColor(eColor::Yellow, eColor::Black);
}
void Game::init(int n)
{
	Rank = 1;
	boardpos = n;
	board.x = (boardpos * 40);
	GameScore = 0;
	Combo = 0;
	time = 500;
	clientState = PlayerState::UnReady;

}
Game::Game() {
	init(0);
    Setup();

    InitbackGround();
}
Game::Game(int n) {

	init(n);
	Setup();
	InitbackGround();
}

Game::~Game() {
}

PlayerState Game::GetState()
{
	return clientState;
}

void Game::CreateNextMino(int n) {
	int Minonum=n;
	if (n == -1)
		Minonum = GetRandom();//받음 
	switch (Minonum) {
    case 0:
        NextMino = std::move(new Imino());
        break;
    case 1:
        NextMino = std::move(new Omino());
        break;
    case 2:
        NextMino = std::move(new Zmino());
        break;
    case 3:
        NextMino = std::move(new Smino());
        break;
    case 4:
        NextMino = std::move(new Jmino());
        break;
    case 5:
        NextMino = std::move(new Lmino());
        break;
	case 6:
		NextMino = std::move(new Tmino());
		break;
    }
	DrawNext();
}
void Game::DrawNext()
{
	string outbackmino[] = {
		"■■■■■",
		"■■■■■",
		"■■■■■",
		"■■■■■",

	};
	string outImino[] = {
		"    ■    ",
		"    ■    ",
		"    ■    ",
		"    ■    ",

	};
	string outJmino[] = {
		"          ",
		"   ■■   ",
		"   ■     ",
		"   ■     ",
		
	};
	string outLmino[] = {
		"          ",
		"      ■  ",
		"  ■■■  ",
		"          ",
	};
	string outOmino[] = {
		
		"          ",
		"   ■■   ",
		"   ■■   ",
		"          ",
	};
	string outZmino[] = {
		"          ",
		"     ■   ",
		"   ■■   ",
		"   ■     ",
		
	};
	string outSmino[] = {
		"          ",
		"   ■     ",
		"   ■■   ",
		"     ■   ",
	};
	string outTmino[] = {
		"          ",
		"    ■    ",
		"  ■■■  ",
		"          ",
	};
	Draw nextHint;
	Draw Hintback;
	Hintback.SetString(outbackmino, sizeof(outbackmino) / sizeof(outbackmino[0]));
	Hintback.SetColor(eColor::Black, eColor::Black);
	Hintback.SetPos((boardpos * 40) + 24, 2);
	Hintback.DrawingStr();
	switch (NextMino->minoNumber)
	{
	case (int)eBlockType::Imino:
		
		nextHint.SetString(outImino, sizeof(outImino) / sizeof(outImino[0]));
		nextHint.SetColor(NextMino->forgColor, NextMino->backColor);
		nextHint.SetPos((boardpos * 40) + 24, 2);
		nextHint.DrawingStr();
		break;

	case (int)eBlockType::Jmino:
		nextHint.SetString(outJmino, sizeof(outJmino) / sizeof(outJmino[0]));
		nextHint.SetColor(NextMino->forgColor, NextMino->backColor);
		nextHint.SetPos((boardpos * 40) + 24, 2);
		nextHint.DrawingStr();
		break;

	case (int)eBlockType::Lmino:
		nextHint.SetString(outLmino, sizeof(outLmino) / sizeof(outLmino[0]));
		nextHint.SetColor(NextMino->forgColor, NextMino->backColor);
		nextHint.SetPos((boardpos * 40) + 24, 2);
		nextHint.DrawingStr();
		break;

	case (int)eBlockType::Omino:
		nextHint.SetString(outOmino, sizeof(outOmino) / sizeof(outOmino[0]));
		nextHint.SetColor(NextMino->forgColor, NextMino->backColor);
		nextHint.SetPos((boardpos * 40) + 24, 2);
		nextHint.DrawingStr();
		break;
	case (int)eBlockType::Smino:
		nextHint.SetString(outSmino, sizeof(outSmino) / sizeof(outSmino[0]));
		nextHint.SetColor(NextMino->forgColor, NextMino->backColor);
		nextHint.SetPos((boardpos * 40) + 24, 2);
		nextHint.DrawingStr();
		break;

	case (int)eBlockType::Zmino:
		nextHint.SetString(outZmino, sizeof(outZmino) / sizeof(outZmino[0]));
		nextHint.SetColor(NextMino->forgColor, NextMino->backColor);
		nextHint.SetPos((boardpos * 40) + 24, 2);
		nextHint.DrawingStr();
		break;
	case (int)eBlockType::Tmino:
		nextHint.SetString(outTmino, sizeof(outTmino) / sizeof(outTmino[0]));
		nextHint.SetColor(NextMino->forgColor, NextMino->backColor);
		nextHint.SetPos((boardpos * 40) + 24, 2);
		nextHint.DrawingStr();
		break;
	default:
		break;
	}

}
void Game::LineAdd(int n)
{
	if(n>=3)
		board.AddLine(n-2);
}
int Game::CreateGuideMino() {
    GuideMino.Copy(*CurrentMino);
    int count = 1;
    while (0 == board.CheckMino(GuideMino)) {
        GuideMino.y++;
        count++;
    }

    GuideMino.y -= 1;
    count -= 1;
    return count;
}

int Game::GetRandom() {
    random_device rn;
    ranNum.seed(rn());
    uniform_int_distribution<int> range(0, 6);
    return range(ranNum);//보냄
}

void Game::CurrentBlockCheck() {
    if (CurrentMino == nullptr) {
        CurrentMino = NextMino;
        CreateNextMino();

    }
}

void Game::EndGame()
{
	time = 100;
	board.Endint();
	clientState = PlayerState::End;
}

void Game::StartGame()
{
		clientState = PlayerState::Play;
}

void Game::ReadyGame()
{

		clientState = PlayerState::Ready;
}

void Game::Update() {
    CurrentBlockCheck();

    FutureMino.Copy(*CurrentMino);
    FutureMino.y++;

	//클라이언트 업데이트에 따라변경 
	switch (clientState)
	{
	case PlayerState::Play:
		if (board.CheckMino(FutureMino) == (int)eBlockState::Die)
		{
			clientState = PlayerState::End;
			time = 100;
			board.Endint();
		
		}
		else if (board.CheckMino(FutureMino)) {
			CurrentMino->BlockLock();
		}
		else {
			CurrentMino->Copy(FutureMino);
		}
		break;
	case PlayerState::End:
		board.GameOver();

		break;
	}
}


void Game::DrawBackground() {
	
	backGround.SetPos(boardpos*(40), 0);
    backGround.DrawingStr();

}

void Game::DrawBoard(int checkstate) {
	string score[] = { "" };
	char s[20];
	string cb[] = { "" };
    board.DrawBoard();
	if (board.CheckLine())
	{
		
		time -= 10;
		if (checkstate == 1)
		{
			Combo += 1;
		
			//보냄  (조건부로 보냄)
			//LineAdd(3);
		}
		GameScore += 100 * Combo;
	 }
	else
	{
		if (checkstate == 1)
		{
			Combo = 0;
		}
	}

	sprintf_s(s, "%d점", GameScore);
	*score = s;
	Draw scoreback;
	scoreback.SetString(score, sizeof(score) / sizeof(score[0]));
	scoreback.SetColor(eColor::LightCyan, eColor::Black);
	scoreback.SetPos((boardpos * 40) + 26, 9);
	 
	scoreback.DrawingStr();

	sprintf_s(s, "%d콤보!", Combo);
	*cb = s;

	Draw comboback;
	comboback.SetString(cb, sizeof(cb) / sizeof(cb[0]));
	comboback.SetColor(eColor::LightRed, eColor::Black);
	comboback.SetPos((boardpos * 40) + 26, 11);
	comboback.DrawingStr();
	string msg[] = { "TAB> 준비" };
	Draw msgback;
	switch (clientState)
	{
	case PlayerState::UnReady:
		msgback.SetColor(eColor::LightGreen, eColor::Black);
		sprintf_s(s, "TAB> 준비");
		break;
	case PlayerState::Ready:
		msgback.SetColor(eColor::Yellow, eColor::Black);
		sprintf_s(s, "준비 완료");
		*msg = s;
		break;
	case PlayerState::Play:
		msgback.SetColor(eColor::LightMagenta, eColor::Black);
		sprintf_s(s, "현재 %d등", Rank);
		*msg = s;
		break;
	case PlayerState::End:
		msgback.SetColor(eColor::LightRed, eColor::Black);
		sprintf_s(s, "게임 오버");
		*msg = s;
		break;
	default:
		break;
	}


	msgback.SetString(msg, sizeof(msg) / sizeof(msg[0]));

	msgback.SetPos((boardpos * 40) + 25, 14);
	msgback.DrawingStr();
}

void Game::DrawBlock() {
    if (CurrentMino == nullptr || clientState!= PlayerState::Play)
        return;

	//가이드 블록이 현재블록과 떨어져 있는지 확인하고 그림 
    if (CreateGuideMino() > 0) {
       board.DrawGuide(GuideMino);
    }

	if (CurrentMino->State == 1)
	{
		board.DrawLock(*CurrentMino);
		DrawBoard(1);
	}
	else
        board.DrawMino(*CurrentMino);

    if (CurrentMino->State) {
        delete CurrentMino;
        CurrentMino = nullptr;
    }
}

void Game::PreesKey(int key) {

    if (CurrentMino == nullptr)
        return;

    if (key == (int)eKey::LEFT) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.x -= 1;
    } else if (key == (int)eKey::RIGHT) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.x += 1;
    } else if (key == (int)eKey::UP) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.NextDirect();
    } else if (key == (int)eKey::DOWN) {
        FutureMino.Copy(*CurrentMino);
        FutureMino.y += 1;
    } else if (key == (int)eKey::SpaceBar) {
        CurrentMino->Copy(GuideMino);
        return;
	}else if (key == (int)eKey::TAP)
	{
	
		if (clientState == PlayerState::Play)
		{

			NextMino->x = CurrentMino->x;
			NextMino->y = CurrentMino->y;
			if (board.CheckMino(*NextMino)) {
				NextMino->x = 5;
				NextMino->y = 0;
			}
			else {
				int minonum = CurrentMino->minoNumber;
				CurrentMino = NextMino;
				CreateNextMino(minonum);
			}

		}
		else if (clientState == PlayerState::UnReady)
			clientState = PlayerState::Play;
		
		
		return;
	}
	else{

        return;
    }

    if ( 0 == board.CheckMino(FutureMino)) {
        CurrentMino->Copy(FutureMino);
    }
}
