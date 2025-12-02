# miniRTOS

A minimal cooperative RTOS implementation for STM32F103 (Blue Pill) microcontroller.

## Features

- Simple round-robin cooperative scheduler
- Task creation and management (up to 4 tasks)
- Task delay with 1ms tick resolution
- Lightweight implementation suitable for resource-constrained systems

## Hardware

- **Board:** STM32F103C8T6 (Blue Pill)
- **Clock:** 36 MHz (HSI with PLL)
- **LED:** PC13 (onboard LED)

## Project Structure

- `Core/Inc/minirtos.h` - RTOS API and definitions
- `Core/Src/minirtos.c` - RTOS scheduler implementation
- `Core/Src/main.c` - Main application with LED blink task

## Building

```bash
make -j16 DEBUG=1 -f STM32Make.make
```

## Flashing

```bash
make -j16 DEBUG=1 -f STM32Make.make flash
```

## Example Usage

```c
#include "minirtos.h"

void Task_BlinkLED(void)
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    RTOS_Delay(500);  /* Delay 500ms */
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    
    RTOS_Init();
    RTOS_CreateTask(Task_BlinkLED);
    RTOS_Start();  /* Never returns */
}
```

## License

STMicroelectronics software components are provided AS-IS.
