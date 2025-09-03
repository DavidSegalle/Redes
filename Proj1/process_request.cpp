#include "process_request.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

ProcessRequest::ProcessRequest(){}

std::string ProcessRequest::getFileInfo(std::string msg){

    std::string filename = msg.substr(5);

    std::cout << "Received a file request for: " << filename << "\n";

    // Check if file exists, if so return adequate answer
    if(!file_manager.file_exists(filename)){
        // File does not exist reply
        return std::string(ServerRequests::packetinfo) + "\n0";
    }
    
    // Load file to vector<string>
    loaded_textfile = file_manager.loadFileChunks(filename);

    // Create server reply
    std::string reply = ServerRequests::packetinfo;
    reply += "\n" + filename + "\n";

    reply += file_manager.packetToString(loaded_textfile.size()) + "\n";

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
        
        // Bad code for checking if filename is bigger than packet to avoid issues
        std::string loaded_name = loaded_textfile[0].substr(5);

        if(loaded_name.size() > filename.size() && filename == loaded_textfile[0].substr(5, filename.size())){
            return loaded_textfile[std::stoi(packet_id)];
        }
    }

    return std::string("pdat\n0\n");
}