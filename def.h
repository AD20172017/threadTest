#include "boost/asio.hpp"
#include "boost/system/error_code.hpp"
#include "boost/system/system_error.hpp"

#include <iostream>
#include <functional>

#include <memory>
#include<iostream>
#include <cstring>
#include <cstddef>



using boost::asio::io_service;
using boost::system::error_code;

class session;
class server;
class msgNode;

// namespace asio=boost::asio;
// namespace ip=boost::asio::ip;

// typedef std::shared_ptr<ip::tcp::socket> socket_ptr;