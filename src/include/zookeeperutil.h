#pragma once
#include<semaphore.h>
#include<zookeeper/zookeeper.h>
#include<string>
#include<iostream>
using namespace std;

//封装ZK客户端类
class ZKClient
{
public:
    ZKClient();
    ~ZKClient();

    //zkclient启动的时候连接zkserver
    void Start();

    //在zkserver上根据指定的path创建znode节点 state=0表示临时节点
    void Create(const char*path,const char*data,int datalen,int state=0);

    //根据参数指定的znode节点路径,获取znode节点的值
    string GetData(const char *path);

private:
    //zk的客户端句柄
    zhandle_t*m_zhandle;
};