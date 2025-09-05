#pragma once

#include <string> 
#include <iostream>
#include <sstream>
#include <string.h>

#include "file_manager.hpp"
#include "message_headers.hpp"

class ProcessRequest{

public:
    
    ProcessRequest();
    // Server side processing
    void getFileInfo(GetFile* msg, SendFileInfo* reply);
    std::string getPacket   (std::string msg);
    //std::string getChecksum (std::string msg);

    // Client side processing
    std::string rcvFileInfo     (std::string msg);
    std::string rcvPacketData (std::string msg);
    std::string rcvChecksum  (std::string msg);

private:
    std::vector<std::string> loaded_textfile;

    FileManager file_manager;

};