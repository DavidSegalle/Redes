#include "file_manager.hpp"

FileManager::FileManager(){}
FileManager::~FileManager(){}

void FileManager::saveFile(char* filename, char* file, int size){

    char full_filename[FILENAME_LENGTH + 7];

    for(int i = 0; i < FILENAME_LENGTH + 1; i++){
        full_filename[i] = '\0';
    }

    strcpy(full_filename, "files/");
    strcpy(full_filename + 6, filename);

    FILE* fptr = fopen(full_filename, "w");

    fwrite(file, sizeof(char), size, fptr);
    fclose(fptr);
}


bool FileManager::checkSha(char* file, int size, char sha[SHA256_DIGEST_LENGTH]){

    char new_sha[SHA256_DIGEST_LENGTH];

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) return false;

    if(EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) == 1 && EVP_DigestUpdate(mdctx, file, size) == 1) {
        
        unsigned int out_len = 0;
        EVP_DigestFinal_ex(mdctx, (unsigned char* )new_sha, &out_len);

    }

    EVP_MD_CTX_free(mdctx);

    if(!memcmp(sha, new_sha, SHA256_DIGEST_LENGTH)){
        return true;
    }
    return false;

}
