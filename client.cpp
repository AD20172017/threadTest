#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/system/error_code.hpp"
#include <memory>
#include<iostream>
#include <string>
#include <cstddef>


using boost::asio::buffer;
using boost::asio::io_service;
namespace ip=boost::asio::ip;

typedef std::shared_ptr<ip::tcp::socket> socket_ptr;

io_service service;
ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),80);
size_t read_complete(char * buf, const error_code & err, size_t bytes)
{
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // 我们一个一个读取直到读到回车，不缓存
    return found ? 0 : 1;
}

void sync_echo(std::string msg) {
    msg += '\n';
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(msg));
    char buf[1024];
    int bytes = read(sock, buffer(buf), boost::bind(read_complete,buf,_1,_2));
    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);
    std::cout << "server echoed our " << msg << ": "<< (copy == msg ? "OK" : "FAIL") << std::endl;
    sock.close();
}
int main(int argc, char* argv[]) {
    char* messages[] = { "John says hi", "so does James", "Lucy just got home", "Boost.Asio is Fun!", 0 };
    boost::thread_group threads;
    for ( char ** message = messages; *message; ++message) {
        threads.create_thread( boost::bind(sync_echo, *message));
        std::this_thread::sleep( boost::posix_time::millisec(100));
    }
    threads.join_all();
}