#pragma once
#include"google/protobuf/service.h"
#include"mprpcapplication.h"
#include"muduo/net/TcpConnection.h"
#include"muduo/net/EventLoop.h"
#include"muduo/net/TcpServer.h"
#include"muduo/net/InetAddress.h"
#include<functional>
#include<string>
#include"rpcheader.pb.h"
#include"zookeeperutil.h"

#include<unordered_map>
using namespace muduo;
using namespace muduo::net;
using namespace std;
using namespace placeholders;
using namespace mprpc;
//框架提供的专门发布rpc服务的网络对象类
class RpcProvider
{
public:
    //框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service*service);

    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:

    void onConnection(const TcpConnectionPtr&);
    void onMessage(const TcpConnectionPtr&,Buffer *buffer,Timestamp time);
    //Closure回调操作,用于序列化rpc的响应和网络发送
    void SendRpcResponse(const TcpConnectionPtr&,google::protobuf::Message*);

    EventLoop m_loop;
    struct ServiceInfo //单一服务对象
    {
        google::protobuf::Service*m_service;    //服务对象
        unordered_map<string,const google::protobuf::MethodDescriptor*>m_methodMap; //该服务中的方法名及方法对象
    };
    unordered_map<string,ServiceInfo>m_serviceMap;  //服务对象名及服务对象
};