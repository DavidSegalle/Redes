#include "file_manager.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

FileManager::FileManager(){}

void FileManager::fileChunkCount(char* filename, char* chunk_count){
    
    if(!this->file_exists(filename)){
        for(int i = 0; i < PACKET_ID_LENGTH; i++){
            chunk_count[i] = 0;
        }
    }

    // Load file to memory, divide by DATA_LENGTH, do array magic to put the number into chunk_count.


}

std::string FileManager::packetToString(int num){ // Number is stored backwards
    if(num >= 26*26*26 || num < 0){
        return std::string("FFF");
    }

    std::string value = "aaa";

    /*for(int i = 0; i < REQ_LENGTH; i++){
        value[i] += num % 26;
        num /= 26;
    }*/

    return value;
}

std::string FileManager::loadFile(std::string filename){

    std::filesystem::path dir ("files");
    std::filesystem::path full_path = dir / filename.data();

    std::ifstream file(full_path);

    std::cout << "Opening file " << full_path << "\n";

    if (!file.is_open()) {
        // print error message and return
        std::cerr << "Failed to open file, but file exists" << std::endl;
        exit(1);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();  // read entire file into buffer
    return buffer.str();

}

std::string FileManager::getChecksum(std::string text_block){

    char value[CHECKSUM_LENGTH];

    for(int i = 0; i < CHECKSUM_LENGTH; i++){
        value[i] = 0;
    }

    int digit = 0;
    for(int i = 0; i < text_block.size(); i++){
        
        digit = text_block[i];
    
        value[0] += digit % 26;

        for(int j = 0; j < CHECKSUM_LENGTH; j++){
            
            if(value[j] > 26){

                value[j] %= 26;

                if((j + 1) <= CHECKSUM_LENGTH){
                    value[j+1]++;
                }
            }
        }
        
    }

    for(int i = 0; i < CHECKSUM_LENGTH; i++){
        value[i] += 'a';
    }

    return std::string(value);
}

bool FileManager::file_exists(char* filename){

    int folder_len = 6;

    char full_name[FILENAME_LENGTH + folder_len];

    strcpy(full_name, "files/");

    strcpy(full_name + folder_len, filename);

    if (access(full_name, F_OK) == 0) {
        std::cout << "File does indeed exist\n";
        return true;
    }
    std::cout << "File does not exist\n";
    return false;

}

std::vector<std::string> FileManager::loadFileChunks(std::string filename){

    std::string loaded_file = this->loadFile(filename);

    // In form:  pdat'\n'textfilename'\n'ID'\n'data'\0'.
    //int block_textsize = MSG_LENGTH - REQ_LENGTH - CHECKSUM_LENGTH - filename.length() - 5/* number of \n */ - 3 /*maxfile is zzz blocks so 3 bites max*/;
int block_textsize=0;
    int blocks = 1 + loaded_file.length() / block_textsize;

    std::vector<std::string> separated_file;

    for(int i = 0; i < blocks; i++){
        std::cout << "Loading block number " << i << '\n';
        
        std::string text_block;
        text_block += ServerResponses::packetdata;
        text_block += '\n' + filename + "\n";
        text_block += std::to_string(i);


        // Colocar a checksum aqui
        std::string current_block = loaded_file.substr(i * block_textsize, block_textsize);

        text_block += std::string("\n") + current_block + std::string("\n");
        
        std::string checksum = this->getChecksum(text_block);

        separated_file.push_back(text_block += checksum);

        //std::cout << text_block << "\nwas loaded" << "\n";
    }

    return separated_file;

}