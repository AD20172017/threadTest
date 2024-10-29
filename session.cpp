#include "session.h"

void session::handleRead(const boost::system::error_code ec, std::size_t bytesTransferred)
{
    if(!ec.value()){
        std::cout<<"Server receive data:"<<_data<<std::endl;
            async_write(_sock,buffer(_data,bytesTransferred),
                std::bind(&session::handleWrite,this,std::placeholders::_1,std::placeholders::_2));
            
    }else{
        std::cout<<"Read error\n";
        delete this;
    }
}

void session::handleWrite(const boost::system::error_code ec, std::size_t bytesTransferred)
{
    if(!ec.value()){
        memset(_data,0,max_length);
        _sock.async_read_some(buffer(_data,max_length),
           std::bind(&session::handleRead,this,std::placeholders::_1,std::placeholders::_2));

    }else{
        std::cout<<"Write error\n";
        delete this;
    }
}

session::session(io_context &ioc) : _sock(ioc)
{
}

session::~session()
{

}

void session::start()
{
    memset(_data,0,max_length);
    _sock.async_read_some(buffer(_data,max_length),
        std::bind(&session::handleRead,this,std::placeholders::_1,std::placeholders::_2));
}

void server::startAccept()
{
    auto newSesssion = new session(_ioc);
    _acceptor.async_accept(newSesssion->getSocket(),
        std::bind(&server::handleAccept,this,newSesssion,std::placeholders::_1));

}

void server::handleAccept(session* newSession, const boost::system::error_code &ec)
{
    if(!ec.value()){
        newSession->start();
    }else{
        delete newSession;
    }
    startAccept();

}

server::server(io_context &ioc, short port)
    :_ioc(ioc),_acceptor(ioc,ip::tcp::endpoint(ip::tcp::v4(),port))
{
    startAccept();
}

server::~server()
{
}
