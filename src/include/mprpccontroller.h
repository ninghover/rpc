#pragma once
#include<google/protobuf/service.h>
#include<string>

using namespace google::protobuf;
using namespace std;

class MprpcController:public google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset();
    bool Failed() const;
    string ErrorText()const;
    void SetFailed(const string &reason);

    //目前没有实现的功能
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(Closure*callback);

private:
    bool m_failed;  //RPC方法执行过程中的状态
    string m_errText;   //RPC方法执行过程中的错误信息
};