#include "file_manager.hpp"

FileManager::FileManager(){}
FileManager::~FileManager(){}

void FileManager::saveFile(char* filename, char* file, int size){

    char *full_filename;

    int full_size = size + 7;

    full_filename = new char[full_size];
    for(int i = 0; i < full_size + 1; i++){
        full_filename[i] = '\0';
    }

    strcpy(full_filename, "files/");
    strncpy(full_filename + 6, filename, full_size);

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

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        oss << std::setw(2) << static_cast<unsigned int>( static_cast<unsigned char>(sha[i]) );
    }

    std::cout << "Calculated sha is:\n";
    std::cout << oss.str() << "\n";

    if(!memcmp(sha, new_sha, SHA256_DIGEST_LENGTH)){
        return true;
    }
    return false;

}
