#include "network_client.hpp"

NetworkClient::NetworkClient(std::string ip, std::string port, std::string msg){

    

    this->ip = ip;
    this->port = port;

    for(uint32_t i = 0; i < FILENAME_LENGTH; i++){
        this->filename[i] = '\0';
    }

    for(uint32_t i = 0; i < msg.size() && i < FILENAME_LENGTH; i++){
        this->filename[i] = msg.c_str()[i];
    }

    std::cout << msg << std::endl;
    std::cout << "Connecting to:" << this->ip << " " << this->port << "\nAnd requesting: ";
    std::cout.write(this->filename, FILENAME_LENGTH);
    std::cout << "\n";

  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    uint16_t port_int(std::stoi(this->port));
    servaddr.sin_port = htons(port_int); 
    
    if (inet_pton(AF_INET, this->ip.data(), &servaddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    struct timeval tv;
    tv.tv_sec = 1;  // timeout in seconds
    tv.tv_usec = 0; // and microseconds
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt failed");
    }

}

NetworkClient::~NetworkClient(){
    delete this->file_manager;
    close(this->sockfd);
    
}

void NetworkClient::generateFileRequest(Message* msg, Message* response){
    
    memcpy(msg->type, "getf", PACKET_REQ_LENGTH);
    strncpy(msg->get_file.filename, this->filename, FILENAME_LENGTH);
    
    msg->get_file.filename[FILENAME_LENGTH - 1] = '\0'; // Just making sure
    
    this->sendMsg(msg, response);
    
    // Is server sending what I want?
    if(memcmp(ServerResponses::packetinfo, response->type, PACKET_REQ_LENGTH)){
    
        std::cout << "Server did not reply what was requested\n";
        exit(1);

    }

    SendFileInfo* file_info = &response->send_file_info;
    
    std::cout << "Server replied with packet information\n";

    if(memcmp(this->filename, file_info->filename, FILENAME_LENGTH)){
        std::cout << "Server replying with the wrong file\n";
        exit(1);
    }

    uint32_t total_chunk_count = 0;

    memcpy(&total_chunk_count, file_info->packet_count, PACKET_ID_LENGTH);

    if(!total_chunk_count){
        std::cout << "Requested file does not exist\n";
        exit(1);
    }

    uint32_t last_chunk_size = 0;

    memcpy(&last_chunk_size, file_info->last_chunk_size, PACKET_ID_LENGTH);

    this->file_manager = new FileManager(this->filename, total_chunk_count, last_chunk_size);

}

void NetworkClient::requestFile(Message* msg, Message* response){

    memcpy(msg->type, "geti", PACKET_REQ_LENGTH);

    GetIndex* get_index = &msg->get_index;
    strncpy(get_index->filename, this->filename, FILENAME_LENGTH);

    
    uint32_t index = 0;
    while(!this->file_manager->complete()){
        index = this->file_manager->getNextEmpty();

        memcpy(get_index->index, &index, PACKET_REQ_LENGTH);

        std::cout << "Requesting " << index << " packet\n";

        this->sendMsg(msg, response);

        char* data = response->send_file_data.data;
        uint32_t index = 0;
        
        memcpy(&index, response->send_file_data.packet_id, PACKET_ID_LENGTH);

        this->file_manager->savePacket(data, index);

    }

    this->file_manager->saveFile();

    // Save file

}

void NetworkClient::sendMsg(Message* message, Message* response){

    this->setChecksum(message);

    socklen_t len;
    int n;

    for(int i = 0; i < 5; i++){

        sendto(sockfd, message, MSG_LENGTH, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
        
        n = recvfrom(sockfd, response, MSG_LENGTH, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);

        if (n < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                std::cout << "No reply from server\n";
            } else {
                perror("recvfrom error");
            }
        }
        else if(!this->checkChecksum(response)){
            std::cout << "Reply received but checksum does not match\n";
        }
        else{
            std::cout << "Reply received with correct checksum\n";
            return;
        }
    }
    std::cout << "Can't communicate to server\n";
    exit(1);
}

void NetworkClient::setChecksum(Message* msg){

    uint32_t checksum = 0;

    for(uint i = 0; i < MSG_LENGTH - CHECKSUM_LENGTH; i++){
        checksum += msg->raw_data[i];
    }

    memcpy(&(msg->raw_data[MSG_LENGTH - CHECKSUM_LENGTH]), &checksum, PACKET_ID_LENGTH);

}

bool NetworkClient::checkChecksum(Message* msg){

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