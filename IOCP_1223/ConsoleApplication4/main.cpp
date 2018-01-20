﻿

#include "stdafx.h"
#include "./../Network/Network.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include "Game.h"


enum class MsgCode {

	PreesKey, PreesKey_tick
};
Game* game;
clock_t tick;
void Recv_check(json j)
{
	MsgCode MC = j["sendcode"].get<MsgCode>();

	switch (MC)
	{
	case MsgCode::PreesKey:
		 game->PreesKey(j["KeyNumber"].get<int>());
		break;
	case MsgCode::PreesKey_tick:

		game->PreesKey(j["KeyNumber"].get<int>());
		tick -= 1000;
		break;
	default:
		break;
	}
}
void main() {
	json j_buffer;
	system("mode con:cols=156 lines=30");
	Network net;
	net.SetCheckFunc(Recv_check);
	net.StartClient(PF_INET, 8888, "127.0.0.1");
    tick = clock();
	char buf[MAX_BUFFER];
	game = new Game(0);
	Game* game1= new Game(1);
	Game* game2 = new Game(2);
	Game* game3 = new Game(3);
    game->DrawBackground();
	game1->DrawBackground();
	game2->DrawBackground();
	game3->DrawBackground();
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
				
				strcpy_s(buf, j_buffer.dump().c_str());
				net.Send(buf, strlen(buf));
				//game->PreesKey(nInput);
			}
			else if (nInput == 32 || nInput == 48) {
				//받음

				j_buffer["sendcode"] = MsgCode::PreesKey_tick;
				j_buffer["KeyNumber"] = nInput;

				strcpy_s(buf, j_buffer.dump().c_str());
				net.Send(buf, strlen(buf));
				//game->PreesKey(nInput);
                //tick -= 1000;
            }
			
        }
		game->DrawBlock();
		game->DrawBoard();
		game1->DrawBlock();
		game1->DrawBoard();
		if (now - tick < game->time)
            continue;
		game->Update();
		game1->Update();
        tick = now;
    }
}