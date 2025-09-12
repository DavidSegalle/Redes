#include "file_manager.hpp"

FileManager::FileManager(){}

FileManager::~FileManager(){}

bool FileManager::calculateSha(char* file, int size, char sha[SHA256_DIGEST_LENGTH]){

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) return false;

    bool success = false;

    if(EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) == 1 &&
        EVP_DigestUpdate(mdctx, file, size) == 1) {
        
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

int FileManager::getFileSize(){

    FILE *fptr = fopen(this->full_filename, "rb");
    
    fseek(fptr, 0, SEEK_END);
    long size = (int) ftell(fptr);

    fclose(fptr);

    return size;
}

bool FileManager::loadFile(char* file, int size){

    /*if(!this->fileExists(filename)){
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
    
    this->loaded_file = (char* ) malloc(size);*/

    FILE *fptr = fopen(this->full_filename, "rb");

    size_t read = fread(file, 1, size, fptr);

    if (read != (size_t)size) {
        std::cout << "Issues with the size of the file when loading\n";
        return false;
    }

    fclose(fptr);
    return true;

}