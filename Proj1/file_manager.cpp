#include "file_manager.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

FileManager::FileManager(){

    for(uint i = 0; i < FILENAME_LENGTH; i++){
        this->loaded_filename[i] = 0;
    }

    for(uint i = 0; i < FILENAME_LENGTH + this->folder_len; i++){
        this->full_filename[i] = 0;
    }

    this->loaded_file_chunk_count = 0;
    this->loaded_file = nullptr;

}

uint32_t FileManager::getLastChunkSize(){
    return this->last_chunk_size;
}

void FileManager::setPacketCount(char* chunk_count, uint32_t count){
    memcpy(chunk_count, &count, PACKET_ID_LENGTH);
}

void FileManager::setFileFullName(char* filename){

    strcpy(full_filename, "files/");
    strcpy(full_filename + folder_len, filename);

}

void FileManager::fileChunkCount(char* filename, char* chunk_count){
    uint32_t count = 0;

    // Set as 0 packets in case of error
    this->setPacketCount(chunk_count, count);

    if(!this->loadFile(filename)){
        return;
    }

    count = this->loaded_file_chunk_count;

    this->setPacketCount(chunk_count, count);
    
}

bool FileManager::file_exists(char* filename){

    this->setFileFullName(filename);

    if (access(this->full_filename, F_OK) == 0) {
        std::cout << "File does indeed exist\n";
        return true;
    }
    std::cout << "File does not exist\n";
    return false;

}

bool FileManager::loadFile(char* filename){

    if(!this->file_exists(filename)){
        return false;
    }

    this->setFileFullName(filename);

    FILE *fptr = fopen(this->full_filename, "rb");
    
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (size < 0) {
        std::cout << "Issues opening the file\n";
        fclose(fptr);
        return false;
    }

    if(this->loaded_file){
        free(this->loaded_file);
        this->loaded_file = nullptr;
    }
    
    this->loaded_file = (char* ) malloc(size);

    size_t read = fread(this->loaded_file, 1, size, fptr);

    if (read != (size_t)size) {
        std::cout << "Issues with the size of the file when loading\n";
        free(this->loaded_file);
        this->loaded_file = nullptr;
        fclose(fptr);
        return false;
    }

    strcpy(this->loaded_filename, filename);
    this->last_chunk_size = size % DATA_LENGTH;

    std::cout << "File \"" << this->loaded_filename << "\" loaded into memory\n";

    this->loaded_file_chunk_count = (size / DATA_LENGTH) + 1;
    
    std::cout << "Filesize is: " << size << " bits and it takes up: " << this->loaded_file_chunk_count << " chunks\n";

    fclose(fptr);
    return true;

}

bool FileManager::loadPacket(char* filename, char* area, char* index){

    uint32_t real_index;
    memcpy(&real_index, index, PACKET_ID_LENGTH);

    bool loaded = true;

    int correct_name = memcmp(this->loaded_filename, filename, FILENAME_LENGTH);

    // If no file is loaded
    if(!this->loaded_file){
        loaded = this->loadFile(filename);
    }
    // If the loaded file is not the correct one
    // memcmp has 0 as the result for equal value, weird
    else if(correct_name){
        loaded = this->loadFile(filename);
    }

    if(!loaded){
        return false;
    }

    if(this->loaded_file_chunk_count < real_index){
        return false;
    }

    if(real_index + 1 == this->loaded_file_chunk_count){
        memcpy(area, this->loaded_file + (real_index * DATA_LENGTH), this->last_chunk_size);
        //std::cout.write(area, this->last_chunk_size);
    }
    else{
        memcpy(area, this->loaded_file + (real_index * DATA_LENGTH), DATA_LENGTH);
        //std::cout.write(area, DATA_LENGTH);
    }

    
    std::cout << "\n";

    return true;

}
