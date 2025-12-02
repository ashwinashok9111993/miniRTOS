/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : minirtos.h
  * @brief          : Minimal RTOS Header
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __MINIRTOS_H__
#define __MINIRTOS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Configuration */
#define MAX_TASKS 4
#define STACK_SIZE 256

/* Task states */
typedef enum {
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_BLOCKED
} TaskState_t;

/* Task Control Block */
typedef struct {
    uint32_t *stack_ptr;           /* Stack pointer */
    uint32_t stack[STACK_SIZE];    /* Task stack */
    void (*task_func)(void);       /* Task function */
    TaskState_t state;             /* Task state */
    uint32_t delay_ticks;          /* Delay counter */
} TCB_t;

/* RTOS API */
void RTOS_Init(void);
void RTOS_CreateTask(void (*task_func)(void));
void RTOS_Start(void);
void RTOS_Delay(uint32_t ticks);
void RTOS_Yield(void);
void RTOS_TickHandler(void);

/* Scheduler functions */

#ifdef __cplusplus
}
#endif

#endif /* __MINIRTOS_H__ */
