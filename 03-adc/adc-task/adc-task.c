#include "hardware/adc.h"
#include "adc-task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"



const uint adc_pin = 26;
const uint adc_channel = 0;
const uint temp_channel = 4;
adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
uint64_t adc_ts;
uint ADC_TASK_MEAS_PERIOD_US = 100000;

void adc_task_init()
{
    adc_init();
    adc_gpio_init(adc_pin);
    adc_set_temp_sensor_enabled(true);
}
float adc_voltage_measure()
{
    adc_select_input(adc_channel);
    uint16_t voltage_counts = adc_read();
    float voltage_V = (float)voltage_counts / 4095.0f * 3.3f;
    return voltage_V;
}
float adc_temp_measure()
{
    adc_select_input(temp_channel);
    uint16_t temp_counts = adc_read();
    float temp_V = (float)temp_counts / 4095.0f * 3.3f;
    float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
    return temp_C;
}
void adc_task_set_state(adc_task_state_t state)
{
    adc_state = state;
}
void adc_task_handle()
{
    switch (adc_state)
    {
    case ADC_TASK_STATE_IDLE:
	    // ваш код
	    break;
    case ADC_TASK_STATE_RUN:
	    // ваш код
        if (time_us_64() > adc_ts)
        {
	        adc_ts = time_us_64() + (ADC_TASK_MEAS_PERIOD_US / 2);
	        // ваш код
            float voltage_V = adc_voltage_measure();
            float temp_C = adc_temp_measure();
            printf("%f %f\n", voltage_V, temp_C);
        }
	    break;
    default:
	    break;
    }
}