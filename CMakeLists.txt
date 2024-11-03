cmake_minimum_required(VERSION 3.16.3)
project(myAsio)
#add file here
set(srcServer server.cpp def.h session.cpp session.h msgNode.h msgNode.cpp singleton.h logicSystem.h)
set(srcClient client.cpp def.h )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(BOOST_ROOT /user/include/boost)
#set(Boost_USE_STATIC_LIBS ON)

# find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(/usr/include/jsoncpp)
find_package(jsoncpp REQUIRED)

add_executable(asioServer ${srcServer})
add_executable(asioClient ${srcClient})



# target_link_libraries(asioClient ${Boost_LIBRARIES})
#链接线程库
target_link_libraries(asioClient PRIVATE Threads::Threads jsoncpp_lib)

# target_link_libraries(asioServer ${Boost_LIBRARIES})
target_link_libraries(asioServer PRIVATE Threads::Threads jsoncpp_lib)

#set(BOOST_ROOT "/usr/include/boost")
#set(Boost_NO_SYSTEM_PATHS ON)

# find_package(Boost REQUIRED COMPONENTS asio)

# 包含 Boost 头文件目录
# include_directories(${Boost_INCLUDE_DIRS})


# target_link_libraries(your_executable_name ${Boost_LIBRARIES})

# add_library(asio INTERFACE)
# target_compile_options(asio INTERFACE ASIO_STANDALONE)
# target_include_directories(asio INTERFACE <dir where asio.hpp lives>)
# target_link_libraries(asio INTERFACE <threads>)  
# Using ASIO requires you link your final executable/library with your system's threading library (e.g. pthread on linux)