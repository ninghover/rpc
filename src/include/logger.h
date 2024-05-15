#include"lockqueue.h"
#include<iostream>
#include<string>
#include<queue>

using namespace std;

class Logger
{
public:
    static Logger& GetInstance();
    void Log(Level,string);
private:
    Logger();
    Logger(const Logger&)=delete;
    Logger&operator=(const Logger&)=delete;
    LockQueue<string>m_lockQueue;
    
};

#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(INFO,c); \
    } while(0) \

#define LOG_ERR(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(ERROR,c); \
    } while(0) \

