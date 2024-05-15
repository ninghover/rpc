#pragma once
#include"mprpcconfig.h"

//mprpc框架的基础类
class MprpcApplication
{
public:
    static MprpcApplication*GetInstance();
    static void Init(int argc,char **argv);
    MprpcConfig &GetConfig();
private:

    static MprpcConfig m_mprpcConfig;
    MprpcApplication();
    MprpcApplication(const MprpcApplication&)=delete;
    MprpcApplication operator=(const MprpcApplication&)=delete;
};