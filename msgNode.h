#include "def.h"
#define MAX_LENGTH  1024*2
const int HEAD_LENGTH=2;
using msgLen=short;

#define HEAD_ID_LEN 2
#define HEAD_TOTAL_LEN 4
#define HEAD_DATA_LEN 2

class msgNode
{   
    friend class session;
    friend class logicSystem;

protected:
    msgLen _curLen;
    msgLen _maxLen;
    // std::shared_ptr<std::string> str_ptr;
    //由更高层次的shared_ptr<msgNode>管理
    char* _msg;
    
public:
    msgNode(char* msg,msgLen maxLen);
    msgNode(msgLen maxLen);
    ~msgNode();
    void clear();
};

//消息头=消息id+消息长度+消息内容
class recvNode :public msgNode {
public:
    recvNode(msgLen max_len, msgLen msg_id);
    int getId(){
        return _msgId;
    }
private:
    msgLen _msgId;
};
class sendNode:public msgNode {
public:
    sendNode(char* msg,msgLen max_len, msgLen msg_id);
    int getId(){
        return _msgId;
    }
private:
    msgLen _msgId;
};