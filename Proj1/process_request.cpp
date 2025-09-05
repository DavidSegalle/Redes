#include "process_request.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

ProcessRequest::ProcessRequest(){}

void ProcessRequest::getFileInfo(GetFile* msg, SendFileInfo* reply){

    std::cout << "Filename is: " << msg->filename << "\n";

    // Set the data that won't change type to sending info on a packet
    memcpy(reply->type, ServerResponses::packetinfo, PACKET_REQ_LENGTH);
    memcpy(reply->filename, msg->filename, FILENAME_LENGTH);
    
    file_manager.fileChunkCount(msg->filename, reply->packet_count);

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