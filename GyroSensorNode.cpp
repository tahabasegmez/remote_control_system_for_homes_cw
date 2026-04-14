#include "include/GyroLib.h"
#include "include/SocketConLib.h"
#include <iostream>
#include <string>
#include <signal.h>

// Global objects
Gyro gyro;
SocketCon server;
bool running = true;

// Signal handler
void signalHandler(int signal) {
    std::cout << "\nReceived shutdown signal..." << std::endl;
    running = false;
}

// Handle incoming messages according to Table 2
void handleMessage(const std::string& message) {
    std::cout << "Received: " << message << std::endl;
    
    if (message == "temp:") {
        // Send temperature reading
        double temp = gyro.getTemperature();
        std::string response = "temp " + std::to_string(temp) + ":";
        server.send(response);
        std::cout << "Sent: " << response << std::endl;
    }
    else if (message == "gyro:") {
        // Send gyroscope readings
        double gx = gyro.getGyroX();
        double gy = gyro.getGyroY();
        double gz = gyro.getGyroZ();
        std::string response = "gyro " + std::to_string(gx) + " " + std::to_string(gy) + " " + std::to_string(gz) + ":";
        server.send(response);
        std::cout << "Sent: " << response << std::endl;
    }
    else if (message == "acc:") {
        // Send accelerometer readings
        double ax = gyro.getAccelX();
        double ay = gyro.getAccelY();
        double az = gyro.getAccelZ();
        std::string response = "acc " + std::to_string(ax) + " " + std::to_string(ay) + " " + std::to_string(az) + ":";
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
        server.send("kapat err:");
        std::cout << "Unknown command, sent: kapat err:" << std::endl;
    }
}

int main() {
    std::cout << "GyroSensor Node Starting..." << std::endl;
    
    // Setup signal handling
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Initialize components
    if (!gyro.init()) {
        std::cerr << "Failed to initialize gyro sensor" << std::endl;
        return 1;
    }
    server.init();
    
    // Start server on port 7003
    if (!server.startServer(7003)) {
        std::cerr << "Failed to start server on port 7003" << std::endl;
        return 1;
    }
    
    std::cout << "GyroSensor Node listening on port 7003..." << std::endl;
    
    while (running) {
        // Accept client connection
        if (server.acceptClient()) {
            std::cout << "Client connected to GyroSensor Node" << std::endl;
            
            while (server.isConnected() && running) {
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
    server.release();
    
    std::cout << "GyroSensor Node terminated." << std::endl;
    return 0;
} 