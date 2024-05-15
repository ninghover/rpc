#include"mprpcapplication.h"
#include<iostream>
#include<getopt.h>
#include<unistd.h>

using namespace std;

MprpcConfig MprpcApplication::m_mprpcConfig;

void showArgsHelp()
{
    cout<<"format: command -i <config_file>"<<endl;
}

void MprpcApplication::Init(int argc,char**argv)
{
    if(argc<2)
    {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }
    int opt=0;
    string config_file;
    while((opt=getopt(argc,argv,"i:"))!=-1)
    {
        switch(opt)
        {
            case 'i':
            {
                config_file=optarg;
                break;
            }
            case '?':
            {
                cout<<"invalid args!"<<endl;
                showArgsHelp();
                exit(EXIT_FAILURE);
            }
            default:
            break;
        }
    }

    //加载配置文件
    m_mprpcConfig.LoadConfigFile(config_file.c_str());
    
}

MprpcConfig &MprpcApplication::GetConfig()
{
    return m_mprpcConfig;
}

MprpcApplication::MprpcApplication(){}

MprpcApplication*MprpcApplication::GetInstance()
{
    static MprpcApplication instance;
    return&instance;
}