#pragma once

#include "Client.h"
#include "Room.h"
#include <map>
#include <list>
#include<iostream>
#include "json.hpp"
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

using json = nlohmann::json;
//PostQueuedCompletionStatus

enum class MsgCode {

	PreesKey, PreesKey_tick,Playerindex
};

class Network {
  private:

  public:
    Network();
    ~Network();

    void StartServer(int _family, int _port, char _ip[]);
    void StartClient(int _family, int _port, char _ip[]);

    /// <summary> �б� </summary>
    void Recv();
    /// <summary> ���� </summary>
    int Send(char* _msg, const int _len);
	inline void SetCheckFunc( std::function< void(json)> chek) {
		funcCheck = chek;
	}

  private:
    WSADATA WSAData;
    SOCKET netSocket;

    SOCKADDR_IN serverAddr;

    /// <summary> �밴ü </summary>
    list<shared_ptr<Room>> rooms;

    /// <summary> ������ Ŭ���̾�Ʈ map </summary>
    map<SOCKET, shared_ptr<Client>> clients;

    /// <summary> �������� ���� IOCP �ڵ鷯 </summary>
    static HANDLE hIOCP;

    /// <summary> ��Ʈ��ũ���� ���Ǵ� �������� </summary>
    char messageBuffer[MAX_BUFFER];
    /// <summary> ���������� ���� ������ </summary>
    int sizeBuffer = 0;

	//�ޱ⸸ �ϴ³� �ȿ� ���� �Լ� 
	std::function< void(json s)> funcCheck;
	//�ޱ�⺻ 
	void msgcheck(json s);
	//�ޱ⸸����
	void Recvstart();
	
    void SetServerAddr(int _family, int _port, char _ip[]);

    /// <summary> ���� ���� </summary>
    void OpenNet();
    /// <summary> ���� �ݱ� </summary>
    void CloseNet();

    /// <summary> ���� </summary>
    void Listen();
    /// <summary> ���ε� </summary>
    void Bind();
    /// <summary> ����Ʈ </summary>
    void Accept();

    /// <summary> Ŀ��Ʈ </summary>
    void Connect();
};


