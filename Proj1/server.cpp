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

void respond(Message* req, Message* reply){

    // Server doesn't care about invalid requests, won't reply
    if(!processor.checkChecksum(req)){
        //return;
        // TODO: uncomment this return
    }

    if(!memcmp(ClientRequests::getfile, req->type, PACKET_REQ_LENGTH)){
        std::cout << "Gathering file info\n";
        processor.getFileInfo(&req->get_file, &reply->send_file_info);
    }

    if(!memcmp(ClientRequests::getid, req->type, PACKET_REQ_LENGTH)){
        std::cout << "Gathering a file packet\n";
        processor.getPacket(&req->get_index, &reply->send_file_data);
    }

    // Depois de montar a resposta colocar a checksum
    processor.setChecksum(reply);

    std::cout << "Full server message is:\n";
    std::cout.write(reply->raw_data, MSG_LENGTH);

}

  
int main() { 
    int sockfd; 
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
    
    Message req, reply;
    // Remember to clear the message
    for(uint i = 0; i < MSG_LENGTH; i++){
        req.raw_data[i] = '\0';
        reply.raw_data[i] = '\0';
    }
    
    while(true){

        for(uint i = 0; i < MSG_LENGTH; i++){
            reply.raw_data[i] = 0;
        }

        n = recvfrom(sockfd, (char *)req.raw_data, MSG_LENGTH, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);

        std::cout << "Received message of length: " << n << "\nWith the contents: ";
        std::cout.write(req.raw_data, MSG_LENGTH) << "\n";

        respond(&req, &reply);

        sendto(sockfd, &reply.raw_data, MSG_LENGTH,  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len);

        std::cout<<"Reply sent."<<std::endl;
    }  
    return 0; 
}