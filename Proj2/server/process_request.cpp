#include "process_request.hpp"

ProcessRequest::ProcessRequest(int client_socket){
    this->client_socket = client_socket;
}

ProcessRequest::~ProcessRequest(){
    
}

void ProcessRequest::mainLoop(){

    while(true){

        if(!this->receiveBytes(this->request_type, PACKET_REQ_LENGTH)){
            std::cout << "What\n";
            return;
        }
        

        if(!strncmp(this->request_type, ClientRequests::getfile, NUMBER_LENGTH)){
            std::cout << "Received request for a file\n";
            
            char name_len[NUMBER_LENGTH];
            
            if(!this->receiveBytes(name_len, NUMBER_LENGTH)){
                return;
            }

            FileName filename;
            memcpy(&filename.size, name_len, NUMBER_LENGTH);

            filename.name = new char[filename.size];

            if(!this->receiveBytes(filename.name, filename.size)){
                return;
            }
            if(filename.name[filename.size - 1] != '\0'){
                std::cout << "received an invalid filename\n";
                this->closeConnection();
                delete filename.name;
                return;
            }
            
            std::cout << "Requested filename is: ";
            std::cout << filename.name;
            std::cout << "\n";

            if(!file_manager.fileExists(&filename)){
                // Responder com arquivo não existe
                
                // Limpar o buffer de recv

                delete filename.name;
                continue;
            }
            
            int size = file_manager.getFileSize();
            char* file = new char[size];
            file_manager.loadFile(file, size);

            char checksum[SHA256_DIGEST_LENGTH];
            file_manager.calculateSha(file, size, checksum);

            char response_type[PACKET_REQ_LENGTH];
            memcpy(response_type, ServerResponses::sendfile, PACKET_REQ_LENGTH);
            
            this->sendBytes(this->client_socket, response_type, PACKET_REQ_LENGTH);
            this->sendBytes(this->client_socket, checksum, SHA256_DIGEST_LENGTH);
            this->sendBytes(this->client_socket, reinterpret_cast<char*>(&size), NUMBER_LENGTH);
            this->sendBytes(this->client_socket, file, size);
            
            delete filename.name;
            delete file;

        }

        std::cout << "Full server message is:(still not printing)\n";

    }
}

void ProcessRequest::closeConnection(){

    this->connection = false;
    std::cout << "Closing connection with " << this->client_socket << "\n";
    close(this->client_socket);
    return;

}

void ProcessRequest::getPacket(){

}

bool ProcessRequest::receiveBytes(char* data, int size){
    
    int received = 0;
    int bytes_received = 0;
    std::cout << "Receiving bytes:\n";

    while(received < size){
        bytes_received = recv(this->client_socket, data + received, size - received, 0);
        received += bytes_received;
        if(bytes_received <= 0){
            this->closeConnection();
            return false;
            std::cout << received << "\n";
        }
        std::cout.write(data, size);
        std::cout << "\n";
    }
    std::cout << "Done receiving bytes\n";
    return true;
}

bool ProcessRequest::sendBytes(int sock, char* buf, int len){
    int total = 0;
    while (total < len) {
        std::cout << "I am here\n";
        int sent = send(sock, buf + total, len - total, 0);
        std::cout << "I am here\n";
        std::cout << sent << "\n";
        if (sent <= 0){
            return false;
        }
        
        total += sent;
    }
    return true;
}
