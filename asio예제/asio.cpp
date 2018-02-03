//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "stdafx.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <list>

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

#include "./asio/include/asio.hpp"

using asio::ip::tcp;

class session: public std::enable_shared_from_this<session> {
  public:
    session(tcp::socket socket)
        : socket_(std::move(socket)), // 소켓받아옴 
          timer_(socket_.get_io_service()), //타이머세팅 
          strand_(socket_.get_io_service()) { 
    }
    ~session() {
        GetSession().remove(shared_from_this());
    }

    std::list<std::shared_ptr<session>>& GetSession() {
        //색션을 관리하기 위해서 존재,
        //접근할수 없도록 전역객체를 숨겨두거나.
        //생성 초기화가 필요하나 생성 초기화가 불가능할경우 사용되는 패턴
        static std::list<std::shared_ptr<session>> sessions;
        return sessions;
    }

    void start() {
        try {
            GetSession().push_back(shared_from_this());
        } catch (const std::exception& e) {
            std::cout << e.what();
        }
        do_read();
    }
  private:
    void do_read() {
        auto self(shared_from_this());

        strand_.post([this, self]() { 
            socket_.async_read_some(asio::buffer(data_, max_length)
            , [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    do_write(length);
                    do_read();
                }
            });
        });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());

        strand_.post([this, self, length]() {
            for (auto i : GetSession()) {

                asio::async_write(socket_, asio::buffer(data_, length)
                , [this, self, i](std::error_code ec, std::size_t length) {
                    if (!ec && i == self) {
                        //전송완료
                    }
                });
            }
        });
    }

    tcp::socket socket_; //자신의 소켓
    asio::steady_timer timer_; //타이머.
    asio::io_service::strand strand_; //스트랜드

    enum { max_length = 1024 };
    char data_[max_length];
};

class server {
  public:
    server(asio::io_service& io_service, short port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
          socket_(io_service) {
        do_accept();
    }

  private:
    void do_accept() {

        //클라이언트 접속 대기
        acceptor_.async_accept(socket_,
        [this](std::error_code ec) { //일반적으로 this 이렇게 안함 예제라 간단하게 
            if (!ec) {
                //클라이언트 색션생성과 시작.
                std::make_shared<session>(std::move(socket_))->start();
            }

            //일반적인 재귀호출이 아님 // 람다절은 전역이라 자기자신을 호출하는건아님 
            do_accept(); //다시 접속자 받을 준비 
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main(int argc, char* argv[]) {
    try {

        asio::io_service io_service;//네트워크상에서 쓰고 읽을것을 만들어놓고 관리 하게해줌
        server s(io_service, std::atoi("8888"));//주소는 현재 컴퓨터를 쓰니까 별도로 안넣줌 
        io_service.run();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
