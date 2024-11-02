#include "session.h"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
void session::send(char *msg, int max_length)
{   //算是解耦操作吗
    bool isEmpty = true;
    std::lock_guard<std::mutex> lock(msgMutex);
    if (msgQueue.size() > 0) {
        isEmpty= false;
    }
    
    //debug
    msgQueue.push(std::make_shared<msgNode>(msg,max_length));
    

    if(!isEmpty){
        std::cout<<"MsgQueue is full\n";
        return;
    }
    std::cout<<"MsgQueue is empty\n";
    //写第一条数据
    async_write(_sock,buffer(msg,max_length),
        std::bind(&session::handleWrite,this,std::placeholders::_1,std::placeholders::_2,shared_from_this()));

}
void session::handleRead(const boost::system::error_code ec, std::size_t bytesTransferred, session_ptr ptr)
{
    if(!ec.value()){
        std::cout<<"start read:"<<'\n';
        
        int copyLen=0;
        while(bytesTransferred>0){
            if(!headDone){
                std::cout<<"start accept msgHead\n";
                //头部未接受完
                if(bytesTransferred+recvHeadNode->_curLen<HEAD_LENGTH){
                    memcpy(recvHeadNode->_msg+recvHeadNode->_curLen,_data+copyLen,bytesTransferred);

                    recvHeadNode->_curLen+=bytesTransferred;
                    memset(_data,0,MAX_LENGTH);
                    _sock.async_read_some(buffer(_data,MAX_LENGTH),
                        std::bind(&session::handleRead,this,std::placeholders::_1,std::placeholders::_2,ptr));
                    
                    return;
                }

                int headRemain=HEAD_LENGTH-recvHeadNode->_curLen;
                memcpy(recvHeadNode->_msg+recvHeadNode->_curLen,_data+copyLen,headRemain);
                copyLen+=headRemain;
                bytesTransferred-=headRemain;

                msgLen dataLen=0;

                memcpy(&dataLen,recvHeadNode->_msg,HEAD_LENGTH);
                std::cout<<"data len:"<<dataLen<<'\n';

                if(dataLen>MAX_LENGTH){
                    std::cerr<<"data length is invalid:"<<dataLen<<std::endl;
                    _ser->clearSession(_uuid);
                    return;
                }
            std::cout<<"msg Head finish, start accept data\n";


                recvMsgNode=std::make_shared<msgNode>(dataLen);
                if(bytesTransferred<dataLen){
                    memcpy(recvMsgNode->_msg + recvMsgNode->_curLen, _data + copyLen, bytesTransferred);
                    recvMsgNode->_curLen += bytesTransferred;
                    ::memset(_data, 0, MAX_LENGTH);
                    _sock.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), 
                        std::bind(&session::handleRead, this, std::placeholders::_1, std::placeholders::_2, ptr));
                    //头部处理完成
                    headDone = true;
                    return;
                }
                memcpy(recvMsgNode->_msg + recvMsgNode->_curLen, _data + copyLen, dataLen);
                recvMsgNode->_curLen += dataLen;
                copyLen += dataLen;
                bytesTransferred -= dataLen;
                recvMsgNode->_msg[recvMsgNode->_maxLen] = '\0';
                std::cout << "receive data is " << recvMsgNode->_msg << std::endl;
                //此处可以调用Send发送测试
                send(recvMsgNode->_msg, recvMsgNode->_maxLen);
                //继续轮询剩余未处理数据
                headDone = false;
                recvHeadNode->clear();
                if (bytesTransferred <= 0) {
                    ::memset(_data, 0, MAX_LENGTH);
                    _sock.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), 
                        std::bind(&session::handleRead, this, std::placeholders::_1, std::placeholders::_2, ptr));
                    return;
                }
                continue;

            }
            //已经处理完头部，处理上次未接受完的消息数据
            //接收的数据仍不足剩余未处理的
            int remainMsg = recvMsgNode->_maxLen - recvMsgNode->_curLen;
            if (bytesTransferred < remainMsg) {
                memcpy(recvMsgNode->_msg + recvMsgNode->_curLen, _data + copyLen, bytesTransferred);
                recvMsgNode->_curLen += bytesTransferred;
                ::memset(_data, 0, MAX_LENGTH);
                _sock.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), 
                    std::bind(&session::handleRead, this, std::placeholders::_1, std::placeholders::_2, ptr));
                return;
            }
            memcpy(recvMsgNode->_msg + recvMsgNode->_curLen, _data + copyLen, remainMsg);
            recvMsgNode->_curLen += remainMsg;
            bytesTransferred -= remainMsg;
            copyLen += remainMsg;
            recvMsgNode->_msg[recvMsgNode->_maxLen] = '\0';
            std::cout << "receive data is " << recvMsgNode->_msg << std::endl;
            //此处可以调用Send发送测试
            send(recvMsgNode->_msg, recvMsgNode->_maxLen);
            //继续轮询剩余未处理数据
            headDone = false;
            recvHeadNode->clear();
            if (bytesTransferred <= 0) {
                ::memset(_data, 0, MAX_LENGTH);
                _sock.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
                    std::bind(&session::handleRead, this, std::placeholders::_1, std::placeholders::_2, ptr));
                return;
            }
            continue;


        }



        // std::cout<<"Server receive data:"<<_data<<std::endl;
        //     // send(_data,bytesTransferred);
        //     try
        //     {   
        //         send(_data,bytesTransferred);
        //         memset(_data,0,bytesTransferred);
        //     }
        //     catch(const std::exception& e)
        //     {
        //         std::cerr << e.what() << '\n';
        //     }
            
            
        //     std::cout<<"restart read:\n";
        //     _sock.async_read_some(buffer(_data,max_length),
        //         std::bind(&session::handleRead,this,std::placeholders::_1,std::placeholders::_2,ptr));
            
    }else{
        std::cout<<"Read error\n";
        // delete this;
        close();
        _ser->clearSession(_uuid);

    }
}

void session::handleWrite(const boost::system::error_code ec, std::size_t bytesTransferred,session_ptr ptr)
{
    if(!ec.value()){
        std::lock_guard<std::mutex> lock(msgMutex);
        msgQueue.pop();//确定发送完成后在清除数据
        if(!msgQueue.empty()){
            auto& msg=msgQueue.front();
            
            async_write(_sock,buffer(msg->_msg,msg->_maxLen),
               std::bind(&session::handleRead,this,std::placeholders::_1,std::placeholders::_2,ptr));

        }
        // memset(_data,0,max_length);
        // _sock.async_read_some(buffer(_data,max_length),
        //    std::bind(&session::handleRead,this,std::placeholders::_1,std::placeholders::_2,ptr));

    }else{
        std::cout<<"Write error\n"<<ec.message()<<std::endl;;
        _ser->clearSession(_uuid);
    }
}

session::session(io_context &ioc,server* ser) : _sock(ioc),_ser(ser),queueIsEmpty(true),sockIsClose(false)
{
    boost::uuids::uuid id=boost::uuids::random_generator()();
    _uuid=boost::uuids::to_string(id);
    recvHeadNode=std::make_shared<msgNode>(HEAD_LENGTH);

}

session::~session()
{
    std::cout<<"this session is finish!\n";
}

void session::start()
{   
    memset(_data,0,max_length);
    std::cout<<"start session and set msg len:"<<max_length<<'\n';
    _sock.async_read_some(buffer(_data,max_length),
        std::bind(&session::handleRead,this,std::placeholders::_1,std::placeholders::_2,shared_from_this()));
}

void server::startAccept()
{
    auto newSesssion = std::make_shared<session>(_ioc,this);
    _acceptor.async_accept(newSesssion->getSocket(),
        std::bind(&server::handleAccept,this,newSesssion,std::placeholders::_1));

}

void server::handleAccept(session_ptr newSession, const boost::system::error_code &ec)
{
    if(!ec.value()){
        newSession->start();
        _sessions.insert({newSession->getUuid(),newSession});
    }else{
        // delete newSession;
        clearSession(newSession->getUuid());
        std::cout << "session accept failed, error is " << ec.message() << std::endl;
    }
    startAccept();

}

void server::clearSession(std::string str)
{   std::cout<<"clear uuid:"<<str<<std::endl;
    _sessions.erase(str);
}

server::server(io_context &ioc, short port)
    :_ioc(ioc),_acceptor(ioc,ip::tcp::endpoint(ip::tcp::v4(),port)),_port(port)
{
    startAccept();
}

server::~server()
{
}

void session::close()
{
    _sock.close();
    sockIsClose=true;
}
