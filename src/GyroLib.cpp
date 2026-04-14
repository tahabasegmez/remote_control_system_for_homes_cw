#include "../include/GyroLib.h"
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

// Check if running on Raspberry Pi
#ifdef __arm__
    #include <wiringPiI2C.h>
    #define USE_WIRINGPI 1
#else
    #define USE_WIRINGPI 0
    // For non-Pi systems, provide dummy implementations
    #include <fcntl.h>
    #include <sys/ioctl.h>
    
    // Dummy wiringPi functions for compilation on non-Pi systems
    int wiringPiI2CSetup(int device_addr) {
        std::cout << "Dummy I2C setup for device 0x" << std::hex << device_addr << std::endl;
        return 1; // Return dummy handle
    }
    
    int wiringPiI2CWriteReg8(int handle, int reg, int data) {
        std::cout << "Dummy I2C write: reg=0x" << std::hex << reg << " data=0x" << data << std::endl;
        return 0;
    }
    
    int wiringPiI2CReadReg8(int handle, int reg) {
        std::cout << "Dummy I2C read: reg=0x" << std::hex << reg << std::endl;
        return 0; // Return dummy data
    }
#endif

bool Gyro::init() {
#if USE_WIRINGPI
    std::cout << "Initializing MPU9250/MPU6050 with wiringPi I2C..." << std::endl;
    
    // Setup I2C communication
    device_handle = wiringPiI2CSetup(I2C_DEVICE_ADDR);
    if (device_handle < 0) {
        std::cerr << "Failed to open I2C communication to IMU" << std::endl;
        return false;
    }
    
    // Wake up the device
    if (!wakeUpDevice()) {
        std::cerr << "Failed to wake up IMU device" << std::endl;
        return false;
    }
    
    // Enable I2C Master mode
    wiringPiI2CWriteReg8(device_handle, USER_CTRL, 0x20);
    
    // Wait for device to be ready
    deviceWait(100);
    
    // Set default scales
    setGyroScale(250);    // 250 degrees/second
    setAccelScale(2);     // 2g
    
    initialized = true;
    std::cout << "MPU9250/MPU6050 initialized successfully" << std::endl;
    return true;
    
#else
    std::cout << "Initializing MPU9250/MPU6050 in simulation mode (non-Pi system)..." << std::endl;
    device_handle = 1; // Dummy handle
    gyro_scale_factor = GYRO_SENSITIVITY_250;
    accel_scale_factor = ACCEL_SENSITIVITY_2G;
    initialized = true;
    return true;
#endif
}

bool Gyro::wakeUpDevice() {
#if USE_WIRINGPI
    int wake_result = wiringPiI2CWriteReg8(device_handle, PWR_MGMT_1_ADDR, PWR_MGMT_1_VAL);
    return wake_result >= 0;
#else
    return true; // Dummy success
#endif
}

void Gyro::setGyroScale(int scale_val) {
    switch (scale_val) {
        case 250:
            gyro_scale_factor = GYRO_SENSITIVITY_250;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, GYRO_CONFIG_ADDR, GYRO_FSCALE_250);
#endif
            break;
        case 500:
            gyro_scale_factor = GYRO_SENSITIVITY_500;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, GYRO_CONFIG_ADDR, GYRO_FSCALE_500);
#endif
            break;
        case 1000:
            gyro_scale_factor = GYRO_SENSITIVITY_1000;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, GYRO_CONFIG_ADDR, GYRO_FSCALE_1000);
#endif
            break;
        case 2000:
            gyro_scale_factor = GYRO_SENSITIVITY_2000;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, GYRO_CONFIG_ADDR, GYRO_FSCALE_2000);
#endif
            break;
        default:
            std::cout << "Unable to set gyro scale, using default 250°/s" << std::endl;
            gyro_scale_factor = GYRO_SENSITIVITY_250;
            break;
    }
}

void Gyro::setAccelScale(int scale_val) {
    switch (scale_val) {
        case 2:
            accel_scale_factor = ACCEL_SENSITIVITY_2G;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, ACCEL_CONFIG_ADDR, ACCEL_FSCALE_2G);
#endif
            break;
        case 4:
            accel_scale_factor = ACCEL_SENSITIVITY_4G;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, ACCEL_CONFIG_ADDR, ACCEL_FSCALE_4G);
#endif
            break;
        case 8:
            accel_scale_factor = ACCEL_SENSITIVITY_8G;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, ACCEL_CONFIG_ADDR, ACCEL_FSCALE_8G);
#endif
            break;
        case 16:
            accel_scale_factor = ACCEL_SENSITIVITY_16G;
#if USE_WIRINGPI
            wiringPiI2CWriteReg8(device_handle, ACCEL_CONFIG_ADDR, ACCEL_FSCALE_16G);
#endif
            break;
        default:
            std::cout << "Unable to set accel scale, using default 2g" << std::endl;
            accel_scale_factor = ACCEL_SENSITIVITY_2G;
            break;
    }
}

int Gyro::readWord(int address) {
#if USE_WIRINGPI
    int value;
    value = wiringPiI2CReadReg8(device_handle, address);
    value = value << 8;
    value += wiringPiI2CReadReg8(device_handle, address + 1);
    if (value >= 0x8000)
        value = -(65536 - value);
    return value;
#else
    // Return simulated data for non-Pi systems
    static int counter = 0;
    counter++;
    return (counter % 1000) - 500; // Simulate sensor data
#endif
}

double Gyro::getGyroX() {
    if (!initialized) return 0.0;
    
    int raw_gyro_x = readWord(GYRO_XOUT_H_ADDR);
    return scaleGyro(raw_gyro_x);
}

double Gyro::getGyroY() {
    if (!initialized) return 0.0;
    
    int raw_gyro_y = readWord(GYRO_YOUT_H_ADDR);
    return scaleGyro(raw_gyro_y);
}

double Gyro::getGyroZ() {
    if (!initialized) return 0.0;
    
    int raw_gyro_z = readWord(GYRO_ZOUT_H_ADDR);
    return scaleGyro(raw_gyro_z);
}

double Gyro::getAccelX() {
    if (!initialized) return 0.0;
    
    int raw_accel_x = readWord(ACCEL_XOUT_H_ADDR);
    return scaleAccel(raw_accel_x);
}

double Gyro::getAccelY() {
    if (!initialized) return 0.0;
    
    int raw_accel_y = readWord(ACCEL_YOUT_H_ADDR);
    return scaleAccel(raw_accel_y);
}

double Gyro::getAccelZ() {
    if (!initialized) return 0.0;
    
    int raw_accel_z = readWord(ACCEL_ZOUT_H_ADDR);
    return scaleAccel(raw_accel_z);
}

double Gyro::getTemperature() {
    if (!initialized) return 0.0;
    
    int raw_temp = readWord(TEMP_OUT_H_ADDR);
    return scaleTemp(raw_temp);
}

double Gyro::scaleGyro(int raw_gyro) {
    return static_cast<double>(raw_gyro) / gyro_scale_factor;
}

double Gyro::scaleAccel(int raw_accel) {
    return (static_cast<double>(raw_accel) / accel_scale_factor) * GRAVITATIONAL_CONST;
}

double Gyro::scaleTemp(int raw_temp) {
    return ((static_cast<double>(raw_temp) - ROOMTEMP_OFFSET) / TEMP_SENSITIVITY) + 16.0;
}

void Gyro::deviceWait(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}