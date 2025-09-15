// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

#include "file_manager.hpp"
#include "network_client.hpp"

NetworkClient parse_input(){
    std::string input, ip, port, msg;
    // TODO: UNCOMMENT
    //getline(std::cin, input); // Defaults to stop at newline
    input = "127.0.0.1:8080/test.txt";
    
    std::stringstream ss(input);

    getline(ss, ip, ':');
    getline(ss, port, '/');
    getline(ss, msg);

    if(!(ip.length() && port.length() && msg.length())){
        std::cout << "Found empty value from\nIP: " << ip << "\nPort: " << port <<"\nfile: " << msg << std::endl;
        exit(EXIT_FAILURE);
    }

    return NetworkClient(ip, port, msg);
}

int main()
{

    /*FileManager fm;

    char text[] = "1234567890";
    char sha[32];

    std::cout << fm.checkSha(text, 10, sha) << "\n";

    fm.saveFile("test.txt", text, 10);*/

    NetworkClient connection = parse_input();

    connection.mainLoop();

    /*// creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

    // sending data
    const char* message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);

    // closing socket
    close(clientSocket);*/

    return 0;
}