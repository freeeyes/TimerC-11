#include "pch.h"
#include "CTimerEvent.h"

void Timer_thread_run(CTimerThreadInfo* timer_thread_info)
{
    int ret      = -1;
    int timer_id = 0;

    if (nullptr == timer_thread_info)
    {
        return;
    }

    timer_thread_info->is_run_ = true;

    //执行定时器信息
    while (timer_thread_info->is_run_)
    {
        microseconds timer_interval(0);
        std::unique_lock <std::mutex> lock(timer_thread_info->thread_wait_mutex_);

        if (-1 == ret)
        {
            //等待下一次唤醒
            timer_thread_info->condition_.wait(lock);
            std::cout << "[Timer_thread_run]wait wake up" << " : " << timer_thread_info->thread_mutex_.try_lock() << endl;
        }
        else
        {
            //指定当前定时器
            bool timer_run = true;

            while (timer_run)
            {
                //执行定时器
                std::cout << "[Timer_thread_run]execute timer_id(" << timer_id << ")" << endl;

                //计算下次执行时间
                ret = timer_thread_info->timer_node_list_.get_next_run_timer_interval(timer_id, timer_interval);

                //如果在指定误差下，可以同时执行
                if (timer_interval.count() > 2000)
                {
                    break;
                }
            }

            //下一次执行时间唤醒
            std::cout << "[Timer_thread_run]wait_for(" << timer_interval.count() << ")" << endl;
            timer_thread_info->condition_.wait_for(lock, timer_interval);
            std::cout << "[Timer_thread_run]wait_for wake up" << endl;
        }

        //唤醒后判断应该做什么（增删改定时器）
        vector<CTimerEvents> timer_events_temp_list_;

        //timer_thread_info->thread_mutex_.lock();

        if (timer_thread_info->timer_events_list_.size() > 0)
        {
            timer_events_temp_list_.swap(timer_thread_info->timer_events_list_);
        }

        //timer_thread_info->thread_mutex_.unlock();

        for (auto f : timer_events_temp_list_)
        {
            if (em_insert_timer == f.em_timer_events_state_)
            {
                std::cout << "[Timer_thread_run]add_timer_id = " << f.timer_id_ << endl;
                timer_thread_info->timer_node_list_.add_timer_node_info(f.timer_id_,
                        f.timer_interval_);
            }
            else
            {
                timer_thread_info->timer_node_list_.del_timer_node_info(f.timer_id_);
            }
        }

        if (timer_events_temp_list_.size() > 0)
        {
            ret = timer_thread_info->timer_node_list_.get_next_run_timer_interval(timer_id, timer_interval);
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

    CTimerEvents timer_event;
    timer_event.em_timer_events_state_ = em_insert_timer;
    timer_event.timer_id_ = timer_id;
    timer_event.timer_interval_ = timer_interval;
    timer_thread_info_.timer_events_list_.push_back(timer_event);

    std::cout << "[add_timer]add_timer_id = " << timer_id << " : " << timer_thread_info_.thread_mutex_.try_lock() << endl;
    timer_thread_info_.condition_.notify_one();
    timer_thread_info_.thread_mutex_.unlock();

    return true;
}

bool CTimerEvent::del_timer(int timer_id)
{
    timer_thread_info_.thread_mutex_.lock();

    CTimerEvents timer_event;
    timer_event.em_timer_events_state_ = em_delete_timer;
    timer_event.timer_id_ = timer_id;
    timer_event.timer_interval_ = milliseconds(0);
    timer_thread_info_.timer_events_list_.push_back(timer_event);

    timer_thread_info_.condition_.notify_one();
    timer_thread_info_.thread_mutex_.unlock();

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

