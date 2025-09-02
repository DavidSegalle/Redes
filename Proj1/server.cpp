// Server side implementation of UDP client-server model 
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "message_headers.hpp"
#include "process_request.hpp"
  
#define PORT     8080 
#define MAXLINE 1024

std::string respond(std::string msg){
    ProcessRequest processor;
    // The message has to be at least the request
    if(msg.length() <= 4){
        return std::string();
    }

    if(msg.substr(0, 4) == ClientRequests::getfile){
        std::cout << "getfile request received" << "\n";
        return processor.getFileInfo(msg);
    }

    else if(msg.substr(0, 4) == ClientRequests::getid){
        std::cout << "get nth packet request received" << "\n";
        processor.getPacket(msg);
    }

    else if(msg.substr(0, 4) == ClientRequests::getchecksum){
        std::cout << "get checksum request received" << "\n";
        processor.getChecksum(msg);
    }

    return std::string();
}

  
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
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
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len);
    
    buffer[n] = '\0'; // Adding for safety, not necessary 
    printf("Client : %s\n", buffer);
    std::string req(buffer);

    std::string reply = respond(req);

    sendto(sockfd, reply.c_str(), reply.length(),  
       MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
           len); 
    //const char *hello = "Hello from server"; 
    //sendto(sockfd, (const char *)hello, strlen(hello),  
    //    MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
    //        len); 
    std::cout<<"Reply sent."<<std::endl;  
      
    return 0; 
}