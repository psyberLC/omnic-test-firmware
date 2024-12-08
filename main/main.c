#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_timer.h"

// Stack size in kilobytes allocated for each task
#define STACK_SIZE 4096

// Your LED pin - default GPIO2 for ESP32 DEVKIT V1
#define LED_PIN GPIO_NUM_2

// If LED is connected to pin via the pull-up resitor, 
// the ON state is low (0), otherwise the ON state is high (1)
#define LED_ON 1

// If LED is connected to pin via the pull-up resitor, 
// the OFF state is high (1), otherwise the OFF state is low (0)
#define LED_OFF 0

// Handle of the counter queue
QueueHandle_t counterQueue;

// This task increments the counter and sends its value into the queue every 5 seconds
void queueSendTask( void * args )
{
    uint32_t counter = 0;

    while ( 1 )
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        counter++;
        if( xQueueSend( counterQueue, ( void * ) &counter, ( TickType_t ) 10 ) != pdPASS )
        {
            // Failed to post the message even after 10 ticks
            ESP_LOGE( "QueueSendTask", "Failed to send incremented value after 10 ticks" );
        }
    }
}

// This task waits for other task to send incremented value to the
// queue and logs it with the time it took to receive the value
void queueReceiveTask( void * args )
{
    uint32_t receivedValue;

    while ( 1 )
    {
        int64_t taskStartTime = esp_timer_get_time();
        if( xQueueReceive( counterQueue, &receivedValue, portMAX_DELAY ) == pdPASS )
        {
            // Time between receiving the incremented values from queue
            int64_t taskCompletionTime = ( esp_timer_get_time() - taskStartTime  ) / 1000;
            // Logging
            ESP_LOGI( "QueueReceiveTask", "Incremented value received: %lu", receivedValue);
            ESP_LOGI( "QueueReceiveTask", "Time took to receive the value: %llu\ms", taskCompletionTime);
        }
        else
        {
            // Failed to receive the message
            ESP_LOGE( "QueueReceiveTask", "Failed to receive incremented value" );
        }
    }
}

// This task blinks LED for 100 ms every 1 second
void blinkTask( void * args )
{
    // Resetting the LED pin just in case
    gpio_reset_pin( LED_PIN );
    // Setting the LED pin to output mode
    gpio_set_direction( LED_PIN, GPIO_MODE_OUTPUT );

    while ( 1 )
    {
        // Enabling LED for 100 ms
        gpio_set_level( LED_PIN, LED_ON );
        vTaskDelay( 100 / portTICK_PERIOD_MS );
        // Disabling LED for 900 ms
        gpio_set_level( LED_PIN, LED_OFF );
        vTaskDelay( 900 / portTICK_PERIOD_MS );
    }
}

void app_main( void ) {
    // Task handles initialization
    TaskHandle_t queueSendTaskHandle = NULL;
    TaskHandle_t queueReceiveTaskHandle = NULL;
    TaskHandle_t blinkTaskHandle = NULL;

    // 1-element queue creation
    counterQueue = xQueueCreate( 1, sizeof( uint32_t * ) );

    // Active tasks creation (priority must be greater than tskIDLE_PRIORITY)
    xTaskCreate( queueSendTask, "QueueSendTask", STACK_SIZE, NULL, 1, &queueSendTaskHandle );
    configASSERT( queueSendTaskHandle );

    xTaskCreate( queueReceiveTask, "QueueReceiveTask", STACK_SIZE, NULL, 1, &queueReceiveTaskHandle );
    configASSERT( queueReceiveTaskHandle );

    xTaskCreate( blinkTask, "BlinkTask", STACK_SIZE, NULL, 1, &blinkTaskHandle );
    configASSERT( blinkTaskHandle );
}