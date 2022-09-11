#include <stdint.h>
#include "FreeRTOS.h"
#include "stm32f407xx.h"
#include "task.h"


// LED delay duration
#define LEDDELAY 500

int main(void);
void delay(volatile uint32_t);
static void prvSetupHardware( void );
static void prvInitGPIO( void );
void ledBlinkTask ( void *p );

TaskHandle_t ledBlinkTaskHandle = NULL;

int main(void)
{
    // Setup hardware 
    prvSetupHardware();

    // create tasks
    xTaskCreate(ledBlinkTask, "LED", configMINIMAL_STACK_SIZE, (void *) NULL, tskIDLE_PRIORITY, &ledBlinkTaskHandle);

    // start scheduler
    vTaskStartScheduler();

    for ( ;; );
}

void ledBlinkTask ( void *p )
{
    TickType_t xLastWakeTime;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
        GPIOD->ODR ^= (1U << 12);  
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    }
}

void prvInitGPIO( void )
{
    // Enable GPIOD clock (AHB1ENR: bit 3)
    RCC->AHB1ENR |= (1U << 3);

    // reset bits 25 and 24 (pin 12)
    GPIOD->MODER &= ~(1U << 24);
    // set 25 and 24 bits
    GPIOD->MODER |=  (1U << 24);
    // Set pin 12 to turn on LED
    GPIOD->ODR |= (1U << 12);
}

void prvSetupHardware( void )
{
    // setup STM32 clock, PLL and Flash
    SystemInit();

    // setup GPIO outputs
    prvInitGPIO();
}


void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
