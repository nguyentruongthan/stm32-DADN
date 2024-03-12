/*
 * scheduler.c
 *
 *  Created on: Mar 12, 2024
 *      Author: truong_than
 */

#include "scheduler.h"


#define SCH_MAX_TASKS		10

struct s_task{
	// Pointer to the task (must be a ’ void ( void ) ’ function )
	void (*pTask)(void);

	// Delay (ticks) until the function will ( next ) be run
	uint32_t delay ;

	// Interval (ticks) between subsequent runs .
	uint32_t period ;
	// Incremented (by scheduler) when task i s due to execute
	uint8_t run_me;
	// this is not only ID of task but also position of this task in SCH_tasks_G
	uint32_t task_ID ;
};
struct s_task SCH_tasks_G[SCH_MAX_TASKS];

void SCH_Init(void){
	for(uint8_t i = 0; i < SCH_MAX_TASKS; i++){
		//initial all element in SCH_tasks_G is not filled
		SCH_tasks_G[i].pTask = 0;
	}
}

//This function will be updated the remaining time of each
//tasks that are added to a queue. It will be called in the interrupt timer,
//for example 10 ms.
void SCH_Update(void){
	for(uint8_t i = 0; i < SCH_MAX_TASKS; i++){
		//position i in SCH_tasks_G is empty
		if(SCH_tasks_G[i].pTask == 0) continue;

		//Update delay for task
		if(SCH_tasks_G[i].delay > 0){
			SCH_tasks_G[i].delay --;

			//If delay is equal to 0,
			//we update task's run_me for it run
			//and assign period for task's delay
			if(SCH_tasks_G[i].delay == 0){
				SCH_tasks_G[i].run_me = 1;
				if(SCH_tasks_G[i].period)
					SCH_tasks_G[i].delay = SCH_tasks_G[i].period;
			}
		}
	}
}

//This function will get the task in the queue to run
void SCH_Dispatch_Tasks(void){
	for(uint8_t i = 0; i < SCH_MAX_TASKS; i++){
		if(SCH_tasks_G[i].run_me){
			//print counter of timer
//			char buffer[25];
//			int size = sprintf(buffer, "Task %d:%ld\r\n", i, count_timer);
//			HAL_UART_Transmit(&huart2, (uint8_t*)buffer, (uint16_t)size, 100);

			//Run task
			(*SCH_tasks_G[i].pTask)();
			//update run_me for inform that task was run
			SCH_tasks_G[i].run_me = 0;
			//delete task if task is one shoot task
			if(SCH_tasks_G[i].period == 0){
				SCH_Delete_Task(i);
			}
		}
	}
}

//This function is used to add a task to the queue.
//It should return an ID that is corresponding with the added task
uint32_t SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	for(uint8_t i = 0; i < SCH_MAX_TASKS; i++){
		if(SCH_tasks_G[i].pTask == 0){
			//TODO
			SCH_tasks_G[i].pTask = pFunction;
			SCH_tasks_G[i].delay = DELAY;
			SCH_tasks_G[i].period = PERIOD;
			SCH_tasks_G[i].task_ID = i;
			return i;
		}
	}
	return SCH_MAX_TASKS;
}

//This function is used to delete the task based on its ID.
uint8_t SCH_Delete_Task(uint32_t taskID){
	if(taskID >= SCH_MAX_TASKS) return 0; //ERROR
	if(SCH_tasks_G[taskID].pTask == 0) return 0; //ERROR

	SCH_tasks_G[taskID].pTask = 0x0000;
	SCH_tasks_G[taskID].delay = 0;
	SCH_tasks_G[taskID].period = 0;
	SCH_tasks_G[taskID].run_me = 0;

	return 1;
}
