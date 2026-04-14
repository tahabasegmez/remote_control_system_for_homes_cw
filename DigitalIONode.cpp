#include "include/KeypadLib.h"
#include "include/DigSensorLib.h"
#include "include/RelayLib.h"
#include "include/SocketConLib.h"
#include <iostream>
#include <string>
#include <signal.h>

// Global objects
Keypad keypad;
DigSensor sensor;
Relay relay;
SocketCon server;
bool running = true;
std::string lastKey = "";

// Signal handler
void signalHandler(int signal) {
    std::cout << "\nReceived shutdown signal..." << std::endl;
    running = false;
}

// Handle incoming messages according to Table 1
void handleMessage(const std::string& message) {
    std::cout << "Received: " << message << std::endl;
    
    if (message == "sensorDurum:") {
        // Read current sensor state and send response
        sensor.read();
        std::string response = "sensorDurum " + std::to_string(sensor.getSensorStatus()) + ":";
        server.send(response);
        std::cout << "Sent: " << response << std::endl;
    }
    else if (message == "sensorTip:") {
        // Send sensor type
        std::string response = "sensorTip " + std::string(sensor.getSensorType()) + ":";
        server.send(response);
        std::cout << "Sent: " << response << std::endl;
    }
    else if (message == "relay 0:") {
        // Turn relay OFF
        relay.set(false);
        server.send("relay ok:");
        std::cout << "Sent: relay ok:" << std::endl;
    }
    else if (message == "relay 1:") {
        // Turn relay ON
        relay.set(true);
        server.send("relay ok:");
        std::cout << "Sent: relay ok:" << std::endl;
    }
    else if (message == "relayDurum:") {
        // Send current relay state
        std::string response = "relay " + std::to_string(relay.getState() ? 1 : 0) + ":";
        server.send(response);
        std::cout << "Sent: " << response << std::endl;
    }
    else if (message == "key:") {
        // Send last pressed key
        std::string response = "key " + lastKey + ":";
        server.send(response);
        std::cout << "Sent: " << response << std::endl;
    }
    else if (message == "kapat:") {
        // Close connection
        server.send("kapat ok:");
        std::cout << "Sent: kapat ok:" << std::endl;
        running = false;
    }
    else {
        // Unknown command
        server.send("relay err:");
        std::cout << "Unknown command, sent: relay err:" << std::endl;
    }
}

int main() {
    std::cout << "DigitalIO Node Starting..." << std::endl;
    
    // Setup signal handling
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Initialize components
    keypad.init();
    sensor.init();
    relay.init();
    server.init();
    
    // Start server on port 7002
    if (!server.startServer(7002)) {
        std::cerr << "Failed to start server on port 7002" << std::endl;
        return 1;
    }
    
    std::cout << "DigitalIO Node listening on port 7002..." << std::endl;
    
    while (running) {
        // Accept client connection
        if (server.acceptClient()) {
            std::cout << "Client connected to DigitalIO Node" << std::endl;
            
            while (server.isConnected() && running) {
                // Check for keypad input
                char key = keypad.getKey();
                if (key != '\0') {
                    if (key == '#') {
                        // End of key sequence - do nothing special for now
                        std::cout << "Key sequence ended with #" << std::endl;
                    } else {
                        lastKey += key;
                        std::cout << "Key pressed: " << key << " (sequence: " << lastKey << ")" << std::endl;
                    }
                }
                
                // Check for incoming messages
                std::string message = server.receive();
                if (!message.empty()) {
                    handleMessage(message);
                }
                
                // Small delay
                usleep(50000); // 50ms
            }
        }
        usleep(100000); // 100ms
    }
    
    // Cleanup
    relay.release();
    sensor.release();
    keypad.release();
    server.release();
    
    std::cout << "DigitalIO Node terminated." << std::endl;
    return 0;
} 