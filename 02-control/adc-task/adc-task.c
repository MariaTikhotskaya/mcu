#include "hardware/adc.h"
#include "adc-task.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"

const uint adc_pin = 26;
const uint adc_channel = 0;
void adc_task_init()
{
    adc_init();
    adc_gpio_init(adc_pin);
}
float adc_voltage_measure()
{
    adc_select_input(adc_channel);
    uint16_t voltage_counts = adc_read();
    float voltage_V = float(voltage_counts)/12*3.3
    return voltage_V;
}