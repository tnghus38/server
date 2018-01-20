#include "Network.h"
#include <stdio.h>

HANDLE Network::hIOCP;

void Network::StartServer(int _family, int _port, char _ip[]) {
    SetServerAddr(_family, _port, _ip);
    Bind();
    Listen();
    Accept();
}

void Network::StartClient(int _family, int _port, char _ip[]) {
    SetServerAddr(_family, _port, _ip);
    Connect();
	Recvstart();
}

Network::Network() {
    Network::OpenNet();

	funcCheck= std::bind(&Network::msgcheck, this, std::placeholders::_1);
}

void Network::msgcheck(json j)
{
	
	char ms[MAX_BUFFER];

	strcpy_s(ms, j.dump().c_str());
	printf("\n Check - Receive message : %s \n", ms);
}

Network::~Network() {
    Network::CloseNet();
}

void Network::SetServerAddr(int _family, int _port, char _ip[]) {
    if (_ip == nullptr) {
        serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    } else {
        serverAddr.sin_addr.S_un.S_addr = inet_addr(_ip);
    }
    serverAddr.sin_family = _family;
    serverAddr.sin_port = _port;
}

void Network::OpenNet() {
    int addrLen = sizeof(SOCKADDR_IN);

    memset(&serverAddr, 0, addrLen);

    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        throw "Error - Can not load 'winsock.dll' file\n";
    }

    netSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (netSocket == INVALID_SOCKET) {
        throw "Error - Invalid socket\n";
    }
}

void Network::CloseNet() {
    closesocket(netSocket);
    WSACleanup();
}

void Network::Bind() {
    if (::bind(netSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        CloseNet();
        throw "Error - Fail to bind\n";
    }
}

void Network::Listen() {
    if (listen(netSocket, 5) == SOCKET_ERROR) {
        CloseNet();
        throw "Error - Fail to listen\n";
    }
}

void Network::Connect() {
    if (connect(netSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        CloseNet();
        throw "Error - Fail to connect\n";
    } else {
        //printf("Server Connected\n* Enter Message\n->");
    }
}

void Network::Accept() {

    // ��Ŀ������ ����
    // - CPU * 2��
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    static size_t threadCount = systemInfo.dwNumberOfProcessors * 2;
    unsigned long threadId;

    hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, hIOCP, 0, 1);

    //IOCP ����� �Ϸ� ó�� ������
    for (size_t i = 0; i < threadCount; i++) {
        // - thread ����
        CreateThread(NULL, 0, [](void* data) ->DWORD {
            Network* myThis = (Network*)data;

            //���� ���� ������ �޾ƿ���
            DWORD receiveBytes;
            //��Ʈ ���鶧 ���޵Ȱ� (Ŭ���̾�Ʈ �ڵ��� �� �ִ�.)
            DWORD completionKey;
            //��Ʈ ���鶧 ���޵Ȱ� (��Ĺ ������ �����Ͱ� ���ִ�.)
            struct SOCKETINFO* eventSocket;

            while (true) {
                try {
                    // ����� �Ϸ� ���
                    // ����� IOCP�ڵ�, ���ŵ� ����Ʈ����, CreateIoCompletionPort ���� �Է��Ѱ�, WSASend or WSARecv ���� �ѱ� �ּҰ�
                    if (GetQueuedCompletionStatus(hIOCP, &receiveBytes, (PULONG_PTR)&completionKey, (LPOVERLAPPED *)&eventSocket, INFINITE) == 0) {
                        printf("Error - GetQueuedCompletionStatus Failure\n");
						if (myThis->clients[completionKey]->roomID > 0) {
							for (auto&i : myThis->rooms)
							{
								if (i->id == myThis->clients[completionKey]->roomID)
								{
									i->RemoveClient(myThis->clients[completionKey]);
									printf("%d���� ����", i->id);
								}
							}
						}

                        myThis->clients.erase(completionKey);
                        continue;
                    }

                    //���� ���� ������ ����ġ�� ����
                    eventSocket->dataBuffer.len = eventSocket->bufferSize = receiveBytes;

                    printf("Thread Playing %d \n",GetCurrentThreadId() );

                    //�Ϸ�� Ÿ�� Ȯ���ϰ� �б⵿���̸� �� Ŭ���̾�Ʈ�� ����
                    if (eventSocket->type == eIOType::IO_RECV) {
                        myThis->clients[completionKey]->ActionRecv(eventSocket);
                    } else {

                    }
                } catch (const std::exception& e) {
                    printf(e.what());
                }
            }
        }, this, 0, &threadId);
    }

    //���� ���Ӵ�� ������
    CreateThread(NULL, 0, [](void* data) ->DWORD {
        Network* myThis = (Network*)data;

        while (true) {

            //Ŭ���̾�Ʈ ���� ���
            int addrLen = sizeof(SOCKADDR_IN);
            SOCKADDR_IN clientAddr;
            memset(&clientAddr, 0, addrLen);
            SOCKET client = accept( myThis->netSocket, (struct sockaddr *)&clientAddr, &addrLen);

            if (client == INVALID_SOCKET) {
                printf("Error - Accept Failure\n");
                continue;
            }

            //Ŭ���̾�Ʈ�� �����ϸ� Ŭ���̾�Ʈ ��ü�� �������ش�.
            auto tCleint = myThis->clients[client] = std::make_shared<Client>(client);
			
			//�� �ȿ� �ο� üũ
			int roomcount = 0;
			int roomid = 1;
            //Ŭ���̾�Ʈ�� �뿡 ���ӽ�Ų��.
			for (auto&i : myThis->rooms)
			{
				roomcount = i->AddClinet(tCleint);
				
				if ( 5 > roomcount) {
					//���� 
					i->id = roomid;
					tCleint->roomID = i->id;
					printf("\n %d����  �ο�?: %d\n", i->id,i->count);
					
					json j_buffer;
					j_buffer["sendcode"] = MsgCode::Playerindex;
					j_buffer["Pindex"] = i->PlayerIndexCheck();

					strcpy_s(myThis->messageBuffer, j_buffer.dump().c_str());
					printf("%s", myThis->messageBuffer);
					myThis->Send(myThis->messageBuffer, strlen(myThis->messageBuffer));
					printf("%s", myThis->messageBuffer);
				
					break;
				}
				else
				{//���ο� �ʰ��� �����ϴ��� AddClinet ������ ī��Ʈ�� �ö󰡼� �ٽ� ī��Ʈ ����� 
					i->count = 4;

					printf("\n %d����  �ο�!?: %d\n", i->id, i->count);
				}
				roomid++;

				//printf("\n %d����  �ο�: %d\n", i->id, i->count);
			}
			//  �뿡 ���� ���аų�  ó�� �����Ҷ� 
			if (roomcount == 0 || roomcount > 4)
			{
				//�븸�� 
				auto room = std::make_shared<Room>();
				
				//Ŭ�� �뿡 �ְ� 
				room->AddClinet(tCleint);

				room->id = roomid;
				tCleint->roomID = room->id;
				printf("\n %d����  �ο�: %d\n", room->id, room->count);
				json j_buffer;
				j_buffer["sendcode"] = MsgCode::Playerindex;
				j_buffer["Pindex"] = room->PlayerIndexCheck();

				//strcpy_s(myThis->messageBuffer, j_buffer.dump().c_str());
				//printf("%s", myThis->messageBuffer);
				//myThis->Send(myThis->messageBuffer, strlen(myThis->messageBuffer));
				//printf("%s", myThis->messageBuffer);
				//���� ����Ʈ�� �߰� 
				myThis->rooms.push_back(room);

			}

            //Ŭ���̾�Ʈ�� IOCP��Ʈ�� �������ش�.
            myThis->hIOCP = CreateIoCompletionPort((HANDLE)client, myThis->hIOCP, (DWORD)client, 1);
        }
    }, this, 0, NULL);
}

void Network::Recv() {
    CreateThread(NULL, 0, [](void* data) ->DWORD {
        Network* myThis = (Network*)data;

        myThis->sizeBuffer = recv(myThis->netSocket, myThis->messageBuffer, MAX_BUFFER, 0);
        if (myThis->sizeBuffer > 0) {
            printf("NTRACE - Receive message : %s (%d bytes)\n* Enter Message\n->", myThis->messageBuffer, myThis->sizeBuffer);
        }
        return 0;

    }, this, 0, NULL);
}

int Network::Send(char* _msg, const int _len) {
    int sendBytes = send(netSocket, _msg, _len, 0);
    if (sendBytes > 0) {
     //  printf("NTRACE - Send message : %s (%d bytes)\n", _msg, _len);
    }
    return sendBytes;
}

void Network::Recvstart()
{

		CreateThread(NULL, 0, [](void* data) ->DWORD {
			Network* myThis = (Network*)data;
			while (true)
			{
				myThis->sizeBuffer = recv(myThis->netSocket, myThis->messageBuffer, MAX_BUFFER, 0);
				json s=json::parse(myThis->messageBuffer);
				if (myThis->sizeBuffer > 0) {
					myThis->funcCheck(s);
				}
			}
			return 0;

		}, this, 0, NULL);
		
	
}