#include "./../Network/Network.h"

#include <iostream>
int main() {
    try {
        Network net;
        net.StartServer(PF_INET, 8888, nullptr);
		
        while (true) {
            Sleep(1000);
        }
    } catch (char* e) {
        std::cout << e << std::endl;
    }
}