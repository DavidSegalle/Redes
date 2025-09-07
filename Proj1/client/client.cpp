// Client side implementation of UDP client-server model 
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
using namespace std;

#include "../message_headers.hpp"
#include "network_client.hpp"

#define PORT     8080
#define MAXLINE 1024 

typedef struct {

    char filename[FILENAME_LENGTH];
    char packet_count[PACKET_ID_LENGTH];
    char last_chunk_size[PACKET_ID_LENGTH];

}FileInformation;

NetworkClient parse_input(){
    std::string input, ip, port, msg;
    // TODO: UNCOMMENT
    //getline(std::cin, input); // Defaults to stop at newline
    input = "127.0.0.1:8080/test.txt";
    
    stringstream ss(input);

    getline(ss, ip, ':');
    getline(ss, port, '/');
    getline(ss, msg);

    if(!(ip.length() && port.length() && msg.length())){
        std::cout << "Found empty value from\nIP: " << ip << "\nPort: " << port <<"\nfile: " << msg << std::endl;
        exit(EXIT_FAILURE);
    }

    return NetworkClient(ip, port, msg);
}

bool set_file_data(Message* srv_msg, FileInformation* data){

    if(!memcmp(ServerResponses::packetinfo, srv_msg->type, PACKET_REQ_LENGTH)){
        std::cout << "Received file information\n";

        for(uint i = 0; i < FILENAME_LENGTH; i++){
            if(srv_msg->send_file_info.filename[i]){
                std::cout << "Requested file does exist, saving values\n";
                return true;
            }
        }

        std::cout << "Requested file does not exist\n";
    }
    return false;
}

// Driver code 
int main() { 

    std::cout << "Insert the get request in the format: IP_Servidor:Porta_Servidor/nome_do_arquivo.txt: \n";

    NetworkClient connection = parse_input();

    Message message, response;

    // Remember to clear the message
    for(uint i = 0; i < MSG_LENGTH; i++){
        message.raw_data[i] = '\0';
        response.raw_data[i] = '\0';
    }

    connection.generateFileRequest(&message, &response);

    connection.requestFile(&message, &response);

    return 0; 
}