#include<iostream>
#include"test.pb.h"
using namespace fixbug;
using namespace std;

int main()
{
    UserList users;
    ResultCode*resultCode=users.mutable_result();
    resultCode->set_errcode(1);
    resultCode->set_errmsg("error");

    User*user=users.add_friends();
    user->set_name("huang");
    user->set_age(18);
    user->set_age(User::MAN);

    user=users.add_friends();
    user->set_name("huang");
    user->set_age(18);
    user->set_age(User::MAN);

    cout<<users.friends_size()<<endl;

    string str;
    users.SerializeToString(&str);
    cout<<str<<endl;

    UserList temp;
    if(temp.ParseFromString(str))
    {
        cout<<temp.friends_size()<<endl;
    }
    

    return 0;
}

/*int main()
{
    LoginRequest req;
    req.set_name("huang");
    req.set_age(23);

    string str;
    if(req.SerializeToString(&str))
    {
        cout<<"序列化成功"<<endl;
        cout<<str<<endl;
    }
    else
    {
        cout<<"序列化失败"<<endl;
    }


    //从string反序列化一个login请求对象
    LoginRequest tem;
    if(tem.ParseFromString(str))
    {
        cout<<tem.name()<<endl;
        cout<<tem.age()<<endl;
    }

    return 0;
}*/