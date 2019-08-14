#include "CTimerNodeList.h"

struct
{
    bool operator()(std::shared_ptr<CTimerNodeInfo> a, std::shared_ptr<CTimerNodeInfo> b) const
    {
        return *a < *b;
    }
} customLess;

CTimerNodeList::CTimerNodeList() : lcm_data_(0)
{
}


CTimerNodeList::~CTimerNodeList()
{
}

void CTimerNodeList::add_timer_node_info(int timer_id, milliseconds interval)
{
    steady_clock::time_point timer_now = steady_clock::now();
    std::shared_ptr<CTimerNodeInfo> timer_node_info = std::make_shared<CTimerNodeInfo>();

    timer_node_info->timer_id_       = timer_id;
    timer_node_info->timer_interval_ = interval;
    timer_node_info->Step(timer_now);

    vec_timer_node_list_.push_back(timer_node_info);

    //get_run_list(timer_now);
}

void CTimerNodeList::display()
{
    int nSize = (int)vec_timer_run_list_.size();
    std::cout << "timer count " << nSize << endl;

    //显示最小公倍数
    std::cout << "lcm is " << lcm_data_ << endl;

    for (auto f : vec_timer_run_list_)
    {
        std::cout << "timer id (" << f.timer_id_  << ") interval=" << f.timer_interval_.count() << endl;
    }
}

void CTimerNodeList::get_run_list(steady_clock::time_point& timer_now)
{
    int nSize = (int)vec_timer_node_list_.size();

    //计算所有数据下次执行时间
    lcm_data_ = 0;

    for (int i = 0; i < nSize; i++)
    {
        if (i == 0)
        {
            lcm_data_ = (int)vec_timer_node_list_[i]->timer_interval_.count();
        }
        else
        {
            lcm_data_ = lcm((int)vec_timer_node_list_[i]->timer_interval_.count(), lcm_data_);
        }
    }

    //开始生成数据循环集合
    if (nSize > 1)
    {
        vec_timer_run_list_.clear();

        std::sort(vec_timer_node_list_.begin(), vec_timer_node_list_.end(), customLess);

        timer_now = vec_timer_node_list_[0]->next_time_;

        while (vec_timer_node_list_[0]->next_time_ < timer_now + milliseconds(lcm_data_))
        {
            //std::cout << "Timer ID " << vec_timer_node_list_[0]->timer_id_ << endl;
            CRunNodeInfo run_node_info;
            run_node_info.timer_id_ = vec_timer_node_list_[0]->timer_id_;
            run_node_info.curr_time_ = vec_timer_node_list_[0]->next_time_;

            vec_timer_node_list_[0]->Step(timer_now);
            std::sort(vec_timer_node_list_.begin(), vec_timer_node_list_.end(), customLess);

            run_node_info.timer_interval_ = duration_cast<milliseconds>(vec_timer_node_list_[0]->next_time_ - run_node_info.curr_time_);
            vec_timer_run_list_.push_back(run_node_info);
        }

        //std::cout << "Timer Count " << vec_timer_run_list_.size() << endl;
    }
    else if (nSize == 1)
    {
        CRunNodeInfo run_node_info;
        run_node_info.timer_id_ = vec_timer_node_list_[0]->timer_id_;
        run_node_info.timer_interval_ = vec_timer_node_list_[0]->timer_interval_;
        vec_timer_run_list_.push_back(run_node_info);
    }
}

int CTimerNodeList::lcm(int num1, int num2)
{
    return num1 / gcd(num1, num2) * num2;
}

int CTimerNodeList::gcd(int num1, int num2)
{
    int nRet = 0;

    while (num1 != num2)
    {
        if (num1 > num2)
        {
            num1 = num1 - num2;
        }
        else
        {
            num2 = num2 - num1;
        }
    }

    return num1;
}
