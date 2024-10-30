#include <iostream>
#include <functional>
#include "boost/asio.hpp"
#include "boost/system/error_code.hpp"
#include <memory>
#include <map>

using namespace boost::asio;

class server;
class session;
using session_ptr=std::shared_ptr<session>;

class session:public std::enable_shared_from_this<session>
{
private:
    ip::tcp::socket _sock;
    enum{
        max_length=1024
    };
    char _data[max_length];
    server* _ser;
    std::string _uuid;
    void handleRead(const boost::system::error_code ec, std::size_t bytesTransferred,
        session_ptr ptr);
    void handleWrite(const boost::system::error_code ec, std::size_t bytesTransferred,
        session_ptr ptr);

public:
    session(io_context& ioc,server* ser);
    ~session();
    ip::tcp::socket& getSocket(){
        return _sock;
    }

    const std::string& getUuid(){
        return _uuid;
    }
    void start();
};


class server
{   
private:
    void startAccept();
    void handleAccept(session_ptr newSession,const boost::system::error_code& ec);
    void clearSession(std::string);

    io_context& _ioc;
    ip::tcp::acceptor _acceptor;
    short _port;
    std::map<std::string,std::shared_ptr<session>> _sessions;
public:
    server(io_context& ioc,short port);
    ~server();
    friend class session;
};
