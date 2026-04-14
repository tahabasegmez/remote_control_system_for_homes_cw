#ifndef RELAYLIB_H
#define RELAYLIB_H

class Relay {
public:
    // Required methods from specifications
    void init();           // Initialize relay
    void release();        // Release relay resources
    void set(bool state);  // Set relay state (true = ON, false = OFF)
    
    // Additional helper method for status checking
    bool getState() const; // Get current relay state
    
private:
    bool initialized;
    bool currentState;
    int relayPin;  // GPIO 27 as per specification
    
    // Platform detection and GPIO setup
    void setupGPIO();
    void cleanupGPIO();
    void writeGPIO(bool state);
};

#endif // RELAYLIB_H 