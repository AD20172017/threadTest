#include "session.h"

// https://llfc.club/category?catid=225RaiVNI8pFDD5L4m807g7ZwmF#!aid/2LfzYBkRCfdEDrtE6hWz8VrCLoS
// https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter2.html

int main(int argc, char const *argv[])
{
    // createAcceptorSocketAndBind();
    try
    {
    io_context ioc;
    server s(ioc,8888);
    ioc.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    


    return 0;
}
