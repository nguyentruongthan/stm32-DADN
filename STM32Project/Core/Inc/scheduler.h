/*
 * scheduler.c
 *
 *  Created on: Mar 12, 2024
 *      Author: truong_than
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_
#include "global.h"

void SCH_Init(void);

//This function will be updated the remaining time of each
//tasks that are added to a queue. It will be called in the interrupt timer,
//for example 10 ms.
void SCH_Update(void);

//This function will get the task in the queue to run
void SCH_Dispatch_Tasks(void);
//This function is used to add a task to the queue.
//It should return an ID that is corresponding with the added task
uint32_t SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD);

//This function is used to delete the task based on its ID.
uint8_t SCH_Delete_Task(uint32_t taskID);


#endif /* INC_SCHEDULER_H_ */
