#pragma once

#define MSG_LENGTH 1024
#define PACKET_REQ_LENGTH 4
#define CHECKSUM_LENGTH 4
#define FILENAME_LENGTH 64
#define DATA_LENGTH 952

namespace ClientRequests {
    inline constexpr const char* getfile = "getf";
    inline constexpr const char* getid = "geti";
}

namespace ServerResponses {
    inline constexpr const char* packetinfo = "pinf";
    inline constexpr const char* packetdata = "pdat";
    inline constexpr const char* errorreply = "inva";
}