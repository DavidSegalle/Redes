// Client side implementation of UDP client-server model 
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <string.h>
using namespace std;

#define PORT     8080
#define MAXLINE 1024 
  
// Driver code 
int main() { 

    std::cout << "Insert the get request in the format: @IP_Servidor:Porta_Servidor/nome_do_arquivo.txt: ";

    std::string input, ip, port, msg;
        
    getline(std::cin, input); // Defaults to stop at newline

    stringstream ss(input);

    getline(ss, ip, ':');
    getline(ss, port, '/');
    getline(ss, msg); // Defaults to stop at newline

    cout << ip << " and " << port << " and " << msg << "\n";

    int sockfd;
    char buffer[MAXLINE]; 
    const char *hello = "Hello from client"; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n;
    socklen_t len; 
      
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    std::cout<<"Hello message sent."<<std::endl; 
          
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    buffer[n] = '\0'; 
    std::cout<<"Server :"<<buffer<<std::endl; 
  
    close(sockfd); 
    return 0; 
}