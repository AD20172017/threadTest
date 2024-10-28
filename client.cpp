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

int clientEndPoint(){
    std::string ipAddress="127.0.0.1";
    unsigned short port=8888;
    error_code ec;
    ip::address address=ip::address::from_string(ipAddress,ec);

    if(ec.value()!=0){
        std::cout<<"SOME THING FAIL. ERROR CODE:"
        <<ec.value()<<". MESSAGE:"<<ec.message();
        return ec.value();
    }
    try {
        ip::tcp::endpoint ep(address,port);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
    }
    catch(boost::system::system_error& e){
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
        return e.code().value();
    }
    return 0;
}


int main(int argc, char const *argv[])
{   
    clientEndPoint();

    return 0;
}