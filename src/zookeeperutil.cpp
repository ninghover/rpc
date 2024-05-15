#include "zookeeperutil.h"
#include "mprpcapplication.h"

// 全局watcher观察器 zkserver给zkclient的通知
void global_watcher(zhandle_t *zh, int type,
                    int state, const char *path, void *watcherCtx)
{
    if(type==ZOO_SESSION_EVENT)//回调的消息类型是和会话相关的消息类型
    {
        if(state==ZOO_CONNECTED_STATE)//zkclient和zkserver连接成功
        {
            sem_t *sem=(sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZKClient::ZKClient() : m_zhandle(nullptr) {}
ZKClient::~ZKClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle);
    }
}

// zkclient启动的时候连接zkserver
void ZKClient::Start()
{
    MprpcConfig config = MprpcApplication::GetInstance()->GetConfig();
    string host = config.Get("zookeeperip");
    string port = config.Get("zookeeperport");
    string connstr = host + ":" + port;
    /**
     * zookeeper_mt 多线程版本
     * zookeeper的api客户端程序提供了3个线程,分别是：
     *  1.给用户使用的API调用线程
     *  2.网络I/O线程 
     *  3.watcher回调线程
    */
    m_zhandle = zookeeper_init(connstr.c_str(),global_watcher,30000,nullptr,nullptr,0); 
    if(m_zhandle==nullptr) //返回值只表示创建了一个指针,收到ZOO_CONNECTED_STATE才表示会话创建成功
    {
        cout<<"zookeeper_init error!"<<endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem,0,0);
    zoo_set_context(m_zhandle,&sem);  //将sem设置到m_zhandle句柄中,在watcher中使用

    sem_wait(&sem);
    cout<<"zookeeper init success"<<endl;
}

// 在zkserver上根据指定的path创建znode节点 state=0表示普通创造 ZOO_EPHEMERAL(-1)表示创建一个临时节点
void ZKClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen=sizeof(path_buffer);
    if(ZNONODE==zoo_exists(m_zhandle,path,0,nullptr))
    {
        if(ZOK==zoo_create(m_zhandle,path,data,datalen,&ZOO_OPEN_ACL_UNSAFE,state,path_buffer,bufferlen))
        {
            cout<<"znode create success"<<endl;
        }
        else
        {
            cout<<"znode create error"<<endl;
            exit(EXIT_FAILURE);
        }
    }
}


// 根据参数指定的znode节点路径,获取znode节点的值
string ZKClient::GetData(const char *path)
{
    char buffer[64];
    int bufferlen=sizeof(buffer);
    if(ZOK==zoo_get(m_zhandle,path,0,buffer,&bufferlen,nullptr))
    {
        return buffer;
    }
    return "";
}
