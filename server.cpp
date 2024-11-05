#include "session.h"
#include <csignal>
// https://llfc.club/category?catid=225RaiVNI8pFDD5L4m807g7ZwmF#!aid/2LfzYBkRCfdEDrtE6hWz8VrCLoS
// https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter2.html

int main(int argc, char const *argv[])
{
    // createAcceptorSocketAndBind();
    try
    {
    auto pool = asioIoServicePool::getInstance();
		boost::asio::io_context  io_context;
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&io_context,pool](auto, auto) {
			io_context.stop();
			pool->stop();
			});
		server s(io_context,8888);
		io_context.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout<<"server is stop/n";


    return 0;
}
