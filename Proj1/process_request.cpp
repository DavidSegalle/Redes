#include "process_request.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

ProcessRequest::ProcessRequest(){}

void ProcessRequest::setChecksum(Message* msg){

    uint32_t checksum = 0;

    for(uint i = 0; i < MSG_LENGTH - CHECKSUM_LENGTH; i++){
        checksum += msg->raw_data[i];
    }

    memcpy(&(msg->raw_data[MSG_LENGTH - CHECKSUM_LENGTH]), &checksum, PACKET_ID_LENGTH);

}

bool ProcessRequest::checkChecksum(Message* msg){

    uint32_t rcv_checksum = 0;

    memcpy(&rcv_checksum, &msg->raw_data[MSG_LENGTH - CHECKSUM_LENGTH], CHECKSUM_LENGTH);

    this->setChecksum(msg);

    uint32_t new_checksum = 0;

    memcpy(&new_checksum, &msg->raw_data[MSG_LENGTH - CHECKSUM_LENGTH], CHECKSUM_LENGTH);

    if(new_checksum == rcv_checksum){
        std::cout << "Checksum is correct\n";
        return true;
    }

    std::cout << "Checksum is wrong\n";
        return false;


}

void ProcessRequest::getFileInfo(GetFile* msg, SendFileInfo* reply){

    std::cout << "Filename is: " << msg->filename << "\n";

    // Set the data that won't change type to sending info on a packet
    memcpy(reply->type, ServerResponses::packetinfo, PACKET_REQ_LENGTH);
    memcpy(reply->filename, msg->filename, FILENAME_LENGTH);
    
    file_manager.fileChunkCount(msg->filename, reply->packet_count);

    uint32_t final_chunk_size = file_manager.getLastChunkSize();

    memcpy(reply->last_chunk_size, &final_chunk_size, PACKET_ID_LENGTH);

}

void ProcessRequest::getPacket(GetIndex* msg, SendFileData* reply){

    std::cout << "Client is requesting a packet from: " << msg->filename << "\n";

    memcpy(reply->type, ServerResponses::packetinfo, PACKET_REQ_LENGTH);
    

    //memcpy(reply->filename, msg->filename, FILENAME_LENGTH);
    //reply->packet_id

    // If the packet doesn't exist reply with empty filename and data
    if(!file_manager.loadPacket(msg->filename, reply->data, msg->index)){
        return;
    }

    memcpy(reply->filename, msg->filename, FILENAME_LENGTH);
    memcpy(reply->packet_id, msg->index, PACKET_ID_LENGTH);

}