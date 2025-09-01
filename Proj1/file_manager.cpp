#include "file_manager.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

FileManager::FileManager(){}

bool FileManager::file_exists(std::string filename){

    std::filesystem::path dir ("files");
    std::filesystem::path full_path = dir / filename.data();

    struct stat buffer;
    return (stat (full_path.c_str(), &buffer) == 0); 

}

std::vector<std::string> FileManager::load_file(std::string filename){

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
    std::string loaded_file = buffer.str();

    // In form:  pdat'\n'textfilename'\n'ID'\n'data'\0'.
    int block_textsize = MSG_LENGTH - REQ_LENGTH - filename.length() - 4/* number of \n */ - 3 /*maxfile is 999 blocks so 3 bits*/;

    int blocks = 1 + loaded_file.length() / block_textsize;

    std::vector<std::string> separated_file;

    for(int i = 0; i < blocks; i++){
        std::cout << "Loading block number " << i << '\n';
        
        std::string text_block;
        text_block += ServerRequests::packetdata;
        text_block += '\n' + filename + "\n";
        text_block += std::to_string(i);
        text_block +=  "\n" + loaded_file.substr(i * block_textsize, block_textsize);
        
        separated_file.push_back(text_block);

        //std::cout << text_block << "\nwas loaded" << "\n";
    }

    return separated_file;

}