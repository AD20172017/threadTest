#include "session.h"
#include <csignal>
// https://llfc.club/category?catid=225RaiVNI8pFDD5L4m807g7ZwmF#!aid/2LfzYBkRCfdEDrtE6hWz8VrCLoS
// https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter2.html

int main(int argc, char const *argv[])
{
    // createAcceptorSocketAndBind();
    try
    {
    io_context ioc;
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
            ioc.stop();
            });
    server s(ioc,8888);
    ioc.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout<<"server is stop/n";


    return 0;
}
