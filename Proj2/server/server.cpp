// C++ program to show the example of server application in
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <memory>

#include "process_request.hpp"

using namespace std;

int main()
{

    std::vector<std::shared_ptr<ProcessRequest>> connections;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Todos os ips

    bind(server_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    
    listen(server_socket, 5); // Queue to server can be up to 5 long

    std::cout << "Server listening on port 8080\n";

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cout << "Client failed\n";
            continue;
        }

        cout << "Client with id: " << client_socket << " is now connected\n";

        std::shared_ptr<ProcessRequest> pr = std::make_shared<ProcessRequest>(client_socket);
        
        connections.push_back(pr);

        thread t(&ProcessRequest::mainLoop, pr);
        t.detach(); // run independently
    }

    

    close(server_socket);

    return 0;

    /*// creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    // accepting connection request
    int clientSocket
        = accept(serverSocket, nullptr, nullptr);

    // recieving data
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from client: " << buffer
              << endl;

    // closing the socket.
    close(serverSocket);

    return 0;*/
}