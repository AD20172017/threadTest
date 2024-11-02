#include "def.h"
#include <chrono>
using namespace boost;
using namespace std;

namespace ip=asio::ip;


// const int MAX_LENGTH = 1024;
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
    system::error_code ec;
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
    try {

        int clientNum=10000;
        //创建上下文服务
        boost::asio::io_context   ioc;
        //构造endpoint
        ip::tcp::endpoint  remote_ep(ip::address::from_string("127.0.0.1"), 8888);
        ip::tcp::socket  sock(ioc);
        boost::system::error_code   error = boost::asio::error::host_not_found; ;
        sock.connect(remote_ep, error);
        if (error) {
            cout << "connect failed, code is " << error.value() << " error msg is " << error.message();
            return 0;
        }
        
                // const char* request = "hello world!";
                // size_t request_length = strlen(request);
                // char send_data[MAX_LENGTH] = { 0 };
                // memcpy(send_data, &request_length, 2);
                // memcpy(send_data + 2, request, request_length);
                // boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));
        thread send_thread([&sock,clientNum] {
            for (int i=0;i<clientNum;++i) {
                this_thread::sleep_for(std::chrono::milliseconds(200));
                std::cout<<"send data\n";
                const char* request = "hello world!";
                size_t request_length = strlen(request);
                char send_data[MAX_LENGTH] = { 0 };
                memcpy(send_data, &request_length, 2);
                memcpy(send_data + 2, request, request_length);
                boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 2));
            }
            });
        thread recv_thread([&sock,clientNum] {
            for (int i=0;i<clientNum;++i) {
                this_thread::sleep_for(std::chrono::milliseconds(20));
                cout << "begin to receive..." << endl;
                char reply_head[HEAD_LENGTH];
                size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_LENGTH));
                short msglen = 0;
                memcpy(&msglen, reply_head, HEAD_LENGTH);
                char msg[MAX_LENGTH] = { 0 };
                size_t  msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));
                std::cout << "Reply is: ";
                std::cout.write(msg, msglen) << endl;
                std::cout << "Reply len is " << msglen;
                std::cout << "\n";
            }
            });
        send_thread.join();
        std::cout<<"send finsih\n";
        recv_thread.join();
        std::cout<<"recv finsih\n";

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}