#include "boost/asio.hpp"
#include "boost/system"
#include <memory>
#include<iostream>
#include <string>
#include <cstddef>


using boost::asio::buffer;
using boost::asio::io_service;
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


int main(int argc, char const *argv[])
{   

    return 0;
}
