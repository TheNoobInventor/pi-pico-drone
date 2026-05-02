#include "mpu6050.h"
#include <stdio.h>

// Anonymous namespace
namespace {
  // Write data to the device register
  void write_to_register(uint8_t reg, uint8_t value) {
    // Write one byte to the register, reg, and the second byte to the data,
    // value
    uint8_t buf[] = {reg, value};
    i2c_write_blocking(I2C_PORT, MPU_ADDR, buf, 2,
                       false); // False - finished with bus
  }

  // Read data from the device register
  void read_from_register(uint8_t reg, uint8_t *buf, size_t length) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto
    // incrementing so we don't need to keep sending the register we want, just
    // the first incorporate buffer length in here

    i2c_write_blocking(I2C_PORT, MPU_ADDR, &reg, 1,
                       true); // True - to keep master control of bus
    i2c_read_blocking(I2C_PORT, MPU_ADDR, buf, length, false);
  }
}

MPU6050::MPU6050() {

  // Initialise i2c hardware block
  i2c_init(I2C_PORT, 400 * 1000); // Max bus speed 400 kHz
  gpio_set_function(MPU_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(MPU_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(MPU_SDA_PIN);
  gpio_pull_up(MPU_SCL_PIN);

  // Reset the device then wake it up
  write_to_register(REG_PWR_MGMT_1, 0x80);
  sleep_ms(200); // Allow device to reset and stabilize

  write_to_register(REG_PWR_MGMT_1, 0x00);
  sleep_ms(200); // Allow stabilization after waking up

  // Configure gyroscope and accelerometer settings
  write_to_register(REG_GYRO_CONFIG, GYRO_CONFIG_VALUE);
  write_to_register(REG_ACCEL_CONFIG, ACCEL_CONFIG_VALUE);

  // Set DLPF (low pass filter) to 10Hz which corresponds to the value 0x05
  // (refer to datasheet), no noise above 10Hz will pass through
  write_to_register(REG_CONFIG, 0x05);

  // Set sample rate divider to 200Hz
  write_to_register(REG_SMPLRT_DIV, SAMPLE_RATE_DIV);

  // Check device connection
  uint8_t who_am_i = 0;
  read_from_register(WHO_AM_I_REG, &who_am_i, 1);

  // TODO: convert to std::cout
  printf("MPU6050 WHO_AM_I: 0x%02X\n", who_am_i);

  if (who_am_i != 0x68) {
    printf("MPU6050 not found!\n");
    while (1) {
    }
  }
}

// Get raw gyroscope data
void MPU6050::get_gyro_raw() {
  uint8_t buffer[6];

  // Read gyroscope data for 6 bytes
  read_from_register(REG_GYRO_XOUT_H, buffer, 6);

  for (int i = 0; i < 3; i++) {
    gyro_raw[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
  }
}

// Get gyroscope data after applying sensitivity settings and removing
// offsets (calibration)
void MPU6050::get_gyro(Vector3f &gyro) {
  get_gyro_raw();

  for (int i = 0; i < 3; i++) {
    gyro(i) = (gyro_raw[i] - gyro_offsets(i)) / GYRO_SCALE_FACTOR;
  }
}

// Get raw accelerometer data
void MPU6050::get_accel_raw() {
  uint8_t buffer[6];

  // Read accelerometer data for 6 bytes
  read_from_register(REG_ACCEL_XOUT_H, buffer, 6);

  for (size_t i = 0; i < 3; i++) {
    accel_raw[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
  }
}

// Get accelerometer data after applying sensitivity settings and removing
// offsets (calibration)
void MPU6050::get_accel(Vector3f &accel) {
  get_accel_raw();

  for (size_t i = 0; i < 3; i++) {
      accel(i) = (accel_raw[i] - accel_offsets(i)) / ACCEL_SCALE_FACTOR;
      accel(i) *= M_PER_SECS_SQUARED; // convert g-unit to m/s²
  }
}

// Get gyroscope and accelerometer offsets for 2000 measurement values
void MPU6050::get_offsets() {

  // Declare then initialize total raw values to zero
  Vector3f total_gyro_raw;
  Vector3f total_accel_raw;

  total_gyro_raw.setZero();
  total_accel_raw.setZero();

  size_t counter = 0; // While loop counter

  // Sum up offsets for 2000 measurement values
  while (counter < 2000) {
    for (int i = 0; i < 3; i++) {
      get_gyro_raw();
      total_gyro_raw(i) += gyro_raw[i];

      get_accel_raw();
      total_accel_raw(i) += accel_raw[i];
    }
      counter ++;
  }

  // Find average offsets by dividing total raw values by 2000
  for (int i = 0; i < 3; i++) {
    gyro_offsets(i) = total_gyro_raw(i) / 2000;
    accel_offsets(i) = total_accel_raw(i) / 2000;
  }

  // Remove 1g from the calculated z-axis accelerometer value to compensate for
  // gravity
  accel_offsets(2) = accel_offsets(2) - ACCEL_SCALE_FACTOR;
}

//
void MPU6050::update(Vector3f &gyro, Vector3f &accel) {

  // Get gyroscope and accelerometer values
  get_gyro(gyro);
  get_accel(accel);

}

// Add comments
// Code for calculating angles
