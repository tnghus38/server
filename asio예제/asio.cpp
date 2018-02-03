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
        : socket_(std::move(socket)), // ���Ϲ޾ƿ� 
          timer_(socket_.get_io_service()), //Ÿ�̸Ӽ��� 
          strand_(socket_.get_io_service()) { 
    }
    ~session() {
        GetSession().remove(shared_from_this());
    }

    std::list<std::shared_ptr<session>>& GetSession() {
        //������ �����ϱ� ���ؼ� ����,
        //�����Ҽ� ������ ������ü�� ���ܵΰų�.
        //���� �ʱ�ȭ�� �ʿ��ϳ� ���� �ʱ�ȭ�� �Ұ����Ұ�� ���Ǵ� ����
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
                        //���ۿϷ�
                    }
                });
            }
        });
    }

    tcp::socket socket_; //�ڽ��� ����
    asio::steady_timer timer_; //Ÿ�̸�.
    asio::io_service::strand strand_; //��Ʈ����

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

        //Ŭ���̾�Ʈ ���� ���
        acceptor_.async_accept(socket_,
        [this](std::error_code ec) { //�Ϲ������� this �̷��� ���� ������ �����ϰ� 
            if (!ec) {
                //Ŭ���̾�Ʈ ���ǻ����� ����.
                std::make_shared<session>(std::move(socket_))->start();
            }

            //�Ϲ����� ���ȣ���� �ƴ� // �������� �����̶� �ڱ��ڽ��� ȣ���ϴ°Ǿƴ� 
            do_accept(); //�ٽ� ������ ���� �غ� 
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main(int argc, char* argv[]) {
    try {

        asio::io_service io_service;//��Ʈ��ũ�󿡼� ���� �������� �������� ���� �ϰ�����
        server s(io_service, std::atoi("8888"));//�ּҴ� ���� ��ǻ�͸� ���ϱ� ������ �ȳ��� 
        io_service.run();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
