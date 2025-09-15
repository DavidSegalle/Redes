#include "network_client.hpp"

NetworkClient::NetworkClient(std::string ip, std::string port, std::string msg){

    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(stoi(port));
    serverAddress.sin_addr.s_addr = stoi(ip);
    
    // sending connection request
    if(connect(this->server_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))){
        std::cout << "Coult not connect to server, failing\n";
        exit(1);
    }

    // sending data
    
    
    // closing socket
    

}

NetworkClient::~NetworkClient(){
    close(this->server_socket);
}

void NetworkClient::mainLoop(){

    char placeholder_name[] = "test.txt";
    FileName filename;
    filename.size = 9;
    filename.name = new char[filename.size];

    for(int i = 0; i < filename.size; i++){
        filename.name[i] = placeholder_name[i];
    }

    request_file(&filename);

    delete filename.name;

}

bool NetworkClient::request_file(FileName* filename){
    
    char* request;
    int request_size = filename->size + 8; // 4 for request type and 4 for name length

    request = new char[request_size];

    strcpy(request,ClientRequests::getfile);

    memcpy(request + 4, &filename->size, 4);

    strcpy(request + 8, filename->name);

    std::cout.write(request, request_size);
    std::cout << "\n";

    send(this->server_socket, request, request_size, 0);

    delete request;

    char req_type[PACKET_REQ_LENGTH];
    this->receiveBytes(req_type, PACKET_REQ_LENGTH);

    if(!strncmp(req_type, ServerResponses::fileerror, PACKET_REQ_LENGTH)){
        std::cout << "Server said file does not exist\n";
        return false;
    }
    // Just in case
    else if(strncmp(req_type, ServerResponses::sendfile, PACKET_REQ_LENGTH)){
        std::cout << "Server is replying with garbage\n";
        return false;
    }

    // Remember to check the checksum
    char checksum[SHA256_DIGEST_LENGTH];
    this->receiveBytes(checksum, SHA256_DIGEST_LENGTH);

    int file_size = 0;
    char file_size_char[NUMBER_LENGTH];

    this->receiveBytes(file_size_char, NUMBER_LENGTH);

    memcpy(&file_size, file_size_char, NUMBER_LENGTH);

    std::cout << "The received file contains: " << file_size << " bytes\n";

    char* file;

    file = new char[file_size];

    this->receiveBytes(file, file_size);

    this->file_manager.saveFile(filename, file, file_size);

    delete file;

    return true;
    /*std::cout.write(message, request_size);
    std::cout << "\n";
    
    send(this->socket, message, 17, 0);

    char* buffer;
    buffer = new char[3930];
    for(int i = 0; i < 3930; i++){
        buffer[i] = 0;
    }

    int sizes = 3930;

    int received = 0;
    int bytes_received = 0;
    std::cout << "Receiving bytes:\n";

    while(received < sizes){
        bytes_received = recv(this->socket, buffer + received, sizes - received, 0);
        received += bytes_received;
        if(bytes_received <= 0){
            close(this->socket);
            return ;
            std::cout << received << "\n";
        }
        std::cout.write(buffer, sizes);
        std::cout << "\n";
    }
    std::cout << "Done receiving bytes\n";
    return ;

    std::cout.write(buffer, 3930);
    delete buffer;*/
}


bool NetworkClient::receiveBytes(char* data, int size){
    
    int received = 0;
    int bytes_received = 0;
    std::cout << "Receiving bytes:\n";

    while(received < size){
        bytes_received = recv(this->server_socket, data + received, size - received, 0);
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

bool NetworkClient::sendBytes(int sock, char* buf, int len){
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

void NetworkClient::closeConnection(){

    this->connection = false;
    std::cout << "Closing connection with " << this->server_socket << "\n";
    close(this->server_socket);
    return;

}
