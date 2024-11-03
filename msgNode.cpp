#include "msgNode.h"

msgNode::msgNode(char* msg,msgLen maxLen)
    :_maxLen(maxLen),_curLen(0)
{   //_msg msg傻傻分不清
    _msg=new char[_maxLen+1];
    _msg[_maxLen]='\0';
    // memcpy(_msg,&maxLen,HEAD_LENGTH);
    // memcpy(_msg+HEAD_LENGTH,msg,maxLen);
    // std::cout<<"msgNode create"<<std::endl;

}

msgNode::msgNode(msgLen maxLen)
    :_maxLen(maxLen),_curLen(0)
{
    _msg=new char[maxLen+1];
    _msg[_maxLen]='\0';

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

recvNode::recvNode(msgLen max_len, msgLen msg_id)
    :msgNode(max_len),_msgId(msg_id)
{
}

sendNode::sendNode( char *msg, msgLen max_len, msgLen msg_id)
    :msgNode(max_len+HEAD_TOTAL_LEN),_msgId(msg_id)
{
    msgLen msgIdHost=boost::asio::detail::socket_ops::host_to_network_short(_msgId);
    memcpy(_msg,&msgIdHost,HEAD_ID_LEN);

    msgLen maxLenHost=boost::asio::detail::socket_ops::host_to_network_short(max_len);
    memcpy(_msg+HEAD_ID_LEN,&maxLenHost,HEAD_DATA_LEN);
    memcpy(_msg+HEAD_ID_LEN+HEAD_DATA_LEN,msg,max_len);

}
