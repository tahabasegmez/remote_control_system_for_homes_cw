#ifndef KEYPADLIB_H
#define KEYPADLIB_H

#include <string>

class Keypad {
public:
    // Required methods from specifications
    void init();           // Initialize keypad
    void release();        // Release keypad resources
    char getKey();         // Get pressed key
    
private:
    bool initialized;
    
    // Keypad layout (4x4)
    char keys[4][4] = {{'1','2','3','A'},
                       {'4','5','6','B'},
                       {'7','8','9','C'},
                       {'*','0','#','D'}};
    
    // GPIO pins - using specification: GPIO{16,20,21,12,06,13,19,26}
    int rowPins[4] = {16, 20, 21, 12};    // Input pins
    int colPins[4] = {6, 13, 19, 26};     // Output pins
    
    // Platform detection and GPIO setup
    void setupGPIO();
    void cleanupGPIO();
    char scanKeys();
};

#endif // KEYPADLIB_H 