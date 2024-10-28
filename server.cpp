#include "def.h"
#include <set>
// https://llfc.club/category?catid=225RaiVNI8pFDD5L4m807g7ZwmF#!aid/2LfzYBkRCfdEDrtE6hWz8VrCLoS
// https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter2.html

const int MAX_LENGTH = 1024;

std::set<std::shared_ptr<std::thread>> thread_set;

int clientEndPoint()
{
    std::string ipAddress = "127.0.0.1";
    unsigned short port = 88;
    error_code ec;
    ip::address address = ip::address::from_string(ipAddress, ec);

    if (ec.value() != 0)
    {
        std::cout << "SOME THING FAIL. ERROR CODE:"
                  << ec.value() << ". MESSAGE:" << ec.message();
        return ec.value();
    }

    ip::tcp::endpoint ep(address, port);
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
    sock.connect(ep);
    return 0;
}

void useBufferStr(ip::tcp::socket &sock)
{
    asio::const_buffer buf;
    auto sendBuff = asio::buffer("hello world!"); // asio::const_buffer1类型

    std::unique_ptr<char[]> uniqueBuf(new char[1024]);
    auto sendBuff1 = asio::buffer(static_cast<void *>(uniqueBuf.get()), 1024);
    std::fill_n(uniqueBuf.get(), 1024, 'A');

    std::string str = "buffer test";
    std::size_t totalByteWritten = 0;

    while (totalByteWritten != str.size())
    {
        totalByteWritten += sock.write_some(
            asio::buffer(str.c_str() + totalByteWritten, str.size() - totalByteWritten));
    }
    str += "\nsend test\n";
    int sendLen = sock.send(asio::buffer(str.c_str(), str.size()));
    // sendLen=asio::write(sock,asio::buffer(str.c_str(),str.size()));

    if (sendLen <= 0)
    {
        std::cout << "ERROR\n";
    }
}

int createAcceptorSocketAndBind()
{
    unsigned short port = 8888;
    // ip::address address=ip::address_v6::any();
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), port);

    boost::asio::io_context ioc;
    ip::tcp::acceptor acceptor(ioc, ep.protocol());
    error_code ec;
    acceptor.bind(ep);
    acceptor.listen();
    ip::tcp::socket sock(ioc);
    acceptor.accept(sock);
    std::cout << "Acceptted and client address:"
              << sock.remote_endpoint().address() << ':' << sock.remote_endpoint().port() << '\n';
    // acceptor.open(ip::tcp::v6(),ec);
    useBufferStr(sock);

    return 0;
}

void session(socket_ptr sock) {
    try {
        for (;;) {
            char data[MAX_LENGTH];
            memset(data, '\0', MAX_LENGTH);
            boost::system::error_code error;
            //read会一直阻塞
            size_t length = sock->read_some(boost::asio::buffer(data, MAX_LENGTH), error);
            if (error == boost::asio::error::eof) {
                std::cout << "connection closed by peer" << std::endl;
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }
            std::cout << "receive from " << sock->remote_endpoint().address().to_string() << std::endl;
            std::cout << "receive message is " << data << std::endl;
            //回传信息值
            boost::asio::write(*sock, boost::asio::buffer(data, length));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n" << std::endl;
    }
}

void server(boost::asio::io_context& io_context, unsigned short port) {
    ip::tcp::acceptor a(io_context, ip::tcp::endpoint(ip::tcp::v4(), port));
    for (;;) {
        socket_ptr socket(new ip::tcp::socket(io_context));
        a.accept(*socket);
        auto t =  std::make_shared<std::thread>(session, socket);
        thread_set.insert(t);
    }
}

int main(int argc, char const *argv[])
{
    // createAcceptorSocketAndBind();
    try
    {
    asio::io_context ioc;
    server(ioc,8888);

    for(auto&t:thread_set){
        t->join();
    }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    return 0;
}
