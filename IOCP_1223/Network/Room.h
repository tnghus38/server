#pragma once

#include "Client.h"
#include <vector>
#include <array>
#include "json.hpp"
enum class MsgCode {

	PreesKey, PreesKey_tick, PreesKey_Ready, PreesKey_Start
};
using namespace std;

using json = nlohmann::json;
class Room {
  public:
    /// <summary> �� ���̵� </summary>
    int id;
	int count = 0;
	int ready_count = 0;
    /// <summary> Ŭ���̾�Ʈ �߰�  : 5 = ����</summary>
    inline int AddClinet(shared_ptr<Client> client) {
        for (auto& i : Clients) {
            if (i == nullptr) {
                i = move(client);
                i->SetFunc(std::bind(&Room::Recv, this, std::placeholders::_1), std::bind(&Room::Send, this, std::placeholders::_1));
                break;
            }
        }

        return ++count;
    }

    /// <summary> Ŭ���̾�Ʈ ���� </summary>
    inline int RemoveClient(shared_ptr<Client> client) {
        for (auto& i : Clients) {
            if (i == client) {
                i->ReturnFunc();
                i = nullptr;
                break;
            }
        }

        return --count;
    }
	
  private:
    /// <summary> Ŭ���̾�Ʈ ī��Ʈ </summary>
  

    /// <summary> Ŭ���̾�Ʈ �迭 </summary>
    array<shared_ptr<Client>, 4> Clients;

    /// <summary> �б� ó���� </summary>
    inline void Recv(SOCKETINFO* socketinfoRecv) {
		
		WSABUF _buf = socketinfoRecv->dataBuffer;
		
	
        Send(_buf);
		

    }

    /// <summary> ���� ó���� </summary>
    inline void Send(WSABUF& sendbuffer) {
		int n = 0;
		char ms[MAX_BUFFER];
		json s = json::parse(sendbuffer.buf);
		if (s["sendcode"] == MsgCode::PreesKey_Ready)
		{
			ready_count++;
			if (count == ready_count)
			{
				s["sendcode"] = MsgCode::PreesKey_Start;
				s["Roomcount"] = count;
				printf("dd%d\n", count);
			}
		}
		
        for (auto& i : Clients) {
            if (i != nullptr) {
				
				
				s["Pindex"] = n++;
				

				strcpy_s(ms, s.dump().c_str());
				sendbuffer.buf = ms;
				i->Send(sendbuffer);
            }
        }
    }
};