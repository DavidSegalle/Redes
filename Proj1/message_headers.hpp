#pragma once

#define MSG_LENGTH 1024
#define REQ_LENGTH 4

namespace ClientRequests {
    inline constexpr const char* getfile = "getf";
    inline constexpr const char* getid = "geti";
    inline constexpr const char* getchecksum = "getc";
}

namespace ServerRequests {
    inline constexpr const char* packetinfo = "pinf";
    inline constexpr const char* packetdata = "pdat";
    inline constexpr const char* sendchecktum = "setc";
}