#include "mySingleton.h"
#include <thread>
#include <vector>
#include "boost/asio.hpp"
#include <cstddef>


class asioIoServicePool: public singleton<asioIoServicePool>
{
    friend class singleton<asioIoServicePool>;
public:
    ~asioIoServicePool();
    asioIoServicePool(const asioIoServicePool&)=delete;
    asioIoServicePool&operator=(const asioIoServicePool&)=delete;

    using IoContext=boost::asio::io_context;
    using work=IoContext::work;
    using workPtr=std::unique_ptr<work>;
    boost::asio::io_context& getIoService();

    void stop();
private:
    asioIoServicePool(std::size_t size=std::thread::hardware_concurrency());
    std::vector<IoContext> _iocs;
    std::vector<workPtr> _works;
    std::vector<std::thread> _threads;
    std::size_t _nextIoc;

    
};

