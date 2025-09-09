#pragma once

#include <iostream>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_manager.hpp"
#include "../message_headers.hpp"

class ProcessRequest{

public:
    
    ProcessRequest(int client_socket);
    ~ProcessRequest();

    void mainLoop();

private:

    void getFileInfo();
    void getPacket();
    //std::string getChecksum (std::string msg);

    void setChecksum(Message* msg);
    bool checkChecksum(Message* msg);

    void closeConnection();

    FileManager file_manager;

    int client_socket;

    Message request, response;

};