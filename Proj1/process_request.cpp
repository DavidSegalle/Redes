#include "process_request.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

ProcessRequest::ProcessRequest() : 
loaded_textfile(){}

std::string ProcessRequest::getFileInfo(std::string msg){

    std::string filename = msg.substr(5);

    std::cout << "Received a file request for: " << filename << "\n";

    // Check if file exists, if so return adequate answer
    if(!file_manager.file_exists(filename)){
        // Placeholder for does not exist
        return std::string();
    }
    
    loaded_textfile = file_manager.load_file(filename);

    std::string reply = "pinf\n";
    reply += filename + "\n";
    reply += std::to_string(loaded_textfile.size()) + "\n";

    return reply;

}

std::string ProcessRequest::getPacket(std::string msg){
    std::string req_data = msg.substr(5);

    std::stringstream ss(req_data);

    std::string filename, packet_id;

    getline(ss, filename, '\n');
    getline(ss, packet_id, '\n');

    std::cout << "Received a request for packet number " << packet_id << " for the file " << filename << "\n";

    if(!loaded_textfile.empty() && loaded_textfile.size() > (long unsigned int)std::stoi(packet_id)){
        return loaded_textfile[std::stoi(packet_id)];
    }

    return std::string("pdat\n0\n");
}

std::string ProcessRequest::getChecksum(std::string msg){

    std::string filename = msg.substr(5);

    // Check if file exists and if so calculate checksum
    // Plasceholder
    return std::string();
}