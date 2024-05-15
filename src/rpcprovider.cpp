#include"rpcprovider.h"
#include"google/protobuf/descriptor.h"

void RpcProvider::NotifyService(google::protobuf::Service*service)
{
    const google::protobuf::ServiceDescriptor*pserviceDesc=service->GetDescriptor();
    string serviceName=pserviceDesc->name();
    ServiceInfo serviceInfo;
    for(int i=0;i<pserviceDesc->method_count();i++)
    {
        const google::protobuf::MethodDescriptor*pmethodDesc=pserviceDesc->method(i);
        string methodName=pmethodDesc->name();

        serviceInfo.m_methodMap.insert({methodName,pmethodDesc});
    }
    serviceInfo.m_service=service;
    m_serviceMap.insert({serviceName,serviceInfo});
}

void RpcProvider::onConnection(const TcpConnectionPtr&conn)
{
    if(!conn->connected())
    {
        conn->shutdown();
    }
}
/**
 * (service_name+method_name)+args
 * UserServiceLoginzhang san
 * 
 * 第一部分:4字节，记录第二部分的长度
 * 第二部分:service_name+method_name+args
 * 
 * recv_buf：
 *  第一部分:4字节,记录第二部分的长度
 *  第二部分:service_name+method_name+args_size
 *  第三部分:args_str
*/
void RpcProvider::onMessage(const TcpConnectionPtr&conn,Buffer *buffer,Timestamp time)
{
    string recv_buf=buffer->retrieveAllAsString();
    
    //从字符流中读取前4个字节的内容
    uint32_t header_size=0;
    recv_buf.copy((char*)&header_size,4,0);//从recv_buf的第0开始,拷贝4个

    string rpc_header_str=recv_buf.substr(4,header_size);
    RpcHeader rpcHeader;
    string service_name;
    string method_name;
    uint32_t args_size;
    string args_str;
    if(rpcHeader.ParseFromString(rpc_header_str))
    {
        service_name=rpcHeader.service_name();
        method_name=rpcHeader.method_name();
        args_size=rpcHeader.args_size();
        args_str=recv_buf.substr(4+header_size,args_size);
    }
    else
    {
        cout<<"rpc_header_str : "<<rpc_header_str<<" parse error!"<<endl;
        return;
    }

    cout<<"-------------------------------------"<<endl;
    cout<<"service_name : "<<service_name<<endl;
    cout<<"method_name : "<<method_name<<endl;
    cout<<"args_str : "<<args_str<<endl;
    cout<<"-------------------------------------"<<endl;

    auto it=m_serviceMap.find(service_name);
    if(it==m_serviceMap.end())
    {
        cout<<service_name<<" is not exits!"<<endl;
        return;
    }
    auto mit=it->second.m_methodMap.find(method_name);
    if(mit==it->second.m_methodMap.end())
    {
        cout<<service_name<<" : "<<method_name<<" is not exits!"<<endl;
        return;
    }
    google::protobuf::Service * service=it->second.m_service;
    const google::protobuf::MethodDescriptor*method=mit->second;

    google::protobuf::Message*request=service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str))
    {
        cout<<"request parse error,content:"<<args_str<<endl;
        return;
    }

    google::protobuf::Message*response=service->GetResponsePrototype(method).New();

    //给下mainCallMethod方法绑定一个Closured回调
    google::protobuf::Closure*done =google::protobuf::NewCallback
                    <RpcProvider,const TcpConnectionPtr&,google::protobuf::Message*>
                    (this,&RpcProvider::SendRpcResponse,conn,response);

    service->CallMethod(method,nullptr,request,response,done);


}
void RpcProvider::SendRpcResponse(const TcpConnectionPtr&conn,google::protobuf::Message*response)
{
    string response_str;
    if(response->SerializeToString(&response_str))
    {
        conn->send(response_str);
    }
    else
    {
        cout<<"serialize response_str error!"<<endl;
    }
    conn->shutdown();
}

void RpcProvider::Run()
{
    string ip=MprpcApplication::GetInstance()->GetConfig().Get("rpcserverip");
    uint16_t port=atoi(MprpcApplication::GetInstance()->GetConfig().Get("rpcserverport").c_str());
    InetAddress addr(ip,port);
    TcpServer server(&m_loop,addr,"provider");

    //注册回调
    server.setConnectionCallback(bind(&RpcProvider::onConnection,this,_1));
    server.setMessageCallback(bind(&RpcProvider::onMessage,this,_1,_2,_3));
    server.setThreadNum(4);


    char method_path_data[128]={0};
    sprintf(method_path_data,"%s:%d",ip.c_str(),port);
    ZKClient zk;
    zk.Start();
    for(auto&service:m_serviceMap)
    {
        string service_path="/"+service.first;
        zk.Create(service_path.c_str(),nullptr,0);
        for(auto&method:service.second.m_methodMap)
        {
            string method_path=service_path+"/"+method.first;

            zk.Create(method_path.c_str(),method_path_data,strlen(method_path_data),ZOO_EPHEMERAL);
        }
    }


    cout<<"RpcProvider start service at ip:"<<ip<<" port:"<<port<<endl;

    server.start();
    m_loop.loop();

}