

#include "stdafx.h"
#include "./../Network/Network.h"
#include <iostream>
#include <conio.h>
#include "Game.h"


enum class KeyCode {
	PreesKey, PreesKey_tick
};

Game* game[4];

void Recv_check(json j)
{
	KeyCode k = j["sendcode"].get<KeyCode>();

	switch (k)
	{

	case KeyCode::PreesKey:
		game[0]->PreesKey(j["KeyNumber"].get<int>());
		break;
	case KeyCode::PreesKey_tick:
		game[0]->PreesKey(j["KeyNumber"].get<int>());
		game[0]->tick -= 1000;
		break;
	default:
		break;
	}
}
void main() {
	system("mode con:cols=156 lines=30");

	json j_buffer;
	Network net;
	char buf[MAX_BUFFER];

	for (int i = 0; i < 4; i++)
	{
		game[i] = new Game(i);
		game[i]->DrawBackground();
	}
	net.SetCheckFunc(Recv_check);
	net.StartClient(PF_INET, 8888, "127.0.0.1");
   
	
    while (true) {
        clock_t now = clock();

        if (_kbhit()) {
            int nInput = _getch();
			//보냄 
			
            if (nInput == 0xE0 || nInput == 0) {
                nInput = _getch();
				//받음
				j_buffer["sendcode"] = KeyCode::PreesKey;
				j_buffer["KeyNumber"] = nInput;
				
				strcpy_s(buf, j_buffer.dump().c_str());
				net.Send(buf, strlen(buf));
				//game->PreesKey(nInput);
			}
			else if (nInput == 32 || nInput == 48) {
				//받음

				j_buffer["sendcode"] = KeyCode::PreesKey_tick;
				j_buffer["KeyNumber"] = nInput;

				strcpy_s(buf, j_buffer.dump().c_str());
				net.Send(buf, strlen(buf));
				//game->PreesKey(nInput);
                //tick -= 1000;
            }
			
        }

		for (int i = 0; i < 4; i++)
		{
			game[i]->DrawBlock();
			if (now - game[i]->tick < game[i]->time)
			{
			}
			else
			{
				game[i]->Update();
				game[i]->Update();
				game[i]->tick = now;
			}
		}
		
    }
}