#include<string>
#include<iostream>
#include<vector>
#include"friend.pb.h"
#include"mprpcapplication.h"
#include"friend.pb.h"
#include"rpcprovider.h"
using namespace std;
using namespace fixbug;

class FriendService:public FriendServiceRpc
{
public:
    vector<string>getFriendList(uint32_t userid)
    {
        cout<<"do getFriendList service here!"<<endl;
        vector<string>friends;
        friends.emplace_back("黄浩");
        friends.emplace_back("张三");
        friends.emplace_back("hoverhhuang");
        return friends;
    }

    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResonse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t userid=request->userid();
        vector<string>friends=getFriendList(userid);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");

        for(string &name:friends)
        {
            string* p=response->add_friends();
            *p=name;
        }

        done->Run();

    }
};

int main(int argc,char **argv)
{
    MprpcApplication::Init(argc,argv);
    RpcProvider rpcProvider;
    rpcProvider.NotifyService(new FriendService());
    rpcProvider.Run();
    return 0;
}
