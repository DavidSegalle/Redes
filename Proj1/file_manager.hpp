#pragma once

#include <string> 
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>

#include "message_headers.hpp"

class FileManager{

public:
    
    FileManager();
    // Server side processing
    bool file_exists(std::string filename);

    std::vector<std::string> loadFileChunks(std::string filename);

    std::string getChecksum(std::string text_block);

    std::string packetToString(int i);

private:
    std::string loadFile(std::string filename);

};