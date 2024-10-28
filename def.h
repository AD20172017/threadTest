#include "boost/asio.hpp"
#include "boost/system/error_code.hpp"
#include "boost/system/system_error.hpp"

#include <memory>
#include<iostream>
#include <string>
#include <cstddef>



using boost::asio::io_service;
using boost::system::error_code;

namespace asio=boost::asio;
namespace ip=boost::asio::ip;

typedef std::shared_ptr<ip::tcp::socket> socket_ptr;