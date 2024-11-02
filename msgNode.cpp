#include "msgNode.h"

msgNode::msgNode(char* msg,msgLen maxLen)
    :_maxLen(maxLen+HEAD_LENGTH),_curLen(0)
{   //_msg msg傻傻分不清
    _msg=new char[_maxLen+1];
    memcpy(_msg,&maxLen,HEAD_LENGTH);
    memcpy(_msg+HEAD_LENGTH,msg,maxLen);
    _msg[_maxLen]='\0';
    std::cout<<"msgNode create"<<std::endl;

}

msgNode::msgNode(msgLen maxLen)
    :_maxLen(maxLen+HEAD_LENGTH),_curLen(0)
{
    _msg=new char[maxLen+1];
}

msgNode::~msgNode()
{
    delete[] _msg;
}

void msgNode::clear()
{
    memset(_msg,0,_maxLen);
    _curLen=0;
}
