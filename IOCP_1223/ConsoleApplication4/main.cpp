

#include "stdafx.h"
#include "./../Network/Network.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include "Game.h"



Game* game[4];
clock_t tick;
int myIndex;
int p_stat;
void Recv_check(json j)
{
	MsgCode MC = j["sendcode"].get<MsgCode>();
	if(myIndex==5)
		myIndex = j["Pindex"].get<int>();
	switch (MC)
	{
	case MsgCode::PreesKey:
		if(j["InputPlayer"].get<int>()!=5)
		 game[j["InputPlayer"].get<int>()]->PreesKey(j["KeyNumber"].get<int>());

		break;
	case MsgCode::PreesKey_tick:
		if (j["InputPlayer"].get<int>() != 5)
		{
			game[j["InputPlayer"].get<int>()]->PreesKey(j["KeyNumber"].get<int>());
			game[j["InputPlayer"].get<int>()]->Update();
		}
		break;
	case MsgCode::PreesKey_Ready:
		if (j["InputPlayer"].get<int>() != 5)
		{
			game[j["InputPlayer"].get<int>()]->ReadyGame();
		
		}
		break;
	case MsgCode::PreesKey_Start:
		for (int i = 0; i < 4; i++)
		{
			if(i<j["Roomcount"].get<int>())
				game[i]->StartGame();
			else 
				game[i]->EndGame();
		}
		break;
	default:
		break;
	}
}
void main() {
	myIndex = 5;
	json j_buffer;
	system("mode con:cols=156 lines=30");
	Network net;
	net.SetCheckFunc(Recv_check);
	net.StartClient(PF_INET, 8888, "127.0.0.1");
    tick = clock();
	char buf[MAX_BUFFER];
	for (int i = 0; i < 4; i++)
	{
		game[i] = new Game(i);
		game[i]->DrawBackground();
	}

	//본인 인덱스 번호 받아오기위한코드 
	j_buffer["sendcode"] = MsgCode::PreesKey;
	j_buffer["Pindex"] = myIndex;
	j_buffer["InputPlayer"] = myIndex;
	j_buffer["Roomcount"] = 0;
	strcpy_s(buf, j_buffer.dump().c_str());
	net.Send(buf, strlen(buf));
    while (true) {
        clock_t now = clock();
		
        if (_kbhit()) {
            int nInput = _getch();
			//보냄 
			
            if (nInput == 0xE0 || nInput == 0) {
                nInput = _getch();
				//받음
				j_buffer["sendcode"] = MsgCode::PreesKey;
				j_buffer["KeyNumber"] = nInput;
				j_buffer["Pindex"] = myIndex;
				j_buffer["InputPlayer"] = myIndex;
				strcpy_s(buf, j_buffer.dump().c_str());
				net.Send(buf, strlen(buf));
				//game->PreesKey(nInput);
			}
			else if (nInput == 32 || nInput == 48) {
				//받음
				if (game[myIndex]->GetState() == PlayerState::UnReady)
				{
					j_buffer["sendcode"] = MsgCode::PreesKey_Ready;
				}
				else
				{
					j_buffer["sendcode"] = MsgCode::PreesKey_tick;
				}
				j_buffer["KeyNumber"] = nInput;
				j_buffer["Pindex"] = myIndex;
				j_buffer["InputPlayer"] = myIndex;
				strcpy_s(buf, j_buffer.dump().c_str());
				net.Send(buf, strlen(buf));
				//game->PreesKey(nInput);
                //tick -= 1000;
            }
			
        }
		for (int i = 0; i < 4; i++)
		{

			game[i]->DrawBlock();
			game[i]->DrawBoard();


			if (now - tick < 500)
			{
			}
			else {
				tick = now;
				for (int j = 0; j < 4; j++)
				{
					game[j]->Update();
				}
			}
		}
    }
}