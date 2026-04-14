#ifndef DIGSENSORLIB_H
#define DIGSENSORLIB_H

class DigSensor {
public:
    // Required methods from specifications
    void init();           // Initialize sensor
    void release();        // Release sensor resources  
    bool read();           // Read sensor value
    
    // Additional helper methods for compatibility
    const char* getSensorType();    // Returns "ISIK" for light sensor
    int getSensorStatus();          // Returns 0 or 1
    
private:
    bool initialized;
    bool currentValue;
    int sensorPin;  // GPIO 17 as per specification
    
    // Platform detection and GPIO setup
    void setupGPIO();
    void cleanupGPIO();
    bool readGPIO();
};

#endif // DIGSENSORLIB_H 