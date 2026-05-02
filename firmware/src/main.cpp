#include "config.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "power_status/power_status.h"
#include "sensors/mpu6050.h"
#include <array>
#include <cstdint>
#include <pico/float.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <../include/Eigen/Dense>
// #include "sensors/bmp280.h"
// #include "sensors/bn220.h"

// #if CYW43_USES_VSYS_PIN
#include "pico/cyw43_arch.h"
#endif

bool old_battery_status = false;   //
bool battery_status = true;        // Check
float old_voltage = -1;            //
const char *power_str = "UNKNOWN"; //

// Function that reads the battery voltage
void read_battery_voltage() {

  while (true) { // Get battery status
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
    sleep_ms(1000);
  }
}

int main() {
  stdio_init_all();

  // Initialise ADC hardware block
  adc_init();

  // Initialise and setup GPIOs for the LEDs
  gpio_init(GREEN_LED);
  gpio_init(RED_LED);
  gpio_init(BLUE_LED);

  gpio_set_dir(GREEN_LED, GPIO_OUT);
  gpio_set_dir(RED_LED, GPIO_OUT);
  gpio_set_dir(BLUE_LED, GPIO_OUT);

  // Turn on green LED for 2 seconds, then turn on the red LED
  gpio_put(GREEN_LED, 1);
  sleep_ms(2000);
  gpio_put(GREEN_LED, 0);
  gpio_put(RED_LED, 1);

// Pico W uses a CYW43 pin to get VBUS so we need to initialise it
#if CYW43_USES_VSYS_PIN
  if (cyw43_arch_init()) {
    printf("failed to initialise\n");
    return 1;
  }
#endif

  // Initialise MPU6050 imu
  MPU6050 imu;
  std::cout << "Calibrating the MPU6050 module...keep it level and still...\n";
  imu.get_offsets();
  // Turn off red LED after completing calibration
  gpio_put(RED_LED, 0);

  Vector3f accel, gyro;

  // Set output to 3 decimal places
  std::cout << std::setprecision(3) << std::fixed;

  while (1) {
    imu.update(gyro, accel);

    std::cout << "aX = " << accel(0) << "g aY = " << accel(1) << "g aZ = " << accel(2) << "g gX = " << gyro(0) << "dps gY = " << gyro(1) << "dps gZ = " << gyro(2) << "dps" << '\n';
    sleep_ms(100);
  }
  // Add some LED flashing to indicate when things are happening
  //
    // Read battery voltage
  // read_battery_voltage();

  //---------------------------------

#if CYW43_USES_VSYS_PIN
  cyw43_arch_deinit();
#endif
  return 0;
  // #endif
}
