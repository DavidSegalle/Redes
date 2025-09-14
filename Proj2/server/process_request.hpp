#pragma once

#include <iostream>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/evp.h>

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

    bool sendBytes(int sock, char* buf, int len);
    bool receiveBytes(char* data, int size);

    void getFileInfo();
    void getPacket();

    void closeConnection();

private:

    FileManager file_manager;

    int client_socket;

    char request_type[NUMBER_LENGTH];

    bool connection;

};