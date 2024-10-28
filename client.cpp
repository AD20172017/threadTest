#include "def.h"
const int MAX_LENGTH = 1024;
void readBuffer(ip::tcp::socket &sock)
{
    const unsigned int MESSAGE_SIZE = 32;
    char buf[MESSAGE_SIZE];
    std::size_t totalByte = 0;
    // while(totalByte!=MESSAGE_SIZE){
    //     totalByte+=sock.read_some(asio::buffer(buf+totalByte,MESSAGE_SIZE-totalByte));
    // }
    int readLen = asio::read(sock, asio::buffer(buf + totalByte, MESSAGE_SIZE - totalByte));
    if (readLen <= 0)
        std::cout << "FAIL\n";
    std::cout << buf << std::endl;
}

int clientEndPoint()
{
    std::string ipAddress = "127.0.0.1";
    unsigned short port = 8888;
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
    try
    {
        sock.connect(ep);
        readBuffer(sock);
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "Error occured! Error code = " << e.code()
                  << ". Message: " << e.what();
        return e.code().value();
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    try
    {
        asio::io_context ioc;
        ip::tcp::endpoint remoteEp(ip::address::from_string("127.0.0.1"), 8888);
        ip::tcp::socket sock(ioc);
        error_code error = asio::error::host_not_found;
        sock.connect(remoteEp, error);
        if (error)
        {
            std::cout << "connect fail:" << error.value() << "msg:" << error.message();
            return 0;
        }
        std::cout<<"Enter in:";
        char message[MAX_LENGTH];
        std::cin.getline(message,MAX_LENGTH);
        std::size_t msgLen=strlen(message);

        asio::write(sock,asio::buffer(message,msgLen));
        
        char reply[MAX_LENGTH];
        auto replyLen=asio::read(sock,asio::buffer(reply,msgLen));
        std::cout<<"Reply:";
        std::cout.write(reply,replyLen);
        std::cout<<'\n';


    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    

    return 0;
}