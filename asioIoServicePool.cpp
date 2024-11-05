#include "asioIoServicePool.h"

asioIoServicePool::~asioIoServicePool()
{
}

boost::asio::io_context &asioIoServicePool::getIoService()
{
    auto& ioc=_iocs[_nextIoc++];
    _nextIoc/=_iocs.size();
    return ioc;
}

void asioIoServicePool::stop()
{
    for (auto& work : _works) {
        work.reset();
    }
    for (auto& t : _threads) {
        t.join();
    }
}

asioIoServicePool::asioIoServicePool(std::size_t size )
    :_iocs(size),_works(size),_nextIoc(0)
{
    for (std::size_t i = 0; i < size; ++i) {
        _works[i] = std::unique_ptr<work>(new work(_iocs[i]));
    }

    for (std::size_t i = 0; i < _iocs.size(); ++i) {
        _threads.emplace_back([this, i]() {
            _iocs[i].run();
            });
    }
}
