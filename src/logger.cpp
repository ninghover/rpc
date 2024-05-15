#include"logger.h"
#include<time.h>
#include<iostream>

using namespace std;


//把日志写入lockqueue中
void Logger::Log(Level logLevel,string msg)
{
    m_lockQueue.Push({logLevel,msg});
}

Logger&Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
{
    //把lockqueue中的日志写入磁盘上
    thread writeLogTask([&](){
        while(1)
        {
            time_t now=time(nullptr);
            tm*tmnow=localtime(&now);
            char filename[64]={0};
            sprintf(filename,"../log/%4d-%2d-%2d-log.txt",tmnow->tm_year+1900,tmnow->tm_mon+1,tmnow->tm_mday);
            FILE *fp=fopen(filename,"a+");
            if(nullptr==fp)
            {
                cout<<"logger file : "<<filename<<" open fail!"<<endl;
                exit(EXIT_FAILURE);
            }

            char time_buf[64]={0};
            
            pair<Level,string>data=m_lockQueue.Pop();
            sprintf(time_buf,"%2d : %2d : %2d => [%s] ",tmnow->tm_hour,tmnow->tm_min,tmnow->tm_sec,(data.first==INFO ? "info":"error"));
            data.second.insert(0,time_buf);
            data.second.append("\n");

            fputs(data.second.c_str(),fp);
            fclose(fp);  
        }

    });
    writeLogTask.detach();
}

    