#include "../include/SocketConLib.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

SocketCon::SocketCon() : server_socket(-1), client_socket(-1), is_server(false), connected(false), initialized(false) {
}

SocketCon::~SocketCon() {
    release();
}

void SocketCon::init() {
    initialized = true;
    std::cout << "Socket connection initialized" << std::endl;
}

void SocketCon::release() {
    if (client_socket >= 0) {
        closeSocket(client_socket);
        client_socket = -1;
    }
    
    if (server_socket >= 0) {
        closeSocket(server_socket);
        server_socket = -1;
    }
    
    connected = false;
    is_server = false;
    initialized = false;
}

bool SocketCon::startServer(int port) {
    if (!initialized) return false;
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create server socket" << std::endl;
        return false;
    }
    
    // Set socket options
    if (!setSocketOptions(server_socket)) {
        closeSocket(server_socket);
        server_socket = -1;
        return false;
    }
    
    // Bind socket
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind server socket to port " << port << std::endl;
        closeSocket(server_socket);
        server_socket = -1;
        return false;
    }
    
    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Failed to listen on server socket" << std::endl;
        closeSocket(server_socket);
        server_socket = -1;
        return false;
    }
    
    is_server = true;
    std::cout << "Server started on port " << port << std::endl;
    return true;
}

bool SocketCon::acceptClient() {
    if (!is_server || server_socket < 0) return false;
    
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        std::cerr << "Failed to accept client connection" << std::endl;
        return false;
    }
    
    connected = true;
    std::cout << "Client connected from " << inet_ntoa(client_addr.sin_addr) << std::endl;
    return true;
}

bool SocketCon::connectToServer(const std::string& ip, int port) {
    if (!initialized) return false;
    
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Failed to create client socket" << std::endl;
        return false;
    }
    
    // Connect to server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address: " << ip << std::endl;
        closeSocket(client_socket);
        client_socket = -1;
        return false;
    }
    
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server " << ip << ":" << port << std::endl;
        closeSocket(client_socket);
        client_socket = -1;
        return false;
    }
    
    connected = true;
    is_server = false;
    std::cout << "Connected to server " << ip << ":" << port << std::endl;
    return true;
}

bool SocketCon::send(const std::string& message) {
    if (!connected || client_socket < 0) return false;
    
    ssize_t bytes_sent = ::send(client_socket, message.c_str(), message.length(), 0);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send message" << std::endl;
        return false;
    }
    
    return true;
}

std::string SocketCon::receive() {
    if (!connected || client_socket < 0) return "";
    
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        if (bytes_received == 0) {
            std::cout << "Connection closed by peer" << std::endl;
        } else {
            std::cerr << "Error receiving data" << std::endl;
        }
        connected = false;
        return "";
    }
    
    buffer[bytes_received] = '\0';
    return std::string(buffer);
}

bool SocketCon::isConnected() {
    return connected;
}

void SocketCon::closeSocket(int socket_fd) {
    if (socket_fd >= 0) {
        close(socket_fd);
    }
}

bool SocketCon::setSocketOptions(int socket_fd) {
    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options" << std::endl;
        return false;
    }
    return true;
} 