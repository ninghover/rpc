#include"mprpcconfig.h"
#include<iostream>
#include<cstring>

using namespace std;

void MprpcConfig::LoadConfigFile(const char*config_file)
{
    FILE*fp=fopen(config_file,"r");
    if(fp==nullptr)
    {
        cout<<"file not exit"<<endl;
        exit(EXIT_FAILURE);
    }
    while(!feof(fp))
    {
        char buf[128];
        fgets(buf,128,fp);
        string src_buf(buf);
        if(src_buf.size()>0&&src_buf[src_buf.size()-1]=='\n')
        {
            src_buf=src_buf.substr(0,src_buf.size()-1);
        }
        //1.去除#开头的注释  2.去除前后的空格 3. 按=分割
        Trim(src_buf);
        if(src_buf.empty()||src_buf[0]=='#')
            continue;
        
        int ind=src_buf.find('=');
        string key=src_buf.substr(0,ind);
        Trim(key);
        string value=src_buf.substr(ind+1,src_buf.size()-ind-1);
        Trim(value);
    
        m_configMap[key]=value;
    }
}
    //查询配置项信息
string MprpcConfig::Get(const string &key)
{
    auto it=m_configMap.find(key);
    if(it==m_configMap.end())
    {
        return "";
    }
    return m_configMap[key];
}

void MprpcConfig::Trim(string&src_buf)
{
    int ind=src_buf.find_first_not_of(' ');
    if(ind!=-1)
    {
        src_buf=src_buf.substr(ind,src_buf.size()-ind);
    }
    ind=src_buf.find_last_not_of(' ');

    if(ind!=-1)
    {
        src_buf=src_buf.substr(0,ind+1);
    }  
}