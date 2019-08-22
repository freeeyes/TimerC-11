#include "pch.h"
#include "CTimerEvent.h"

void Timer_thread_run(CTimerThreadInfo* timer_thread_info)
{
    if (nullptr == timer_thread_info)
    {
        return;
    }

    timer_thread_info->is_run_ = true;

    //执行定时器信息
    while (timer_thread_info->is_run_)
    {
        microseconds timer_interval(0);
        int timer_id;
        std::unique_lock <std::mutex> lock(timer_thread_info->thread_wait_mutex_);
        int ret = timer_thread_info->timer_node_list_.get_next_run_timer_interval(timer_id, timer_interval);

        if (-1 == ret)
        {
            //等待下一次唤醒
            timer_thread_info->condition_.wait(lock);
            std::cout << "[Timer_thread_run]wait wake up" << endl;
            timer_thread_info->thread_mutex_.unlock();
        }
        else
        {
            //下一次执行时间唤醒
            std::cout << "[Timer_thread_run]wait_for(" << timer_interval.count() << ")" << endl;
            timer_thread_info->condition_.wait_for(lock, timer_interval);
            timer_thread_info->thread_mutex_.unlock();
            std::cout << "[Timer_thread_run]wait_for wake up" << endl;

            timer_thread_info->timer_event_.em_timer_events_state_ = em_execute_timer;
            timer_thread_info->timer_event_.timer_id_ = timer_id;
            timer_thread_info->timer_event_.timer_interval_ = milliseconds(0);

            std::cout << "[Timer_thread_run]timerid = " << timer_id << endl;
        }

        //唤醒后判断应该做什么（增删改定时器，还是执行执行的定时器）
        if (em_execute_timer == timer_thread_info->timer_event_.em_timer_events_state_)
        {
            //执行定时器
            std::cout << "[" << get_curr_time().c_str() << "]timerid = " << timer_thread_info->timer_event_.timer_id_ << endl;
        }
        else if (em_insert_timer == timer_thread_info->timer_event_.em_timer_events_state_)
        {
            std::cout << "[Timer_thread_run]add_timer_id = " << timer_thread_info->timer_event_.timer_id_ << endl;
            timer_thread_info->timer_node_list_.add_timer_node_info(timer_thread_info->timer_event_.timer_id_,
                    timer_thread_info->timer_event_.timer_interval_);
        }
        else
        {
            std::lock_guard<std::mutex> lock(timer_thread_info->thread_mutex_);
            timer_thread_info->timer_node_list_.del_timer_node_info(timer_thread_info->timer_event_.timer_id_);
        }
    }

    timer_thread_info->is_run_ = false;
}

CTimerEvent::CTimerEvent()
{
}


CTimerEvent::~CTimerEvent()
{
}

bool CTimerEvent::add_timer(int timer_id, milliseconds timer_interval)
{
    //如果线程没有启动，则启动定时器线程
    run();

    timer_thread_info_.thread_mutex_.lock();

    timer_thread_info_.timer_event_.em_timer_events_state_ = em_insert_timer;
    timer_thread_info_.timer_event_.timer_id_ = timer_id;
    timer_thread_info_.timer_event_.timer_interval_ = timer_interval;

    std::cout << "[add_timer]add_timer_id = " << timer_id << endl;
    timer_thread_info_.condition_.notify_one();

    return true;
}

bool CTimerEvent::del_timer(int timer_id)
{
    timer_thread_info_.thread_mutex_.lock();

    timer_thread_info_.timer_event_.em_timer_events_state_ = em_delete_timer;
    timer_thread_info_.timer_event_.timer_id_ = timer_id;
    timer_thread_info_.timer_event_.timer_interval_ = milliseconds(0);

    timer_thread_info_.condition_.notify_one();

    return true;
}

void CTimerEvent::run()
{
    if(timer_thread_info_.is_run_ == false)
    {
        timer_thread_info_.thread_ = std::thread(Timer_thread_run, &timer_thread_info_);
        this_thread::sleep_for(milliseconds(10));
    }
}

