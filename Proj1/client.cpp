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

#include "message_headers.hpp"
#include "process_request.hpp"

#define PORT     8080
#define MAXLINE 1024 

typedef struct {
    std::string ip, port, msg;
}ConnectionInfo;

typedef struct {

    char filename[FILENAME_LENGTH];
    char packet_count[PACKET_ID_LENGTH];
    char last_chunk_size[PACKET_ID_LENGTH];

}FileInformation;

ProcessRequest processor;


ConnectionInfo parse_input(){
    std::string input;
    // TODO: UNCOMMENT
    //getline(std::cin, input); // Defaults to stop at newline
    input = "127.0.0.1:8080/test.txt";
    
    stringstream ss(input);

    ConnectionInfo connection_info;

    getline(ss, connection_info.ip, ':');
    getline(ss, connection_info.port, '/');
    getline(ss, connection_info.msg);

    return connection_info;
}

void generate_file_request(Message* msg, std::string filename){
    strncpy(msg->type, "getf", PACKET_REQ_LENGTH);
    strncpy(msg->get_file.filename, filename.c_str(), FILENAME_LENGTH);
    msg->get_file.filename[FILENAME_LENGTH - 1] = '\0'; // Just making sure
    // Falta calcular e colocar a checksum aqui no message.get_file.checksum

    processor.setChecksum(msg);
}

bool send_msg(int sockfd, Message* message, Message* response, sockaddr_in* servaddr){
    
    socklen_t len;
    int n;

    for(int i = 0; i < 5; i++){

        sendto(sockfd, message, MSG_LENGTH, MSG_CONFIRM, (const struct sockaddr *) servaddr, sizeof(*servaddr)); 
        
        n = recvfrom(sockfd, response, MSG_LENGTH, MSG_WAITALL, (struct sockaddr *) servaddr, &len);

        if (n < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                std::cout << "No reply from server\n";
            } else {
                perror("recvfrom error");
            }
        }
        else if(!processor.checkChecksum(response)){
            std::cout << "Reply received but checksum does not match\n";
        }
        else{
            std::cout << "Reply received with correct checksum\n";
            return true;
        }
    }
    return false;
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
        return false;
    }
}

// Driver code 
int main() { 

    std::cout << "Insert the get request in the format: IP_Servidor:Porta_Servidor/nome_do_arquivo.txt: ";

    /*std::string input, ip, port, msg;
    // TODO: UNCOMMENT
    //getline(std::cin, input); // Defaults to stop at newline
    input = "127.0.0.1:8080/test.txt";
    stringstream ss(input);

    getline(ss, ip, ':');
    getline(ss, port, '/');
    getline(ss, msg); // Defaults to stop at newline*/

    ConnectionInfo connection = parse_input();

    std::cout << connection.msg << "\n";

    Message message, response;

    // Remember to clear the message
    for(uint i = 0; i < MSG_LENGTH; i++){
        message.raw_data[i] = '\0';
    }

    generate_file_request(&message, connection.msg);

    
    if(!(connection.ip.length() && connection.port.length() && connection.msg.length())){
        std::cout << "Found empty value from\nIP: " << connection.ip << "\nPort: " << connection.port <<"\nfile: " << connection.msg << std::endl;
        exit(EXIT_FAILURE);
    }

    cout << "Connecting to:" << connection.ip << " " << connection.port << " " << connection.msg << "\n";

    int sockfd;
    struct sockaddr_in servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    uint16_t port_int(std::stoi(connection.port));
    servaddr.sin_port = htons(port_int); 
    
    if (inet_pton(AF_INET, connection.ip.data(), &servaddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    
    
    std::cout<<"Request for the file has been sent."<<std::endl; 
          
    // Client has a 1 second timeout on waiting for replies
    struct timeval tv;
    tv.tv_sec = 1;  // timeout in seconds
    tv.tv_usec = 0; // and microseconds
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt failed");
    }

    if(!send_msg(sockfd, &message, &response, &servaddr)){
        std::cout << "Got no reply from server, aborting\n";
        return 0; 
    }

    FileInformation data;

    if(!set_file_data(&response, &data)){
        std::cout << "File does not exist\n";
        return 0;
    }

    // Insert function to call for all the packets of the file

    close(sockfd);

    return 0; 
}