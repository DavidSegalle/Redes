#pragma once

#include <string> 
#include <iostream>
#include <sstream>

class ProcessRequest{

public:
    
    // Server side processing
    std::string getFileInfo (std::string msg);
    std::string getPacket   (std::string msg);
    std::string getChecksum (std::string msg);

    // Client side processing
    std::string rcvFileInfo     (std::string msg);
    std::string rcvPacketData (std::string msg);
    std::string rcvChecksum  (std::string msg);

private:
    std::string loaded_textfile;

};