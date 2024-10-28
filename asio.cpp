#include "def.h"
// https://llfc.club/category?catid=225RaiVNI8pFDD5L4m807g7ZwmF#!aid/2LfzYBkRCfdEDrtE6hWz8VrCLoS
// https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter2.html
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

void useBufferStr(ip::tcp::socket& sock){
    asio::const_buffer buf;
    auto sendBuff=asio::buffer("hello world!");//asio::const_buffer1类型

    std::unique_ptr<char[]> uniqueBuf(new char[1024]);
    auto sendBuff1=asio::buffer(static_cast<void*>(uniqueBuf.get()),1024);
    std::fill_n(uniqueBuf.get(), 1024, 'A');


    std::string str="buffer test";
    std::size_t totalByteWritten=0;

    while (totalByteWritten!=str.size())
    {
        totalByteWritten+=sock.write_some(
            asio::buffer(str.c_str()+totalByteWritten,str.size()-totalByteWritten));
    }
    str+="\nsend test\n";
    int sendLen=sock.send(asio::buffer(str.c_str(),str.size()));
    // sendLen=asio::write(sock,asio::buffer(str.c_str(),str.size()));

    if(sendLen<=0){
        std::cout<<"ERROR\n";
    }
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
    useBufferStr(sock);

    
    return 0;
}


int main(int argc, char const *argv[])
{   
    createAcceptorSocketAndBind();
    return 0;
}
