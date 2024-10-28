#include "boost/asio.hpp"
#include "boost/system/error_code.hpp"
#include "boost/system/system_error.hpp"

#include <memory>
#include<iostream>
#include <string>
#include <cstddef>


using boost::asio::buffer;
using boost::asio::io_service;
using boost::system::error_code;
namespace ip=boost::asio::ip;

typedef std::shared_ptr<ip::tcp::socket> socket_ptr;


void func(){
    std::cout<<"baidu:";
    io_service service;
    ip::tcp::resolver resolver(service);
    ip::tcp::resolver::query query("www.baidu.com", "80");
    ip::tcp::resolver::iterator iter = resolver.resolve( query);
    ip::tcp::endpoint ep = *iter;
    // std::cout << ep.address().to_string() << ep.port() << ep.protocol()<<std::endl;
    std::cout << ep.address().to_string() << ":" << ep.port()<< '/' << ep.protocol().protocol() << std::endl;
} 

int clientEndPoint(){
    std::string ipAddress="127.0.0.1";
    unsigned short port=88;
    error_code ec;
    ip::address address=ip::address::from_string(ipAddress,ec);

    if(ec.value()!=0){
        std::cout<<"SOME THING FAIL. ERROR CODE:"
        <<ec.value()<<". MESSAGE:"<<ec.message();
        return ec.value();
    }

    ip::tcp::endpoint ep(address,port);
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
    sock.connect(ep);
    return 0;

}

int serverEndPoint(){
    unsigned short port=100;
    ip::address address=ip::address_v6::any();
    ip::tcp::endpoint ep(address,port);
    return 0;

}

int createSocket(){
    boost::asio::io_context ioc;
    ip::tcp::socket sock(ioc);
    error_code ec;
    sock.open(ip::tcp::v4(),ec);
    if(ec.value()!=0){
        std::cout<<"SOME THING FAIL. ERROR CODE:"
        <<ec.value()<<". MESSAGE:"<<ec.message();
        return ec.value();
    }
    return 0;

}

int createAcceptorSocketAndBind(){
    unsigned short port=8888;
    // ip::address address=ip::address_v6::any();
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),port);

    boost::asio::io_context ioc;
    ip::tcp::acceptor acceptor(ioc,ep.protocol());
    error_code ec;
    acceptor.bind(ep);
    acceptor.listen();
    ip::tcp::socket sock(ioc);
    acceptor.accept(sock);
    std::cout<<"Acceptted and client address:"
    <<sock.remote_endpoint().address()<<':'<<sock.remote_endpoint().port()<<'\n';    
    // acceptor.open(ip::tcp::v6(),ec);

    
    return 0;


}

int main(int argc, char const *argv[])
{   
    createAcceptorSocketAndBind();
    return 0;
}
