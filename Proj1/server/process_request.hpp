#pragma once

#include <iostream>
#include <string.h>

#include "file_manager.hpp"
#include "../message_headers.hpp"

class ProcessRequest{

public:
    
    ProcessRequest();
    // Server side processing
    void getFileInfo(GetFile* msg, SendFileInfo* reply);
    void getPacket(GetIndex* msg, SendFileData* reply);
    //std::string getChecksum (std::string msg);

    void setChecksum(Message* msg);
    bool checkChecksum(Message* msg);

private:

    FileManager file_manager;

};