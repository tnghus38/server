#pragma once

#include "Client.h"
#include <vector>
#include <array>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;
class Room {
  public:
    /// <summary> �� ���̵� </summary>
    int id;
	int count = 0;
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
	inline int PlayerIndexCheck() {
		int n=0;
		for (auto& i : Clients) {
			if (i == nullptr&&n==0)
			{
				return n;
			}
			if (i->ID == n)
				n++;
		}

		return n;
	}
  private:
    /// <summary> Ŭ���̾�Ʈ ī��Ʈ </summary>
  

    /// <summary> Ŭ���̾�Ʈ �迭 </summary>
    array<shared_ptr<Client>, 4> Clients;

    /// <summary> �б� ó���� </summary>
    inline void Recv(SOCKETINFO* socketinfoRecv) {
		
		WSABUF s = socketinfoRecv->dataBuffer;
        Send(s);
		

    }

    /// <summary> ���� ó���� </summary>
    inline void Send(WSABUF& sendbuffer) {
		int n = 0;
        for (auto& i : Clients) {
            if (i != nullptr) {
				char ms[MAX_BUFFER];

				printf("\n %s���䱳 \n", sendbuffer.buf);
				json s = json::parse(sendbuffer.buf);
				s["Pindex"] = n;
				strcpy_s(ms, s.dump().c_str());
				sendbuffer.buf = ms;
				printf("\n %s�Ľ� \n", sendbuffer.buf);
                i->Send(sendbuffer);
            }
			n++;
        }
    }
};