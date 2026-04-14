#include "../include/DigSensorLib.h"
#include <iostream>

// Platform detection
#ifdef __arm__
    #define IS_RASPBERRY_PI 1
#else
    #define IS_RASPBERRY_PI 0
#endif

void DigSensor::init() {
    std::cout << "Initializing Digital Sensor..." << std::endl;
    initialized = false;
    sensorPin = 17;  // GPIO 17 as per specification
    currentValue = false;
    setupGPIO();
    initialized = true;
    std::cout << "Digital sensor initialized successfully on GPIO " << sensorPin << std::endl;
}

void DigSensor::release() {
    if (initialized) {
        cleanupGPIO();
        initialized = false;
        std::cout << "Digital sensor released" << std::endl;
    }
}

bool DigSensor::read() {
    if (!initialized) return false;
    currentValue = readGPIO();
    return currentValue;
}

const char* DigSensor::getSensorType() {
    return "ISIK";  // Light sensor type as per specification
}

int DigSensor::getSensorStatus() {
    return currentValue ? 1 : 0;
}

void DigSensor::setupGPIO() {
#if IS_RASPBERRY_PI
    // Real GPIO setup for Raspberry Pi would go here
#endif
    // Platform-independent initialization
}

void DigSensor::cleanupGPIO() {
#if IS_RASPBERRY_PI  
    // Real GPIO cleanup for Raspberry Pi would go here
#endif
    // Platform-independent cleanup
}

bool DigSensor::readGPIO() {
#if IS_RASPBERRY_PI
    // Real GPIO reading would go here
    // For now, simulate sensor reading
    return false;
#else
    // Simulation mode - return false (no light)
    return false;
#endif
} 