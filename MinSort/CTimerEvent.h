#pragma once

#include "CTimerNodeList.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <time.h>
#include "date.h"

enum em_timer_events_state
{
    em_execute_timer = 0,     //ִ�ж�ʱ���¼�
    em_insert_timer,          //��Ӷ�ʱ��
    em_delete_timer           //ɾ����ʱ��
};

inline std::string get_curr_time()
{
    string strDate;

    auto  tt_now = system_clock::now();

    //�õ�����ֵ
    typedef duration<int, ratio_multiply<hours::period, ratio<24> >::type> days;
    system_clock::duration tp = tt_now.time_since_epoch();
    auto dp = date::floor<date::days>(tp);

    auto time = date::make_time(duration_cast<milliseconds>(tp - dp));

    auto tt = std::chrono::system_clock::to_time_t
              (tt_now);
    struct tm* ptm = localtime(&tt);
    char date[60] = { 0 };
    sprintf(date, "%04d-%02d-%02d-%02d.%02d.%02d:%d",
            (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
            (int)time.hours().count(), (int)time.minutes().count(), (int)time.seconds().count(), (int)time.subseconds().count());

    strDate = date;
    return strDate;
}

//timerִ���¼�
class CTimerEvents
{
public:
    CTimerEvents() : em_timer_events_state_(em_execute_timer), timer_id_(0), timer_interval_(0)
    {

    }

    em_timer_events_state em_timer_events_state_;
    int timer_id_;
    milliseconds timer_interval_;
};

class CTimerThreadInfo
{
public:
    CTimerThreadInfo() : is_run_(false) {};

    CTimerNodeList timer_node_list_;
    CTimerEvents timer_event_;
    bool is_run_;
    std::thread thread_;
    mutable std::mutex thread_wait_mutex_;
    mutable std::mutex thread_mutex_;
    std::condition_variable condition_;
};

class CTimerEvent
{
public:
    CTimerEvent();
    ~CTimerEvent();

    bool add_timer(int timer_id, milliseconds timer_interval);

    bool del_timer(int timer_id);

private:
    void run();

private:
    CTimerThreadInfo timer_thread_info_;
};

