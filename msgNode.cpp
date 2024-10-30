#include "msgNode.h"

msgNode::msgNode(char* msg,int maxLen)
    :_maxLen(maxLen),_curLen(0)
{   //_msg msg傻傻分不清
    _msg=new char[maxLen];
    memcpy(_msg,msg,maxLen);
}

msgNode::~msgNode()
{
    delete[] _msg;
}