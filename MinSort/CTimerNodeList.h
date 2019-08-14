#ifndef _TIMER_NODE_LIST_H
#define _TIMER_NODE_LIST_H

#include <iostream>
#include <chrono>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;
using namespace std::chrono;

//节点信息
class CTimerNodeInfo
{
public:
    CTimerNodeInfo() : timer_id_(0), next_time_(steady_clock::now()), begin_time_(steady_clock::now()), timer_interval_(0)
    {

    }

    void display_time(steady_clock::time_point& timer)
    {

    }

    void Step(steady_clock::time_point& timer_now)
    {
        if (next_time_ < timer_now)
        {
            //需要跳转计算到当前时间最后一个执行时间点
            milliseconds interval = duration_cast<milliseconds>(timer_now - begin_time_);

            milliseconds begin = interval % timer_interval_;

            next_time_ = timer_now + (timer_interval_ - begin);
        }
        else
        {
            next_time_ += timer_interval_;
        }
    }

    bool operator < (const CTimerNodeInfo& ar) const
    {
        //steady_clock::time_point timer_now = steady_clock::now();
        //milliseconds ar_interval = duration_cast<milliseconds>(ar.next_time_ - timer_now);
        //milliseconds this_interval = duration_cast<milliseconds>(this->next_time_ - timer_now);

        //std::cout << "this interval " << this_interlval.count() << "ms" << endl;
        return (this->next_time_ < ar.next_time_);
    }

public:
    int timer_id_;
    steady_clock::time_point next_time_;
    steady_clock::time_point begin_time_;
    milliseconds timer_interval_;
};

//镜像信息
class CRunNodeInfo
{
public:
    CRunNodeInfo() : timer_id_(0), curr_time_(steady_clock::now()), timer_interval_(0)
    {
    };

    int                      timer_id_;
    steady_clock::time_point curr_time_;
    milliseconds             timer_interval_;
};

class CTimerNodeList
{
public:
    CTimerNodeList();
    ~CTimerNodeList();

    void add_timer_node_info(int timer_id, milliseconds interval);

    void display();

    int lcm(int num1, int num2);
    int gcd(int num1, int num2);

public:
    vector<std::shared_ptr<CTimerNodeInfo>> vec_timer_node_list_;
    vector<CRunNodeInfo> vec_timer_run_list_;
    int lcm_data_;
};
#endif
