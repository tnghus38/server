

#include "stdafx.h"
#include "./../Network/Network.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include "Game.h"



Game* game[4];
clock_t tick;
int myIndex;
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

				j_buffer["sendcode"] = MsgCode::PreesKey_tick;
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


			if (now - tick < game[i]->time)
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