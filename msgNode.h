#include "def.h"
//sizeof(short)=2

class msgNode
{   
    friend class session;
private:
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
