#include <iostream>
#include <functional>
#include "boost/asio.hpp"
#include "boost/system/error_code.hpp"
using namespace boost::asio;
class session
{
private:
    ip::tcp::socket _sock;
    enum{
        max_length=1024
    };
    char _data[max_length];

    void handleRead(const boost::system::error_code ec, std::size_t bytesTransferred);
    void handleWrite(const boost::system::error_code ec, std::size_t bytesTransferred);

public:
    session(io_context& ioc);
    ~session();
    ip::tcp::socket& getSocket(){
        return _sock;
    }

    void start();
};


class server
{
private:
    void startAccept();
    void handleAccept(session* newSession,const boost::system::error_code& ec);

    io_context& _ioc;
    ip::tcp::acceptor _acceptor;

public:
    server(io_context& ioc,short port);
    ~server();
};
