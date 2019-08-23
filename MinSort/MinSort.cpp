// MinSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CTimerEvent.h"

int main()
{
    /*
    CTimerNodeList timer_node_list;

    CTimerNodeInfo timer_node_info1;
    CTimerNodeInfo timer_node_info2;
    CTimerNodeInfo timer_node_info3;

    int timer_id_1                = 1;
    milliseconds timer_interval_1 = milliseconds(500);

    int timer_id_2                = 2;
    milliseconds timer_interval_2 = milliseconds(1000);

    int timer_id_3                = 3;
    milliseconds timer_interval_3 = milliseconds(1500);

    timer_node_list.add_timer_node_info(timer_id_1, timer_interval_1);
    timer_node_list.add_timer_node_info(timer_id_2, timer_interval_2);
    //timer_node_list.add_timer_node_info(timer_id_3, timer_interval_3);

    timer_node_list.display();


    std::cout << "[" << get_curr_time().c_str() << "] Begin" << endl;
    this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "[" << get_curr_time().c_str() << "] End" << endl;

    std::cout << "#####################" << endl;
    long long b = system_clock::now().time_since_epoch().count();
    //this_thread::sleep_for(std::chrono::milliseconds(100));
    this_thread::sleep_for(std::chrono::microseconds(100 * 1000));
    long long e = system_clock::now().time_since_epoch().count();

    std::cout << "[" << b << "] Begin" << endl;
    std::cout << "[" << e << "] end" << endl;

    long long in = e - b;
    std::cout << "[" << in << "] interval" << endl;
    */

    CTimerManager timer_events;

    timer_events.add_timer(1, milliseconds(1000));
    //timer_events.add_timer(2, milliseconds(2000));

    getchar();
    return 0;

}