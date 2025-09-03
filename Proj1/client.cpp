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

#define PORT     8080
#define MAXLINE 1024 
  
// Driver code 
int main() { 

    std::cout << "Insert the get request in the format: IP_Servidor:Porta_Servidor/nome_do_arquivo.txt: ";

    std::string input, ip, port, packet, msg;
    // TODO: UNCOMMENT
    //getline(std::cin, input); // Defaults to stop at newline
    input = "127.0.0.1:8080/test.txt";
    stringstream ss(input);

    packet += "getf\n";

    getline(ss, ip, ':');
    getline(ss, port, '/');
    getline(ss, msg); // Defaults to stop at newline

    packet += msg;

    if(!(ip.length() && port.length() && msg.length())){
        std::cout << "Found empty value from\nIP: " << ip << "\nPort: " << port <<"\nfile: " << msg << std::endl;
        exit(EXIT_FAILURE);
    }

    cout << "Connecting to:" << ip << " " << port << " " << msg << "\n";

    int sockfd;
    
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    uint16_t port_int(std::stoi(port));
    servaddr.sin_port = htons(port_int); 
    
    if (inet_pton(AF_INET, ip.data(), &servaddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    int n;
    socklen_t len; 
    
    char buffer[MAXLINE];

    for(int i = 0; i < MAXLINE; i++){
        buffer[i] = '\0';
    }

    //packet = "geti\ntest.txt\n0\n";

    sendto(sockfd, (const char *)packet.data(), packet.length(), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    std::cout<<"Request for the file has been sent."<<std::endl; 
          
    // Colocar timeout aqui
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    buffer[n] = '\0'; 
    std::cout<<"Server :"<<buffer<<std::endl;

    for(int i = 0; i < MAXLINE; i++){
        buffer[i] = '\0';
    }
    packet = "geti\ntest.txt\n0\n";
    sendto(sockfd, (const char *)packet.data(), packet.length(), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    std::cout<<"Request for the packet has been sent."<<std::endl; 
          
    // Colocar timeout aqui
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    buffer[n] = '\0';
    std::cout<<"Server :"<<buffer<<std::endl;
  
    close(sockfd); 
    return 0; 
}