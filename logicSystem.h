#include "session.h"
#include <functional>
#include "singleton.h"
#include <thread>
#include <queue>
typedef std::function<void(std::shared_ptr<session>,msgLen msgId,std::string msgData)> funCallBack;

class logicNode {
    friend class LogicSystem;
public:
    logicNode(std::shared_ptr<session>, std::shared_ptr<recvNode>);
private:
    std::shared_ptr<session> _session;
    std::shared_ptr<recvNode> _recvnode;
};

class logicSystem:singleton<logicSystem>
{
    friend class singleton<logicSystem>;
private:
    logicSystem(/* args */);
    void dealMsg();
    void registerCallBacks();
    void helloWorldCallBack(std::shared_ptr<session>,msgLen msgId,std::string msgData);
    std::thread workerThread;
    std::queue<std::shared_ptr<logicNode>> q;
public:
    ~logicSystem();
};

