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
    milliseconds timer_interval_2 = milliseconds(1000);

    int timer_id_3                = 3;
    milliseconds timer_interval_3 = milliseconds(1500);

    timer_node_list.add_timer_node_info(timer_id_1, timer_interval_1);
    timer_node_list.add_timer_node_info(timer_id_2, timer_interval_2);
    timer_node_list.add_timer_node_info(timer_id_3, timer_interval_3);

    timer_node_list.display();

    getchar();
    return 0;

}