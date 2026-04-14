#include "include/SocketConLib.h"
#include <iostream>
#include <string>
#include <signal.h>

// Global objects
SocketCon clientSocket;        // Connection to client
SocketCon digitalIOSocket;     // Connection to DigitalIONode
SocketCon gyroSocket;          // Connection to GyroSensorNode
bool running = true;

// Signal handler
void signalHandler(int signal) {
    std::cout << "\nReceived shutdown signal..." << std::endl;
    running = false;
}

// Connect to DigitalIONode and GyroSensorNode
bool connectToNodes() {
    // Connect to DigitalIONode (port 7002)
    digitalIOSocket.init();
    if (!digitalIOSocket.connectToServer("127.0.0.1", 7002)) {
        std::cerr << "Failed to connect to DigitalIONode on port 7002" << std::endl;
        return false;
    }
    std::cout << "Connected to DigitalIONode" << std::endl;
    
    // Connect to GyroSensorNode (port 7003)
    gyroSocket.init();
    if (!gyroSocket.connectToServer("127.0.0.1", 7003)) {
        std::cerr << "Failed to connect to GyroSensorNode on port 7003" << std::endl;
        return false;
    }
    std::cout << "Connected to GyroSensorNode" << std::endl;
    
    return true;
}

// Determine which node should handle the message and forward it
void forwardMessage(const std::string& message) {
    std::cout << "Forwarding message: " << message << std::endl;
    
    // Messages for DigitalIONode (from Table 1)
    if (message == "sensorDurum:" || 
        message == "sensorTip:" ||
        message == "relay 0:" ||
        message == "relay 1:" ||
        message == "relayDurum:" ||
        message == "key:") {
        
        // Send to DigitalIONode
        digitalIOSocket.send(message);
        
        // Get response and forward to client
        std::string response = digitalIOSocket.receive();
        if (!response.empty()) {
            clientSocket.send(response);
            std::cout << "Response from DigitalIONode: " << response << std::endl;
        }
    }
    // Messages for GyroSensorNode (from Table 2)
    else if (message == "temp:" ||
             message == "gyro:" ||
             message == "acc:") {
        
        // Send to GyroSensorNode
        gyroSocket.send(message);
        
        // Get response and forward to client
        std::string response = gyroSocket.receive();
        if (!response.empty()) {
            clientSocket.send(response);
            std::cout << "Response from GyroSensorNode: " << response << std::endl;
        }
    }
    // Special case: kapat message goes to both nodes
    else if (message == "kapat:") {
        // Send to both nodes
        digitalIOSocket.send(message);
        gyroSocket.send(message);
        
        // Get responses from both nodes
        std::string digitalResponse = digitalIOSocket.receive();
        std::string gyroResponse = gyroSocket.receive();
        
        // Send confirmation to client
        clientSocket.send("kapat ok:");
        std::cout << "Shutdown command sent to both nodes" << std::endl;
        
        running = false;
    }
    else {
        // Unknown message - send error
        clientSocket.send("error: unknown command");
        std::cout << "Unknown command: " << message << std::endl;
    }
}

int main() {
    std::cout << "Server Node Starting..." << std::endl;
    
    // Setup signal handling
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Connect to both nodes first
    if (!connectToNodes()) {
        std::cerr << "Failed to connect to required nodes" << std::endl;
        return 1;
    }
    
    // Initialize server socket
    clientSocket.init();
    
    // Start server on port 7001
    if (!clientSocket.startServer(7001)) {
        std::cerr << "Failed to start server on port 7001" << std::endl;
        return 1;
    }
    
    std::cout << "Server Node listening on port 7001..." << std::endl;
    
    while (running) {
        // Accept client connection
        if (clientSocket.acceptClient()) {
            std::cout << "Client connected to Server Node" << std::endl;
            
            while (clientSocket.isConnected() && running) {
                // Check for incoming messages from client
                std::string message = clientSocket.receive();
                if (!message.empty()) {
                    forwardMessage(message);
                }
                
                // Small delay
                usleep(50000); // 50ms
            }
        }
        usleep(100000); // 100ms
    }
    
    // Cleanup
    digitalIOSocket.release();
    gyroSocket.release();
    clientSocket.release();
    
    std::cout << "Server Node terminated." << std::endl;
    return 0;
} 