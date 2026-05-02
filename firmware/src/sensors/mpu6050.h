// Adapted from https://github.com/raspberrypi/pico-examples/blob/master/i2c/mpu6050_i2c/mpu6050_i2c.c
// and https://shillehtek.com/blogs/news/pico-w-mpu6050-read-imu-data-cpp

#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "Core"
#include "stdint.h"
#include "stddef.h"
#include <math.h>
#include <pico/time.h>
#include <stdint.h>
#include <pico/binary_info.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <array>
#include <cstdint>
#include <../include/Eigen/Dense>

// I2C defines
#define I2C_PORT i2c0
#define MPU_ADDR 0x68
#define MPU_SDA_PIN 0
#define MPU_SCL_PIN 1

// MPU6050 register addresses
#define REG_PWR_MGMT_1 0x6B
#define REG_ACCEL_XOUT_H 0x3B
#define REG_GYRO_XOUT_H 0x43
#define REG_CONFIG 0x1A
#define REG_GYRO_CONFIG 0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_SMPLRT_DIV 0x19
#define WHO_AM_I_REG 0x75

// Sensitivity scale factors for different ranges
#define ACCEL_SCALE_FACTOR_2G 16384.0  // for ±2g
#define ACCEL_SCALE_FACTOR_4G 8192.0   // for ±4g
#define ACCEL_SCALE_FACTOR_8G 4096.0   // for ±8g
#define ACCEL_SCALE_FACTOR_16G 2048.0  // for ±16g

#define GYRO_SCALE_FACTOR_250DPS 131.0 // for ±250 degrees per second
#define GYRO_SCALE_FACTOR_500DPS 65.5  // for ±500 degrees per second
#define GYRO_SCALE_FACTOR_1000DPS 32.8 // for ±1000 degrees per second
#define GYRO_SCALE_FACTOR_2000DPS 16.4 // for ±2000 degrees per second

// Select the desired scale factor
#define ACCEL_SCALE_FACTOR ACCEL_SCALE_FACTOR_2G
#define GYRO_SCALE_FACTOR GYRO_SCALE_FACTOR_500DPS

// Corresponding configuration values
#define ACCEL_CONFIG_VALUE 0x00 // for ±2g
#define GYRO_CONFIG_VALUE 0x08  // for ±500 degrees per second, 0x00 for 250dps
#define SAMPLE_RATE_DIV 4       // Sample rate = 1kHz / (1 + 4) = 200Hz

#define M_PER_SECS_SQUARED 9.80665 // Acceleration G-unit to m/s^2

using Eigen::Vector3f;

class MPU6050 {
  private:
    std::array<int16_t, 3> accel_raw;
    std::array<int16_t, 3> gyro_raw;

    Vector3f gyro_offsets;
    Vector3f accel_offsets;

    Vector3f gyro;
    Vector3f accel;

  public:
    MPU6050();
    void get_accel_raw();
    void get_gyro_raw();
    void get_accel(Vector3f &accel);
    void get_gyro(Vector3f &gyro);
    void get_offsets();
    void update(Vector3f &gyro, Vector3f &accel);
};

#endif //__MPU6050_H__

