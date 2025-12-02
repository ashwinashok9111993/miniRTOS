/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : minirtos.c
  * @brief          : Minimal cooperative RTOS implementation
  ******************************************************************************
  */
/* USER CODE END Header */

#include "minirtos.h"
#include "stm32f1xx_hal.h"

/* Private variables */
static TCB_t tasks[MAX_TASKS];
static uint8_t task_count = 0;
static uint8_t current_task = 0;
static volatile uint32_t system_ticks = 0;

/**
  * @brief  Initialize the RTOS
  * @retval None
  */
void RTOS_Init(void)
{
    task_count = 0;
    current_task = 0;
    system_ticks = 0;
    
    /* Clear all task control blocks */
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].task_func = NULL;
        tasks[i].state = TASK_READY;
        tasks[i].delay_ticks = 0;
        tasks[i].stack_ptr = NULL;
    }
}

/**
  * @brief  Create a new task
  * @param  task_func: Pointer to task function
  * @retval None
  */
void RTOS_CreateTask(void (*task_func)(void))
{
    if (task_count >= MAX_TASKS) {
        return; /* Maximum tasks reached */
    }
    
    tasks[task_count].task_func = task_func;
    tasks[task_count].state = TASK_READY;
    tasks[task_count].delay_ticks = 0;
    task_count++;
}

/**
  * @brief  Delay task for specified ticks
  * @param  ticks: Number of system ticks to delay
  * @retval None
  */
void RTOS_Delay(uint32_t ticks)
{
    if (current_task < task_count) {
        tasks[current_task].delay_ticks = ticks;
        tasks[current_task].state = TASK_BLOCKED;
    }
}

/**
  * @brief  Yield CPU to next task
  * @retval None
  */
void RTOS_Yield(void)
{
    /* Will be handled by scheduler */
}

/**
  * @brief  Simple round-robin scheduler
  * @retval None
  */
static void RTOS_Scheduler(void)
{
    uint8_t next_task = current_task;
    uint8_t tasks_checked = 0;
    
    /* Find next ready task */
    while (tasks_checked < task_count) {
        next_task = (next_task + 1) % task_count;
        
        /* Check if task is ready */
        if (tasks[next_task].state == TASK_READY) {
            current_task = next_task;
            tasks[current_task].state = TASK_RUNNING;
            return;
        }
        
        tasks_checked++;
    }
    
    /* No ready tasks - idle (all blocked) */
    current_task = 0xFF; /* Invalid task index indicates idle */
}

/**
  * @brief  RTOS tick handler - should be called from SysTick_Handler
  * @retval None
  */
void RTOS_TickHandler(void)
{
    system_ticks++;
    
    /* Decrement delay counters */
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].state == TASK_BLOCKED && tasks[i].delay_ticks > 0) {
            tasks[i].delay_ticks--;
            if (tasks[i].delay_ticks == 0) {
                tasks[i].state = TASK_READY;
            }
        }
    }
}

/**
  * @brief  Start the RTOS scheduler
  * @retval None (never returns)
  */
void RTOS_Start(void)
{
    if (task_count == 0) {
        return; /* No tasks to run */
    }
    
    /* Start with first task */
    current_task = 0xFF; /* Start with invalid to force scheduler to pick first task */
    
    /* Infinite scheduler loop */
    while (1) {
        /* Run scheduler to select next task */
        RTOS_Scheduler();
        
        /* Execute current task if valid and ready/running */
        if (current_task < task_count && 
            tasks[current_task].state == TASK_RUNNING) {
            
            /* Call task function */
            if (tasks[current_task].task_func != NULL) {
                tasks[current_task].task_func();
            }
            
            /* After task runs, set it back to ready (if not blocked by delay) */
            if (tasks[current_task].state == TASK_RUNNING) {
                tasks[current_task].state = TASK_READY;
            }
        }
    }
}
