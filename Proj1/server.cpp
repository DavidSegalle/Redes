// Server side implementation of UDP client-server model 
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string> 
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "message_headers.hpp"
#include "process_request.hpp"
  
#define PORT     8080 

ProcessRequest processor;

void respond(char* msg, char* reply){

    char req_type[PACKET_REQ_LENGTH];

    strncpy(req_type, msg, PACKET_REQ_LENGTH);

    std::cout << "Received a request of type: " << req_type << "\n";

    if(!memcmp(ClientRequests::getfile, req_type, PACKET_REQ_LENGTH)){
        processor.getFileInfo(msg, reply);
    }

    else if(!memcmp(ClientRequests::getid, req_type, PACKET_REQ_LENGTH)){
        //processor.getPacket(msg, reply);
    }

    else{
        strcpy(reply, ServerResponses::errorreply);
    }

}

  
int main() { 
    int sockfd; 
    char buffer[MSG_LENGTH]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_port = htons(PORT); 

    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
  
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
      
    socklen_t len;
    int n; 
  
    len = sizeof(cliaddr);  //len is value/result 
  
    // Receives request
    
    while(true){

        for(int i = 0; i < MSG_LENGTH; i++){
            buffer[i] = 0;
        }

        n = recvfrom(sockfd, (char *)buffer, MSG_LENGTH,  
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                    &len);
        
        printf("Client sent: %s\n", buffer);

        char reply[MSG_LENGTH];
        for(int i = 0; i < MSG_LENGTH; i++){
            reply[i] = 0;
        }

        respond(buffer, reply);

        sendto(sockfd, reply, MSG_LENGTH,  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 

        std::cout<<"Reply sent."<<std::endl;  
    }  
    return 0; 
}