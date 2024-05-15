#pragma once
#include<unordered_map>
#include<string>
using namespace std;
//框架读取配置文件类
class MprpcConfig
{
public:
    void LoadConfigFile(const char*config_file);
    //查询配置项信息
    string Get(const string &key);
    void Trim(string &src_buf);
private:
    unordered_map<string,string>m_configMap;
};
