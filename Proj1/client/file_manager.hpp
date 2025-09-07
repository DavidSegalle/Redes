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
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "../message_headers.hpp"

class FileManager{

public:
    
    FileManager(char* filename, uint32_t total_chunk_count, uint32_t last_chunk_size);
    ~FileManager();


    bool savePacket(char* data, uint32_t index);

    uint32_t getNextEmpty();

    bool saveFile();

    bool complete();


private:
    //files/ has 6 characters lol
    static const int folder_len = 6;

    char filename[FILENAME_LENGTH];
    bool* loaded_chunks;
    char* file_data;

    uint32_t total_chunk_count;
    uint32_t last_chunk_size;

};