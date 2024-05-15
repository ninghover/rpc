#include<iostream>
#include<string>
#include"user.pb.h"
#include"mprpcapplication.h"
#include"rpcprovider.h"
#include"logger.h"
using namespace std;
using namespace fixbug;

class UserService:public UserServiceRpc
{
public:
    bool Login(string name,string pwd)
    {
        cout<<"-------------------------------------"<<endl;
        cout<<"......login......"<<endl;
        cout<<"name : "<<name<<endl;
        cout<<"pwd : "<<pwd<<endl;
        cout<<"-------------------------------------"<<endl;
        return true;
    }

    bool Register(string name,string pwd)
    {
        cout<<"-------------------------------------"<<endl;
        cout<<"......register......"<<endl;
        cout<<"name : "<<name<<endl;
        cout<<"pwd : "<<pwd<<endl;
        cout<<"-------------------------------------"<<endl;
        return true;        
    }

    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        //框架给业务上报请求参数LoginRequest,应用获取数据做本地业务
        string name=request->name();
        string pwd=request->pwd();

        //做本地业务
        if(Login(name,pwd))
        {
            //把响应写入 报错错误码、错误消息、返回值
            ResultCode*result=response->mutable_result();
            result->set_errcode(0);
            result->set_errmsg("");
            response->set_success("登录成功！");
        }
        else
        {
            ResultCode*result=response->mutable_result();
            result->set_errcode(1);
            result->set_errmsg("登录错误");
            response->set_success("登录失败！");
        }
        //执行回调 (框架执行响应对象数据的序列化和网络发送)
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        string name=request->name();
        string pwd=request->pwd();
        
        bool ret=Register(name,pwd);

        if(ret)
        {
            ResultCode*resCode= response->mutable_result();
            resCode->set_errcode(0);
            resCode->set_errmsg("");
            response->set_success("注册成功 ");
        }
        else
        {
            ResultCode*resCode=response->mutable_result();
            resCode->set_errcode(1);
            resCode->set_errmsg("注册错误");
            response->set_success("注册失败");
        }

        done->Run();
    }
};

int main(int argc,char**argv)
{

    LOG_ERR("%s:%s,%d",__FILE__,__FUNCTION__,__LINE__);
    LOG_INFO("信息");
    LOG_ERR("错误");

    //调用框架的初始化操作
    MprpcApplication::Init(argc,argv);

    //provider是一个rpc网络服务对象
    RpcProvider provider;   

    //把UserService对象发布到rpc节点上
    provider.NotifyService(new UserService());

    //启动一个rpc服务发布节点,Run以后进程进入阻塞状态,等待远程rpc调用请求
    provider.Run();

    return 0;
}