#pragma once
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<iostream>

using namespace std;

enum Level
{
    INFO,   //普通信息
    ERROR   //错误信息
};

template<typename T>
class LockQueue
{
public:
    //多个worker线程都会把日志写到queue中
    void Push(pair<Level,const T&>data)
    {
        lock_guard<mutex>lock(m_mutex);
        m_queue.push(data);
        m_cond.notify_one();
    }
    //只有一个线程会把queue中的日志写到磁盘中
    pair<Level,T> Pop()
    {
        unique_lock<mutex>lock(m_mutex);
        while(m_queue.empty())
        {
            m_cond.wait(lock);
        }
        pair<Level,T> data=m_queue.front();
        m_queue.pop();
        return data;
    }
private:
    queue<pair<Level,T>>m_queue;
    mutex m_mutex;
    condition_variable m_cond;
};