#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "power_status/power_status.h"
#include <FreeRTOS.h>
#include <pico/float.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <task.h>

// Pico W uses a CYW43 pin to get VBUS so we need to initialise it
#if CYW43_USES_VSYS_PIN
#include "pico/cyw43_arch.h"
#endif

// extern "C" {
//   #include "power_status/power_status.h"
// }

// Initialize constants/GPIO pins
#define ABC abc
#define ABC abc

bool old_battery_status = false;
bool battery_status = true;
float old_voltage = -1;
char *power_str = "UNKNOWN";

// Might change name
// Initialize led, comms etc
void setup_task(void *pvParameters) {

  // Initialize and setup GPIOs for the LEDs
  const uint green_led = 7;
  const uint red_led = 8;
  const uint blue_led = 9;

  gpio_init(green_led);
  gpio_init(red_led);
  gpio_init(blue_led);

  gpio_set_dir(green_led, GPIO_OUT);
  gpio_set_dir(red_led, GPIO_OUT);
  gpio_set_dir(blue_led, GPIO_OUT);

  // Turn on red led for 4 seconds, then turn on the green LED
  gpio_put(red_led, 1);
  vTaskDelay(pdMS_TO_TICKS(4000)); // wait 4 seconds
  gpio_put(red_led, 0);
  gpio_put(green_led, 1);

}

// Read battery voltage task
// should this be run in an infinite loop?
void read_batt_volt_task(void *pvParameters) {

  // bool old_battery_status = false;
  // bool battery_status = true;
  // float old_voltage = -1;
  // char *power_str = "UNKNOWN";

  while (true) {
    // Get battery status
    if (power_source(&battery_status) == PICO_OK) {
      power_str = battery_status ? "BATTERY" : "POWERED";
    }

    // Get voltage
    float voltage = 0;
    int voltage_return = power_voltage(&voltage);
    voltage = floorf(voltage * 100) / 100;

    // Display power if it's changed
    if (old_battery_status != battery_status || old_voltage != voltage) {
      char percent_buf[10] = {0};
      if (battery_status && voltage_return == PICO_OK) {
        const float min_battery_volts = 3.0f;
        const float max_battery_volts = 4.2f;
        int percent_val = (int)(((voltage - min_battery_volts) /
                                 (max_battery_volts - min_battery_volts)) *
                                100);
        snprintf(percent_buf, sizeof(percent_buf), " (%d%%)", percent_val);
      }

      // Display power and remember old vales
      printf("Power %s, %.2fV%s \n", power_str, voltage, percent_buf);
      old_battery_status = battery_status;
      old_voltage = voltage;
    }
    // sleep_ms(1000);
    vTaskDelay(dMS_TO_TICKS(1000)); // wait for a second

  }
}

int main() {
  stdio_init_all();

  adc_init(); // Initialise ADC HW

  // Pico W uses a CYW43 pin to get VBUS so we need to initialise it
  #if CYW43_USES_VSYS_PIN
  if (cyw43_arch_init()) {
      printf("failed to initialise\n");
      return 1;
  }
  #endif

  xTaskCreate(setup_task, "Setup_Task", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY, NULL);
  xTaskCreate(read_batt_volt_task, "Read_Batt_Volt_Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  // Start the scheduler
  vTaskStartScheduler();

  while (1) {
  };

#if CYW43_USES_VSYS_PIN
  cyw43_arch_deinit();
#endif
  return 0;
}
