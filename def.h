#include "boost/asio.hpp"
#include "boost/system/error_code.hpp"
#include "boost/system/system_error.hpp"

#include <iostream>
#include <functional>

#include <memory>
#include<iostream>
#include <string>
#include <cstddef>



using boost::asio::io_service;
using boost::system::error_code;

class session;
class server;
class msgNode;
#define MAX_LENGTH  1024*2
const int HEAD_LENGTH=2;
using msgLen=short;
// namespace asio=boost::asio;
// namespace ip=boost::asio::ip;

// typedef std::shared_ptr<ip::tcp::socket> socket_ptr;