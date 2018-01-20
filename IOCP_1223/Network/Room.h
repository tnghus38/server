#pragma once

#include "Client.h"
#include <vector>
#include <array>

using namespace std;

class Room {
  public:
    /// <summary> 룸 아이디 </summary>
    int id;
	int count = 0;
    /// <summary> 클라이언트 추가  : 5 = 실패</summary>
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

    /// <summary> 클라이언트 삭제 </summary>
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
			if (i == nullptr)
			{
				return n;
			}
			if (i->ID == n)
				n++;
		}

		return n;
	}
  private:
    /// <summary> 클라이언트 카운트 </summary>
  

    /// <summary> 클라이언트 배열 </summary>
    array<shared_ptr<Client>, 4> Clients;

    /// <summary> 읽기 처리용 </summary>
    inline void Recv(SOCKETINFO* socketinfoRecv) {
        Send(socketinfoRecv->dataBuffer);
    }

    /// <summary> 쓰기 처리용 </summary>
    inline void Send(WSABUF& sendbuffer) {
        for (auto& i : Clients) {
            if (i != nullptr) {
                i->Send(sendbuffer);
            }
        }
    }
};