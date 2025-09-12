#include "process_request.hpp"

ProcessRequest::ProcessRequest(int client_socket){
    this->client_socket = client_socket;

    for(int i = 0; i < MSG_LENGTH; i++){
        this->request.raw_data[i] = 0;
        this->response.raw_data[i] = 0;
    }

}

ProcessRequest::~ProcessRequest(){

}

void ProcessRequest::mainLoop(){

    while(true){

        for(int i = 0; i < MSG_LENGTH; i++){
            this->request.raw_data[i] = 0;
            this->response.raw_data[i] = 0;
        }

        this->receiveBytes(this->request_type, PACKET_REQ_LENGTH);

        if(strncmp(this->request_type, ClientRequests::getfile, PACKET_REQ_LENGTH)){
            char filename[FILENAME_LENGTH];
            
            this->receiveBytes(filename, FILENAME_LENGTH);

            if(!file_manager.fileExists(filename)){
                // Responder com arquivo não existe
            }

            char response_type[PACKET_REQ_LENGTH];
            
            memcpy(response_type, ServerResponses::sendfile, PACKET_REQ_LENGTH);
            
            int size = file_manager.getFileSize();

            char* file = new char[size];

            file_manager.loadFile(file, size);

            char checksum[SHA256_DIGEST_LENGTH];
            file_manager.calculateSha(file, size, checksum);

            // Send to client
            send(this->client_socket,  &response_type,  PACKET_REQ_LENGTH,     0);
            send(this->client_socket,  &filename,       FILENAME_LENGTH,       0);
            send(this->client_socket,  (char*) &size,   4,                     0);
            send(this->client_socket,  file,            size,                  0);
            send(this->client_socket,  checksum,        SHA256_DIGEST_LENGTH,  0);

            delete file;

        }

        /*int received = 0;
        while(received < MSG_LENGTH){
            std::cout << "test\n";
            bytes_received = recv(this->client_socket, this->request.raw_data + received, MSG_LENGTH - received, 0);
            received += bytes_received;
        }

        if(!bytes_received){
            std::cout << "Client shutdown the connection\n";
            this->closeConnection();
            return;
        }
        else if (bytes_received != MSG_LENGTH){
            std::cout << "Received wrong sized packet, closing connection\n";
            this->closeConnection();
            return;
        }

        // Creating the response
        if(!memcmp(ClientRequests::getfile, this->request.type, PACKET_REQ_LENGTH)){
            std::cout << "Gathering file info\n";
            this->getFileInfo();
        }

        if(!memcmp(ClientRequests::getid, this->request.type, PACKET_REQ_LENGTH)){
            std::cout << "Gathering a file packet\n";
            this->getPacket();
        }

        */

        std::cout << "Full server message is:\n";
        std::cout.write(this->response.raw_data, MSG_LENGTH);

    }
}

void ProcessRequest::closeConnection(){
    close(this->client_socket);
    return;
}

void ProcessRequest::getPacket(){

}

void ProcessRequest::receiveBytes(char* data, int size){
    
    int received = 0;
    int bytes_received = 0;

    while(received < size){
        std::cout << "test\n";
        bytes_received = recv(this->client_socket, data + received, size - received, 0);
        received += bytes_received;
    }

}

/*void ProcessRequest::getFileInfo(){

    GetFile* msg = &this->request.get_file;
    SendFileInfo* reply = &this->response.send_file_info;

    std::cout << "Requested filename is: " << msg->filename << "\n";

    // Set the data that won't change type to sending info on a packet
    memcpy(reply->type, ServerResponses::packetinfo, PACKET_REQ_LENGTH);
    memcpy(reply->filename, msg->filename, FILENAME_LENGTH);
    
    if(!file_manager.fileExists(msg->filename)){
        std::cout << "Requested file does not exist\n";
        return;
    }

    std::cout << "Requested file exists, sending file information\n";

    int chunk_count = file_manager.getFileChunkCount(msg->filename);
    memcpy(reply->packet_count, &chunk_count, PACKET_ID_LENGTH);

    uint32_t final_chunk_size = file_manager.getLastChunkSize();
    memcpy(reply->last_chunk_size, &final_chunk_size, PACKET_ID_LENGTH);

}*/
