// MinSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "CTimerNodeList.h"

int main()
{
    CTimerNodeList timer_node_list;

    CTimerNodeInfo timer_node_info1;
    CTimerNodeInfo timer_node_info2;
    CTimerNodeInfo timer_node_info3;

    int timer_id_1                = 1;
    milliseconds timer_interval_1 = milliseconds(500);

    int timer_id_2                = 2;
    milliseconds timer_interval_2 = milliseconds(700);

    int timer_id_3                = 3;
    milliseconds timer_interval_3 = milliseconds(1000);

    timer_node_list.add_timer_node_info(timer_id_1, timer_interval_1);
    timer_node_list.add_timer_node_info(timer_id_2, timer_interval_2);
    timer_node_list.add_timer_node_info(timer_id_3, timer_interval_3);

    steady_clock::time_point timer_now = steady_clock::now();
    timer_node_list.get_run_list(timer_now);
    timer_node_list.display();

    /*
    CTimerNodeList timer_node_list;
    steady_clock::time_point timer_now = steady_clock::now();

    vector<std::shared_ptr<CTimerNodeInfo>> vec_timer_node_list_;

    std::shared_ptr<CTimerNodeInfo> timer_node_info1 = std::make_shared<CTimerNodeInfo>();
    std::shared_ptr<CTimerNodeInfo> timer_node_info2 = std::make_shared<CTimerNodeInfo>();

    timer_node_info1->timer_id_       = 1;
    timer_node_info1->timer_interval_ = milliseconds(1000);

    timer_node_info2->timer_id_       = 2;
    timer_node_info2->timer_interval_ = milliseconds(300);

    vec_timer_node_list_.push_back(timer_node_info1);
    vec_timer_node_list_.push_back(timer_node_info2);

    //计算所有数据下次执行时间
    int lcm_data_ = 0;
    int nSize     = vec_timer_node_list_.size();

    for (int i = 0; i < nSize; i++)
    {
        if (i == 0)
        {
            lcm_data_ = (int)vec_timer_node_list_[i]->timer_interval_.count();
        }
        else
        {
            lcm_data_ = timer_node_list.lcm((int)vec_timer_node_list_[i]->timer_interval_.count(), lcm_data_);
        }

        vec_timer_node_list_[i]->Step(timer_now);
    }

    std::sort(vec_timer_node_list_.begin(), vec_timer_node_list_.end(), [](std::shared_ptr<CTimerNodeInfo> a, std::shared_ptr<CTimerNodeInfo> b)
    {
        return *a < *b;
    });

    std::cout << "**********************" << endl;

    for (auto f : vec_timer_node_list_)
    {
        std::cout << "Timer ID " << f->timer_id_ << endl;
    }

    std::cout << "**********************" << endl;

    while (vec_timer_node_list_[0]->next_time_ < timer_now + milliseconds(lcm_data_))
    {
        std::cout << "Timer ID " << vec_timer_node_list_[0]->timer_id_ << endl;

        vec_timer_node_list_[0]->Step(timer_now);
        std::sort(vec_timer_node_list_.begin(), vec_timer_node_list_.end(), [](std::shared_ptr<CTimerNodeInfo> a, std::shared_ptr<CTimerNodeInfo> b)
        {
            return *a < *b;
        });
    }
    */

    getchar();
    return 0;

}