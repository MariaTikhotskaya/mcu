#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "adc-task/adc-task.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"
// const uint LED_PIN = 25;


void version_callback(const char* args);
void led_on_callback(const char* args);
void led_off_callback(const char* args);
void led_blink_callback(const char* args);
void led_blink_set_period_ms_callback(const char* args);
void help_callback(const char* args);
void adc_callback(const char* args);
void temp_callback(const char* args);
void tm_start_callback(const char* args);
void tm_stop_callback(const char* args);


api_t device_api[] =
{
	{"version", version_callback, "get device name and firmware version"},
    {"on", led_on_callback, "on"},
    {"off", led_off_callback, "off"},
    {"blink", led_blink_callback, "blink"},
    {"set_period", led_blink_set_period_ms_callback, "set_period"},
    {"help", help_callback, "help"},
    {"get_adc", adc_callback, "get_adc"},
    {"get_temp", temp_callback, "get_temp"},
    {"tm_start", tm_start_callback, "tm_start"},
    {"tm_stop", tm_stop_callback, "tm_stop"},
	{NULL, NULL, NULL},
};

void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char* args)
{
    led_task_state_set(LED_STATE_ON);
}

void led_off_callback(const char* args)
{
    led_task_state_set(LED_STATE_OFF);
}  

void led_blink_callback(const char* args)
{
    led_task_state_set(LED_STATE_BLINK);
}

void led_blink_set_period_ms_callback(const char* args)
{
    uint period_ms = 0;
    sscanf(args, "%u", &period_ms);
    if (period_ms <= 0)
    {
        printf("invalid period");
        return;
    }
    led_task_set_blink_period_ms(period_ms);
}

void help_callback(const char* args)
{
    for (int i=0; device_api[i].command_name != NULL; i++)
    {
        printf("%s - %s\n", device_api[i].command_name, device_api[i].command_help);
    }
}
void adc_callback(const char* args)
{
    float voltage_V = adc_voltage_measure();
    printf("%f\n", voltage_V);
}
void temp_callback(const char* args)
{
    float temp_C = adc_temp_measure();
    printf("%f\n", temp_C);
}

void tm_start_callback(const char* args)
{
    adc_task_set_state(ADC_TASK_STATE_RUN);
}

void tm_stop_callback(const char* args)
{
    adc_task_set_state(ADC_TASK_STATE_IDLE);
}

int main()
{
    led_task_init();
    stdio_init_all();
    stdio_task_init();
    adc_task_init();
    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
    protocol_task_init(device_api);

    while(1)
    {
        led_task_handle();
        adc_task_handle();
        protocol_task_handle(stdio_task_handle());
    }
}