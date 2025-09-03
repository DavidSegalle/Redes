#include "file_manager.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

FileManager::FileManager(){}

std::string FileManager::packetToString(int num){ // Number is stored backwards
    if(num >= 26*26*26 || num < 0){
        return std::string("FFF");
    }

    std::string value = "aaa";

    for(int i = 0; i < REQ_LENGTH; i++){
        value[i] += num % 26;
        num /= 26;
    }

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

bool FileManager::file_exists(std::string filename){

    std::filesystem::path dir ("files");
    std::filesystem::path full_path = dir / filename.data();

    struct stat buffer;
    return (stat (full_path.c_str(), &buffer) == 0);

}

std::vector<std::string> FileManager::loadFileChunks(std::string filename){

    std::string loaded_file = this->loadFile(filename);

    // In form:  pdat'\n'textfilename'\n'ID'\n'data'\0'.
    int block_textsize = MSG_LENGTH - REQ_LENGTH - CHECKSUM_LENGTH - filename.length() - 5/* number of \n */ - 3 /*maxfile is zzz blocks so 3 bites max*/;

    int blocks = 1 + loaded_file.length() / block_textsize;

    std::vector<std::string> separated_file;

    for(int i = 0; i < blocks; i++){
        std::cout << "Loading block number " << i << '\n';
        
        std::string text_block;
        text_block += ServerRequests::packetdata;
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