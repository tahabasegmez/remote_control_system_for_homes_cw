#include "include/SocketConLib.h"
#include <iostream>
#include <string>
#include <limits>
#include <signal.h>

// Global socket connection
SocketCon serverConnection;

// Display main menu
void showMenu() {
    std::cout << "\n=== UZAKTAN DENETIM SISTEMI ===" << std::endl;
    std::cout << "1. Sensor Durumu Gor" << std::endl;
    std::cout << "2. Sensor Tipi Gor" << std::endl;
    std::cout << "3. Relay Durumu Degistir" << std::endl;
    std::cout << "4. Relay Durumu Gor" << std::endl;
    std::cout << "5. Keypad Durumu Gor" << std::endl;
    std::cout << "6. Sicaklik Gor" << std::endl;
    std::cout << "7. Gyro Verileri Gor" << std::endl;
    std::cout << "8. Ivme Verileri Gor" << std::endl;
    std::cout << "9. Baglanti Kapat" << std::endl;
    std::cout << "Seciminiz (1-9): ";
}

// Send command and get response
std::string sendCommand(const std::string& command) {
    serverConnection.send(command);
    return serverConnection.receive();
}

// Handle menu choice
void handleChoice(int choice) {
    std::string response;
    
    switch (choice) {
        case 1: // Sensor Status
            response = sendCommand("sensorDurum:");
            std::cout << "Sensor Durumu: " << response << std::endl;
            break;
            
        case 2: // Sensor Type
            response = sendCommand("sensorTip:");
            std::cout << "Sensor Tipi: " << response << std::endl;
            break;
            
        case 3: { // Change Relay Status
            response = sendCommand("relayDurum:");
            std::cout << "Guncel " << response << std::endl;
            
            // Extract current state from response: "relay X:"
            bool currentState = (response.find("1") != std::string::npos);
            
            if (currentState) {
                std::cout << "Relay simdiki durumu: ON" << std::endl;
                std::cout << "OFF yapmak icin '0' a basin: ";
                std::string input;
                std::cin >> input;
                if (input == "0") {
                    response = sendCommand("relay 0:");
                    std::cout << "Relay Yaniti: " << response << std::endl;
                }
            } else {
                std::cout << "Relay simdiki durumu: OFF" << std::endl;
                std::cout << "ON yapmak icin '1' e basin: ";
                std::string input;
                std::cin >> input;
                if (input == "1") {
                    response = sendCommand("relay 1:");
                    std::cout << "Relay Yaniti: " << response << std::endl;
                }
            }
            break;
        }
        
        case 4: // Relay Status
            response = sendCommand("relayDurum:");
            std::cout << "Relay Durumu: " << response << std::endl;
            break;
            
        case 5: // Keypad Status
            response = sendCommand("key:");
            std::cout << "KEY: " << response << std::endl;
            break;
            
        case 6: // Temperature
            response = sendCommand("temp:");
            std::cout << "Sicaklik: " << response << std::endl;
            break;
            
        case 7: // Gyro Data
            response = sendCommand("gyro:");
            std::cout << "Gyro: " << response << std::endl;
            break;
            
        case 8: // Accelerometer Data
            response = sendCommand("acc:");
            std::cout << "Ivme: " << response << std::endl;
            break;
            
        case 9: // Close Connection
            response = sendCommand("kapat:");
            std::cout << "Baglanti kapatiliyor: " << response << std::endl;
            break;
            
        default:
            std::cout << "Gecersiz secim!" << std::endl;
            break;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Client Node Starting..." << std::endl;
    
    // Check command line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <server_ip>" << std::endl;
        std::cerr << "Example: " << argv[0] << " 10.42.0.1" << std::endl;
        return 1;
    }
    
    // Initialize socket
    serverConnection.init();
    
    // Get server IP from command line argument
    std::string serverIP = argv[1];
    int serverPort = 7001;
    
    std::cout << "Connecting to Server Node at " << serverIP << ":" << serverPort << "..." << std::endl;
    
    if (!serverConnection.connectToServer(serverIP, serverPort)) {
        std::cerr << "Failed to connect to Server Node!" << std::endl;
        return 1;
    }
    
    std::cout << "Connected to Server Node successfully!" << std::endl;
    
    bool running = true;
    while (running) {
        showMenu();
        
        int choice;
        std::cin >> choice;
        
        if (choice == 9) {
            handleChoice(choice);
            running = false;
        } else if (choice >= 1 && choice <= 8) {
            handleChoice(choice);
        } else {
            std::cout << "Gecersiz secim! Lutfen 1-9 arasi bir sayi girin." << std::endl;
        }
        
        if (running) {
            std::cout << "\nDevam etmek icin Enter'a basin...";
            std::cin.ignore();
            std::cin.get();
        }
    }
    
    // Cleanup
    serverConnection.release();
    
    std::cout << "Client Node terminated." << std::endl;
    return 0;
} 