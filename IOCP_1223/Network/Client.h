#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <memory>
#include <functional>
#include <atomic>

using namespace std;

//Ŭ���̾�Ʈ�� ��Ŭ������ ������ �������ֱ� ���� ���漱�����ش�.
class Room;

static const int MAX_BUFFER = 1024;

enum class eIOType {
    IO_SEND,
    IO_RECV
};
struct SOCKETINFO {
    /// <summary> IOCP���� ���Ǵ� overlapped��������ü </summary>
    WSAOVERLAPPED overlapped;

    /// <summary> IOCP���� ���Ǵ� ���� ���� ����ü </summary>
    WSABUF dataBuffer;

    /// <summary> ���� ���� </summary>
    char messageBuffer[MAX_BUFFER];

    /// <summary> ���� ����� ��� </summary>
    DWORD bufferSize;

    eIOType type;
    void init(eIOType type) {
        this->type = type;
        this->bufferSize = 0;

        memset(this->messageBuffer, 0x00, MAX_BUFFER);
        ZeroMemory(&this->overlapped, sizeof(this->overlapped));

        this->dataBuffer.len = MAX_BUFFER;
        this->dataBuffer.buf = this->messageBuffer;
    }
};

class Client : public std::enable_shared_from_this<Client> {
  public:
    int ID;
	int roomID = 0;
    string name;
    inline Client(SOCKET socket) {
        this->socket = socket;

        socketinfoRecv.init(eIOType::IO_RECV);
        socketinfoSend.init(eIOType::IO_SEND);

        funcRecv = std::bind(&Client::Recv, this, std::placeholders::_1);
        funcSend = std::bind(&Client::Send, this, std::placeholders::_1);

        PostRecv();
    }

    ~Client() {
        printf("%d ����\n", socket);
        closesocket(socket);
    }

    /// <summary> �б� ���� �ٸ� ��ü�� �ѱ�� </summary>
    inline void SetFunc(std::function< void(SOCKETINFO*)> recv, std::function< void(WSABUF&)> send) {
        funcRecv = recv;
        funcSend = send;
    }

    /// <summary> �б� ���� ó�� �ٽ� �������� </summary>
    inline void ReturnFunc() {
        funcRecv = std::bind(&Client::Recv, this, std::placeholders::_1);
        funcSend = std::bind(&Client::Send, this, std::placeholders::_1);
    }

    /// <summary> �ܺ� ȣ���  Recv </summary>
    inline void ActionRecv(SOCKETINFO* socketinfoRecv) {
        funcRecv(socketinfoRecv);
        PostRecv();
        return;
    }

    /// <summary> �ܺ� ȣ��� Send </summary>
    inline void ActionSend(WSABUF& sendbuffer) {
        funcSend(sendbuffer);
        return;
    }

  protected :
    //������ ����
    friend Room;

    //����� Ŭ���� �̿ܿ��� ����Ҽ� ������ �Ѵ�.

    /// <summary> ���� ��� Send ����</summary>
    inline virtual void Send(WSABUF& sendbuffer) {
        socketinfoSend.dataBuffer = sendbuffer;
        socketinfoSend.bufferSize = sendbuffer.len;
        ZeroMemory(&socketinfoSend.overlapped, sizeof(socketinfoSend.overlapped));
        PostSend();
    }

    /// <summary> ���� ��� Recv ����</summary>
    inline virtual void Recv(SOCKETINFO* socketinfoRecv) {
        if (socketinfoRecv->bufferSize == 0) {
            closesocket(socket);
        } else {
            printf("cllinet.h//TRACE - Receive message : %s (%d bytes)\n", socketinfoRecv->dataBuffer.buf, socketinfoRecv->dataBuffer.len);
			Send(socketinfoRecv->dataBuffer);
        }
    }

  private:

    SOCKETINFO socketinfoRecv;
    SOCKETINFO socketinfoSend;

    DWORD flags = 0;
    SOCKET socket;

    /// <summary> ������ �б���� ��ü �̵��� ���� �Լ� ������ </summary>
    std::function< void(SOCKETINFO*)> funcRecv;

    /// <summary> ������ ������� ��ü �̵��� ���� �Լ� ������ </summary>
    std::function< void(WSABUF&)> funcSend;

  private:

    /// <summary> ���� Recv </summary>
    inline void PostRecv() {
        socketinfoRecv.dataBuffer.len = MAX_BUFFER;
        socketinfoRecv.bufferSize = 0;
        if (WSARecv(socket, &(socketinfoRecv.dataBuffer), 1, &socketinfoRecv.bufferSize, &flags, &socketinfoRecv.overlapped, NULL) == SOCKET_ERROR) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
            }
        }
    }

    /// <summary> ���� Send </summary>
    inline void PostSend() {
        if (WSASend(socket, &(socketinfoSend.dataBuffer), 1, &socketinfoSend.bufferSize, 0, &socketinfoSend.overlapped, NULL) == SOCKET_ERROR) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
            }
        }
    }
};