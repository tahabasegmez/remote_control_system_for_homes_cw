#ifndef SOCKETCONLIB_H
#define SOCKETCONLIB_H

#include <string>
#include <vector>

class SocketCon {
public:
    // Constructor
    SocketCon();
    
    // Destructor
    ~SocketCon();
    
    // Initialize socket
    void init();
    
    // Release resources
    void release();
    
    // Server functions
    bool startServer(int port);
    bool acceptClient();
    
    // Client functions
    bool connectToServer(const std::string& ip, int port);
    
    // Communication functions
    bool send(const std::string& message);
    std::string receive();
    
    // Connection status
    bool isConnected();
    
private:
    int server_socket;
    int client_socket;
    bool is_server;
    bool connected;
    bool initialized;
    
    // Internal helper functions
    void closeSocket(int socket_fd);
    bool setSocketOptions(int socket_fd);
};

#endif // SOCKETCONLIB_H 