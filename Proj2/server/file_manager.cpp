#include "file_manager.hpp"

FileManager::FileManager(){
    this->loaded_file = nullptr;
}

FileManager::~FileManager(){}

bool FileManager::calculateSha(char* filename, char sha[SHA256_DIGEST_LENGTH]){

    // Just in case
    this->loadFile(filename);
    
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) return false;

    bool success = false;

    if(EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) == 1 &&
        EVP_DigestUpdate(mdctx, this->loaded_file, this->file_size) == 1) {
        
        unsigned int out_len = 0;
        if (EVP_DigestFinal_ex(mdctx, (unsigned char* )sha, &out_len) == 1 && out_len == SHA256_DIGEST_LENGTH) {
            success = true;
        }
    }

    EVP_MD_CTX_free(mdctx);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        oss << std::setw(2) << static_cast<unsigned int>( static_cast<unsigned char>(sha[i]) );
    }

    std::cout << "File sha is:\n";
    std::cout << oss.str() << "\n";

    return success;

}

bool FileManager::checkLoaded(char* filename){
    
    if(!memcmp(this->filename, filename, FILENAME_LENGTH)){
        return true;
    }

    return false;

}

void FileManager::setFileFullName(char* filename){

    strcpy(this->full_filename, "files/");
    strcpy(this->full_filename + 6, filename);

}

bool FileManager::fileExists(char* filename){

    this->setFileFullName(filename);

    if (access(this->full_filename, F_OK) == 0) {
        std::cout << "File does indeed exist\n";
        return true;
    }
    std::cout << "File does not exist\n";
    return false;

}

int FileManager::getLastChunkSize(){
    return this->last_chunk_size;
}

bool FileManager::getFile(char* dst, char* filename, int index){

    if(memcmp(filename, this->filename, FILENAME_LENGTH)){
        if(!this->loadFile(filename)){
            std::cout << "Getting packet from unexistant file\n";
            return false;
        }
    }

    for(int i = 0; i < DATA_LENGTH; i++){
        dst[i] = '\0';
    }

    if(index < 0 || index > this->file_size / DATA_LENGTH){
        std::cout <<"Request for an invalid packet\n";
        return false;
    }

    for(int i = 0; i < DATA_LENGTH; i++){
        dst[i] = this->loaded_file[i + (DATA_LENGTH * index)];
    }

    return true;

}

int FileManager::getFileChunkCount(char* filename){
    if(!this->checkLoaded(filename)){
        this->loadFile(filename);
    }

    return (this->file_size / MSG_LENGTH) + 1;
}

bool FileManager::loadFile(char* filename){

    if(!this->fileExists(filename)){
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
    // If wrong file is loaded free
    if(!this->checkLoaded(filename) && this->loaded_file){
        free(this->loaded_file);
        this->loaded_file = nullptr;
        for(int i = 0; i < FILENAME_LENGTH; i++){
            this->filename[i] = 0;
        }
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

    memcpy(this->filename, filename, FILENAME_LENGTH);

    this->last_chunk_size = size % DATA_LENGTH;
    this->file_size = (int)size;

    std::cout << "File \"" << this->filename << "\" loaded into memory\n";

    std::cout << "Filesize is: " << size << " bits and it takes up: " << (size / DATA_LENGTH) + 1 << " chunks\n";

    fclose(fptr);
    return true;

}