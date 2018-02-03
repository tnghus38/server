// Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <future>

#define ASIO_STANDALONE
#define ASIO_HEADER_ONLY
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS

#define ASIO_HAS_VARIADIC_TEMPLATES
#define ASIO_HAS_STD_FUNCTION
#define ASIO_HAS_STD_CHRONO

#define BOOST_ALL_NO_LIB
//#define _WIN32_WINNT = 0x0603
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "./../asio/asio/include/asio.hpp"

using asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[]) {
    try {
        asio::io_service io_service;

        tcp::socket s(io_service);
        tcp::resolver resolver(io_service);
        asio::connect(s, resolver.resolve({ "127.0.0.1", "8888" }));

        char request[max_length];
        char reply[max_length];

        auto temp = std::async(std::launch::async, [request, &s]() {
            while (true) {
                std::cout << "Enter message: ";
                std::cin.getline((char*)request, max_length);
                size_t request_length = std::strlen(request);
                asio::write(s, asio::buffer(request, request_length));
            }
        });

        while (true) {
            size_t size = s.read_some(asio::buffer(reply));
            reply[size] = '\0';
            std::cout << "Read message: ";
            std::cout << reply << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}