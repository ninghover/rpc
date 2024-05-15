#include<iostream>
#include"mprpcapplication.h"
#include"user.pb.h"
#include"mprpcchannel.h"
#include"mprpccontroller.h"
using namespace std;
using namespace fixbug;
int main(int argc,char **argv)
{
    MprpcApplication::Init(argc,argv);
    UserServiceRpc_Stub stub(new MprpcChannel());

    //rpc方法的请求参数
    LoginRequest loginRequest;
    loginRequest.set_name("huang hao");
    loginRequest.set_pwd("123");
    //rpc方法的响应
    LoginResponse loginResponse;
    //发起rpc方法的调用 同步的rpc调用过程 MprpcChannel::callmethod
    //RpcChannel->RpcChannel::callMethod集中来做所有
    MprpcController controller;
    stub.Login(&controller,&loginRequest,&loginResponse,nullptr);
    if(controller.Failed())
    {
        cout<<controller.ErrorText()<<endl;
        return 0;
    }
    if(loginResponse.result().errcode()==0)
    {
        cout<<"rpc login response success:"<<loginResponse.success()<<endl;
    }
    else
    {
        cout<<"rpc login response error"<<loginResponse.result().errmsg()<<endl;
    }

    RegisterRequest registerRequest;
    registerRequest.set_name("ningtao");
    registerRequest.set_pwd("123456");
    RegisterResponse registerResponse;
    stub.Register(nullptr,&registerRequest,&registerResponse,nullptr);
    if(registerResponse.result().errcode()==0)
    {
        cout<<"rpc register response success:"<<registerResponse.success()<<endl;
    }
    else
    {
        cout<<"rpc register response error"<<registerResponse.result().errmsg()<<endl;
    }

    return 0;
}