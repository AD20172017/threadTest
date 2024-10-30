#include "def.h"

class msgNode
{
    friend class session;
private:
    int _curLen;
    int _maxLen;
    // std::shared_ptr<std::string> str_ptr;
    //由更高层次的shared_ptr<msgNode>管理
    char* _msg;
    
public:
    msgNode(char* msg,int maxLen);
    ~msgNode();
};
