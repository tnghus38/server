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

    // 워커스레드 생성
    // - CPU * 2개
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    static size_t threadCount = systemInfo.dwNumberOfProcessors * 2;
    unsigned long threadId;

    hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, hIOCP, 0, 1);

    //IOCP 입출력 완료 처리 쓰레드
    for (size_t i = 0; i < threadCount; i++) {
        // - thread 생성
        CreateThread(NULL, 0, [](void* data) ->DWORD {
            Network* myThis = (Network*)data;

            //사용된 버퍼 사이즈 받아오기
            DWORD receiveBytes;
            //포트 만들때 전달된값 (클라이언트 핸들이 들어가 있다.)
            DWORD completionKey;
            //포트 만들때 전달된값 (소캣 정보의 포인터가 들어가있다.)
            struct SOCKETINFO* eventSocket;

            while (true) {
                try {
                    // 입출력 완료 대기
                    // 대기할 IOCP핸들, 수신된 바이트길이, CreateIoCompletionPort 에서 입력한값, WSASend or WSARecv 에서 넘긴 주소값
                    if (GetQueuedCompletionStatus(hIOCP, &receiveBytes, (PULONG_PTR)&completionKey, (LPOVERLAPPED *)&eventSocket, INFINITE) == 0) {
                        printf("Error - GetQueuedCompletionStatus Failure\n");
						if (myThis->clients[completionKey]->roomID > 0) {
							for (auto&i : myThis->rooms)
							{
								if (i->id == myThis->clients[completionKey]->roomID)
								{
									i->RemoveClient(myThis->clients[completionKey]);
									printf("%d번방 퇴장", i->id);
								}
							}
						}

                        myThis->clients.erase(completionKey);
                        continue;
                    }

                    //받은 버퍼 사이즈 각위치에 복사
                    eventSocket->dataBuffer.len = eventSocket->bufferSize = receiveBytes;

                    printf("Thread Playing %d \n",GetCurrentThreadId() );

                    //완료된 타입 확인하고 읽기동작이면 각 클라이언트로 전송
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

    //서버 접속대기 쓰레드
    CreateThread(NULL, 0, [](void* data) ->DWORD {
        Network* myThis = (Network*)data;

        while (true) {

            //클라이언트 접속 대기
            int addrLen = sizeof(SOCKADDR_IN);
            SOCKADDR_IN clientAddr;
            memset(&clientAddr, 0, addrLen);
            SOCKET client = accept( myThis->netSocket, (struct sockaddr *)&clientAddr, &addrLen);

            if (client == INVALID_SOCKET) {
                printf("Error - Accept Failure\n");
                continue;
            }

            //클라이언트가 접속하면 클라이언트 객체를 생성해준다.
            auto tCleint = myThis->clients[client] = std::make_shared<Client>(client);
			
			//룸 안에 인원 체크
			int roomcount = 0;
			int roomid = 1;
            //클라이언트를 룸에 접속시킨다.
			for (auto&i : myThis->rooms)
			{
				roomcount = i->AddClinet(tCleint);
				
				if ( 5 > roomcount) {
					//성공 
					i->id = roomid;
					tCleint->roomID = i->id;
					printf("\n %d번방  인원?: %d\n", i->id,i->count);
					
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
				{//방인원 초과로 실패하더라도 AddClinet 때문에 카운트가 올라가서 다시 카운트 잡아줌 
					i->count = 4;

					printf("\n %d번방  인원!?: %d\n", i->id, i->count);
				}
				roomid++;

				//printf("\n %d번방  인원: %d\n", i->id, i->count);
			}
			//  룸에 접속 실패거나  처음 생성할때 
			if (roomcount == 0 || roomcount > 4)
			{
				//룸만들어서 
				auto room = std::make_shared<Room>();
				
				//클라를 룸에 넣고 
				room->AddClinet(tCleint);

				room->id = roomid;
				tCleint->roomID = room->id;
				printf("\n %d번방  인원: %d\n", room->id, room->count);
				json j_buffer;
				j_buffer["sendcode"] = MsgCode::Playerindex;
				j_buffer["Pindex"] = room->PlayerIndexCheck();

				//strcpy_s(myThis->messageBuffer, j_buffer.dump().c_str());
				//printf("%s", myThis->messageBuffer);
				//myThis->Send(myThis->messageBuffer, strlen(myThis->messageBuffer));
				//printf("%s", myThis->messageBuffer);
				//룸을 리스트에 추가 
				myThis->rooms.push_back(room);

			}

            //클라이언트의 IOCP포트를 생성해준다.
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