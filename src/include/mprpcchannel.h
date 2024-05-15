#pragma once
#include<google/protobuf/service.h>
#include"zookeeperutil.h"

using namespace google::protobuf;

class MprpcChannel:public google::protobuf::RpcChannel
{
public:
    //所有通过stub代理对象调用的rpc方法,都走到这里,统一做rpc方法调用的数据序列化和网络发送
    void CallMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done);
};