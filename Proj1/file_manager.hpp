#pragma once

#include <string> 
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>
#include <string.h>

#include "message_headers.hpp"

class FileManager{

public:
    
    FileManager();

    // Server side processing
    
    // Tells how many chunks the file needs to be sent via the server
    void fileChunkCount(char* filename, char* chunk_count);

    // Returns false if file cannot be loaded
    bool loadFile(char* filename);

    bool loadPacket(char* filename, char* area, char* index);

private:

    bool file_exists(char* filename);
    void setPacketCount(char* chunk_count, uint32_t count);
    void setFileFullName(char* filename);

private:
    //files/ has 6 characters lol
    static const int folder_len = 6;

    char loaded_filename[FILENAME_LENGTH];
    uint32_t loaded_file_chunk_count;
    char full_filename[FILENAME_LENGTH + folder_len];
    
    char* loaded_file;

};