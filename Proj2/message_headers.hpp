#pragma once

constexpr int PACKET_REQ_LENGTH = 4;
constexpr int NUMBER_LENGTH = 4;

typedef struct {

    char* name;
    int size;

}FileName;

/** 
Each request and response has an associated struct which occuppies 1024 bytes and is stored in a Union to be interpreted as needed.
 */

namespace ClientRequests {
    inline constexpr const char* getfile = "getf";
}

namespace ServerResponses {
    inline constexpr const char* sendfile = "senf";
    inline constexpr const char* fileerror = "fier";
}

