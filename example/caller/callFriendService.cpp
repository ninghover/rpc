#include<iostream>
#include<string>
#include"friend.pb.h"
#include"mprpcapplication.h"
#include"mprpcchannel.h"
#include"mprpccontroller.h"
using namespace std;
using namespace fixbug;
int main(int argc,char **argv)
{
    MprpcApplication::Init(argc,argv);
    FriendServiceRpc_Stub stub(new MprpcChannel());

    GetFriendListRequest friendListRequest;
    friendListRequest.set_userid(10000);
    GetFriendListResonse friendListResponse;
    MprpcController controller;
    stub.GetFriendList(&controller,&friendListRequest,&friendListResponse,nullptr);

    if(controller.Failed())
    {
        cout<<controller.ErrorText()<<endl;
        return 0;
    }

    if(friendListResponse.result().errcode()==0)
    {
        for(int i=0;i<friendListResponse.friends_size();i++)
        {
            cout<<"index : "<<i+1<<" , name : "<<friendListResponse.friends(i)<<endl;
        }
    }

    return 0;
}