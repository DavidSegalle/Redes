#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_manager.hpp"
#include "../message_headers.hpp"

class NetworkClient{

public:

    NetworkClient(std::string ip, std::string port, std::string msg);
    ~NetworkClient();

    void mainLoop();

private:


};