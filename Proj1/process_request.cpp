#include "process_request.hpp"

// Will return pinf\nnumber of packets\nlast_packet size

ProcessRequest::ProcessRequest() : 
loaded_textfile(){}

std:: string ProcessRequest::getFileInfo(std::string msg){

    std::string filename = msg.substr(5);

    std::cout << "Received a file request for: " << filename << "\n";    

    // Check if file exists, if so return adequate answer

    // Plasceholder
    return std::string();

}

std:: string ProcessRequest::getPacket(std::string msg){
    std::string req_data = msg.substr(5);

    std::stringstream ss(req_data);

    std::string filename, packet_id;

    getline(ss, filename, '\n');
    getline(ss, packet_id, '\n');

    std::cout << "Received a request for packet number " << packet_id << " for the file " << filename;

    // Se loaded_textfile existir e for grande o suficiente pra o ID especificado Pegar o loaded_textfile e retornar o bloco adequado

    // Plasceholder
    return std::string();
}

std:: string ProcessRequest::getChecksum(std::string msg){


    std::string filename = msg.substr(5);

    // Check if file exists and if so calculate checksum
    // Plasceholder
    return std::string();
}