#pragma once

#include "Client.h"
#include <vector>
#include <array>

using namespace std;

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

  private:
    /// <summary> Ŭ���̾�Ʈ ī��Ʈ </summary>
  

    /// <summary> Ŭ���̾�Ʈ �迭 </summary>
    array<shared_ptr<Client>, 4> Clients;

    /// <summary> �б� ó���� </summary>
    inline void Recv(SOCKETINFO* socketinfoRecv) {
        Send(socketinfoRecv->dataBuffer);
    }

    /// <summary> ���� ó���� </summary>
    inline void Send(WSABUF& sendbuffer) {
        for (auto& i : Clients) {
            if (i != nullptr) {
                i->Send(sendbuffer);
            }
        }
    }
};