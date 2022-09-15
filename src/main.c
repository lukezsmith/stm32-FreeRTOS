#include <stdint.h>
#include "FreeRTOS.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "task.h"

int main(void);
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
        // GPIOD->ODR ^= (1U << 12);  
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15); // Toggle LEDs
    }
}

void prvInitGPIO( void )
{
	GPIO_InitTypeDef GPIOD_Params; // Initilisation structure for GPIOD Settings
	
	__HAL_RCC_GPIOD_CLK_ENABLE(); // Turn on Clock of GPIOD
	
	// Configure the GPIO Pins 12, 13, 14 and 15 used for LEDs
	GPIOD_Params.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; // Select pins 12 to 15
	GPIOD_Params.Mode = GPIO_MODE_OUTPUT_PP; // Set pins to push pull output mode
	GPIOD_Params.Speed = GPIO_SPEED_LOW; // Set low output speed
	HAL_GPIO_Init(GPIOD, &GPIOD_Params); // Initialise GPIOD according to parameters on GPIOD_Params
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
