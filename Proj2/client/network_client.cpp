#include "network_client.hpp"

NetworkClient::NetworkClient(std::string ip, std::string port, std::string msg){

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(stoi(port));
    serverAddress.sin_addr.s_addr = stoi(ip);
    
    // sending connection request
    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))){
        std::cout << "Coult not connect to server, failing\n";
        exit(1);
    }

    // sending data
    char message[17];
    strcpy(message,"getf");
    int size = 9;
    
    char size_char[4];
    memcpy(size_char, &size, 4);

    for(int i = 0; i < 4; i++){
        message[i + 4] = size_char[i];
    }
    
    strncpy(message + 8,"test.txt", 9);

    std::cout.write(message, 16);
    std::cout << "\n";
    
    send(clientSocket, message, 17, 0);

    char buffer[1500];
    for(int i = 0; i < 1500; i++){
        buffer[i] = 0;
    }

    recv(clientSocket, buffer, 1500, 0);

    std::cout.write(buffer, 1500);
    
    // closing socket
    close(clientSocket);

}

NetworkClient::~NetworkClient(){

}

void NetworkClient::mainLoop(){

}
