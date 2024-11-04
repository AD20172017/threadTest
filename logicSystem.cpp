#include "logicSystem.h"
logicNode::logicNode(std::shared_ptr<session>se, std::shared_ptr<recvNode>re)
    :_session(se),_recvnode(re)
{
}

logicSystem::logicSystem()
    :_stop(false)
{
    registerCallBacks();
    _workerThread=std::thread(&logicSystem::dealMsg,this);
}

void logicSystem::dealMsg()
{  
    while (1)
    {
        std::unique_lock<std::mutex> unique_lk(_mu);

        while (_msgQue.empty()&&!_stop)
        {
            _cv.wait(unique_lk);
        }

        //关闭
        if(_stop){
            while(!_msgQue.empty()){
                auto dealmsg=_msgQue.front();
                std::cout<<"logic Sys receive id is:"<<dealmsg->_recvnode->getId()<<'\n';
                auto iterCallback=_funCallBack.find(dealmsg->_recvnode->getId());
                if(iterCallback==_funCallBack.end()){
                    _msgQue.pop();
                    std::cout<<"callback failed\n";
                    continue;
                }
                iterCallback->second(dealmsg->_session,dealmsg->_recvnode->getId(),
                    std::string(dealmsg->_recvnode->_msg,dealmsg->_recvnode->_maxLen));
                _msgQue.pop();
            }
            break;
        }
        
        auto dealmsg=_msgQue.front();
        std::cout<<"logic Sys receive id is:"<<dealmsg->_recvnode->getId()<<'\n';
        auto iterCallback=_funCallBack.find(dealmsg->_recvnode->getId());
        if(iterCallback==_funCallBack.end()){
            _msgQue.pop();
            continue;
        }
        iterCallback->second(dealmsg->_session,dealmsg->_recvnode->getId(),
            std::string(dealmsg->_recvnode->_msg,dealmsg->_recvnode->_maxLen));
        _msgQue.pop();
    }
    
}

void logicSystem::registerCallBacks()
{
    _funCallBack[MSG_HELLO_WORD]=std::bind(&logicSystem::helloWorldCallBack,this,
        std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
}

void logicSystem::helloWorldCallBack(std::shared_ptr<session> se, msgLen msgId, std::string msgData)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(msgData,root);;
    std::cout<<"logic Sys recevie msg id: "<<root["id"].asInt()<<" data is: "
        <<root["data"].asString()<<std::endl;

    root["data"] = "server has received msg, msg data is " + root["data"].asString();
    std::string return_str = root.toStyledString();
    se->send(return_str.data(), root["id"].asInt());
}

logicSystem::~logicSystem()
{
    _stop=true;
    _cv.notify_one();
    _workerThread.join();
}

void logicSystem::postMsg2Que(std::shared_ptr<logicNode> msg)
{
    std::unique_lock<std::mutex> unique_lk(_mu);
    _msgQue.push(msg);
    //由0变为1则发送通知信号
    if (_msgQue.size() == 1) {
        std::cout<<"is one\n";
        _cv.notify_one();
    }
}
