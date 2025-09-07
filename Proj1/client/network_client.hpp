#pragma once

#include <string> 
#include <iostream>
#include <sstream>
#include <string.h>

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "../message_headers.hpp"
#include "file_manager.hpp"

class NetworkClient{

public:
    
    NetworkClient(std::string ip, std::string port, std::string msg);
    ~NetworkClient();

    void generateFileRequest(Message* msg, Message* response);

    void requestFile(Message* msg, Message* response);

    void sendMsg(Message* message, Message* response);

private:

    void setChecksum(Message* msg);
    bool checkChecksum(Message* msg);

    std::string ip, port;

    char filename[FILENAME_LENGTH];

    FileManager* file_manager;

    int sockfd;
    struct sockaddr_in servaddr; 

};