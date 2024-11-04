#ifndef _LOGIC_SYSTEM_H_
#define  _LOGIC_SYSTEM_H_

#include "mySingleton.h"
#include "session.h"
#include <functional>
#include <thread>
#include <queue>
// #include "msgNode.h"
// class session;

typedef std::function<void(std::shared_ptr<session>,msgLen msgId,std::string msgData)> funCallBack;

enum MSG_IDS{
    MSG_HELLO_WORD=1001
};
class logicNode {
    friend class logicSystem;
public:
    logicNode(std::shared_ptr<session>, std::shared_ptr<recvNode>);
private:
    std::shared_ptr<session> _session;
    std::shared_ptr<recvNode> _recvnode;
};

class logicSystem:singleton<logicSystem>
{
    friend class singleton<logicSystem>;
    //???应该是模板类的问题
    // friend class session;

private:
    logicSystem();
    void dealMsg();
    void registerCallBacks();
    void helloWorldCallBack(std::shared_ptr<session>,msgLen msgId,std::string msgData);
    std::thread _workerThread;
    std::queue<std::shared_ptr<logicNode>> _msgQue;
    std::mutex _mu;
    std::condition_variable _cv;
    bool _stop;
    std::map<short,funCallBack> _funCallBack;
public:
    ~logicSystem();
    void postMsg2Que(std::shared_ptr<logicNode> msg);

};


#endif // !_LOGIC_SYSTEM_H_