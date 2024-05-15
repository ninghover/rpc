#include"mprpcchannel.h"
#include<google/protobuf/descriptor.h>
#include<google/protobuf/message.h>
#include<string>
#include"rpcheader.pb.h"
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
// #include"mprpcapplication.h"
#include"mprpccontroller.h"
#include"logger.h"
using namespace std;
using namespace mprpc;
/**
 * header_size + service_name method_name args_size + args
*/

//所有通过Stub代理对象调用的rpc方法,都走到了这里,统一做rpc方法调用的数据序列化和网络发送
void MprpcChannel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done)
{
    const ServiceDescriptor*sd= method->service();
    string service_name=sd->name();
    string method_name=method->name();

    //获取参数的序列化字符串长度
    string args_str;
    uint32_t args_size=0;
    if(request->SerializeToString(&args_str))
    {
        args_size=args_str.size();
    }
    else
    {
        controller->SetFailed("Serialize request error!");
        return;
    }

    //定义rpc的请求header
    RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);
    
    cout<<"-------------------------------------"<<endl;
    cout<<"service_name : "<<service_name<<endl;
    cout<<"method_name : "<<method_name<<endl;
    cout<<"args_str : "<<args_str<<endl;
    cout<<"-------------------------------------"<<endl;
    
    string rpcHeader_str;
    uint32_t header_size=0;
    if(rpcHeader.SerializeToString(&rpcHeader_str))
    {
        header_size=rpcHeader_str.size();
    }
    else
    {
        controller->SetFailed("Serialize rpc header error!");
        return ;
    }
    string send_str;
    send_str.insert(0,string((char*)&header_size,4));
    send_str+=rpcHeader_str;
    send_str+=args_str;

    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==clientfd)
    {
        controller->SetFailed("create socket error!");
        return ;
    }

    // string ip= MprpcApplication::GetInstance()->GetConfig().Get("rpcserverip");
    // uint16_t port=atoi(MprpcApplication::GetInstance()->GetConfig().Get("rpcserverport").c_str());
    
    ZKClient zk;
    zk.Start();
    string path="/"+service_name+"/"+method_name;
    string conn_str=zk.GetData(path.c_str());
    
    int ind=conn_str.find(":");
    string ip=conn_str.substr(0,ind);
    uint16_t port=atoi((conn_str.substr(ind+1,conn_str.size()-ip.size()-1)).c_str());
    
    LOG_INFO("ip = %s",ip.c_str());
    LOG_INFO("port = %d",port);


    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(ip.c_str());

    if(-1==connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        controller->SetFailed("connect error!");
        close(clientfd);
        return;
    }
    if(-1==send(clientfd,send_str.c_str(),send_str.size(),0))
    {
        controller->SetFailed("send error!");
        close(clientfd);
        return;
    }

    //接收rpc响应
    char buf[1024]={0};
    int recv_size=0;
    if(-1==(recv_size=recv(clientfd,buf,1024,0)))
    {
        controller->SetFailed("recv error!");
        close(clientfd);
        return ;
    }
    
    if(!response->ParseFromArray(buf,recv_size))
    {
        controller->SetFailed("parse error");
        close(clientfd);
        return;
    }
    
    close(clientfd);
}