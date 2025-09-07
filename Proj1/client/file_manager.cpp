#include "file_manager.hpp"


FileManager::FileManager(char* filename, uint32_t total_chunk_count, uint32_t last_chunk_size){

    memcpy(this->filename, filename, FILENAME_LENGTH);
    this->total_chunk_count = total_chunk_count;
    this->last_chunk_size = last_chunk_size;

    uint32_t bit_size = total_chunk_count * DATA_LENGTH;

    // Space for the file data
    this->file_data = (char* ) malloc(sizeof(char) * bit_size);
    for(uint32_t i = 0; i < bit_size; i++){
        this->file_data[i] = 0;
    }

    this->loaded_chunks = (bool* ) malloc(sizeof(bool) * total_chunk_count);
    for(uint32_t i = 0; i < total_chunk_count; i++){
        this->loaded_chunks[i] = false;
    }

    std::cout << "Allocating space for ";
    std::cout.write(this->filename, FILENAME_LENGTH);
    std::cout << "Which is " << this->total_chunk_count << " packets long\n";

}

FileManager::~FileManager(){

    free(this->file_data);
    free(this->loaded_chunks);

}


bool FileManager::savePacket(char* data, uint32_t index){

    if(index > this->total_chunk_count){
        return false;
    }

    uint32_t offset = index * DATA_LENGTH;

    for(uint32_t i = 0; i < DATA_LENGTH; i++){
        this->file_data[i + offset] = data[i];
    }

    this->loaded_chunks[index] = true; 

    return true;

}

uint32_t FileManager::getNextEmpty(){
    for(uint32_t i = 0; i < this->total_chunk_count; i++){
        if(!this->loaded_chunks[i]){
            return i;
        }
    }
    return 0;
}

bool FileManager::saveFile(){

    char full_filename[FILENAME_LENGTH + this->folder_len];
    for(uint32_t i = 0; i < FILENAME_LENGTH + this->folder_len; i++){
        full_filename[i] = '\0';
    }

    strcpy(full_filename, "files/");

    for(uint32_t i = 0; i < FILENAME_LENGTH; i++){
        full_filename[i + this->folder_len] = this->filename[i];
    }

    full_filename[FILENAME_LENGTH + this->folder_len - 1] = '\0'; // Just making sure

    uint32_t filesize = this->total_chunk_count * DATA_LENGTH - (DATA_LENGTH - this->last_chunk_size);


    // write struct to file
    std::ofstream out(full_filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(this->file_data), filesize);
    out.close();

    return true;
}

bool FileManager::complete(){

    for(uint32_t i = 0; i < this->total_chunk_count; i++){
        if(!this->loaded_chunks[i]){
            return false;
        }
    }
    return true;
}