#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
void start_task(void* parameter);
#define start_task_prio 1
#define start_task_stk_size 128

void ledtask1(void* parameter);
#define task1_prio 2
#define task1_stk_size 50

void ledtask2(void* parameter);
#define task2_prio 3
#define task2_stk_size 50

void LED_configuration();

#endif