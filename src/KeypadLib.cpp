#include "../include/KeypadLib.h"
#include <iostream>

// Platform detection
#ifdef __arm__
    #define IS_RASPBERRY_PI 1
#else
    #define IS_RASPBERRY_PI 0
#endif

void Keypad::init() {
    std::cout << "Initializing Keypad..." << std::endl;
    initialized = false;
    setupGPIO();
    initialized = true;
    std::cout << "Keypad initialized successfully" << std::endl;
}

void Keypad::release() {
    if (initialized) {
        cleanupGPIO();
        initialized = false;
        std::cout << "Keypad released" << std::endl;
    }
}

char Keypad::getKey() {
    if (!initialized) return '\0';
    return scanKeys();
}

void Keypad::setupGPIO() {
#if IS_RASPBERRY_PI
    // Real GPIO setup for Raspberry Pi would go here
#endif
    // Platform-independent initialization
}

void Keypad::cleanupGPIO() {
#if IS_RASPBERRY_PI
    // Real GPIO cleanup for Raspberry Pi would go here
#endif
    // Platform-independent cleanup
}

char Keypad::scanKeys() {
#if IS_RASPBERRY_PI
    // Real keypad scanning would go here
    return '\0';
#else
    // Simulation mode - no keys pressed
    return '\0';
#endif
} 