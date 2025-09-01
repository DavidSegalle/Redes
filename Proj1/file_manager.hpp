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

    std::vector<std::string> load_file(std::string filename);

private:


};