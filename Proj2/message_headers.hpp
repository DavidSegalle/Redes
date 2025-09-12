#pragma once

constexpr int MSG_LENGTH        = 1024;
constexpr int PACKET_REQ_LENGTH = 4;
constexpr int CHECKSUM_LENGTH   = 4;
constexpr int PACKET_ID_LENGTH  = 4;
constexpr int FILENAME_LENGTH   = 64;

constexpr int DATA_LENGTH = MSG_LENGTH - PACKET_REQ_LENGTH - FILENAME_LENGTH - PACKET_ID_LENGTH;
/** 
Each request and response has an associated struct which occuppies 1024 bytes and is stored in a Union to be interpreted as needed.
 */

namespace ClientRequests {
    inline constexpr const char* getfile = "getf";
}

namespace ServerResponses {
    inline constexpr const char* sendfile = "senf";
}

// Ask the server for info on filename
typedef struct {

    char type[PACKET_REQ_LENGTH];
    char filename[FILENAME_LENGTH];

}GetFile;

// Ask the server for nth data packet of filename
typedef struct {

    char type[PACKET_REQ_LENGTH];
    char filename[FILENAME_LENGTH];
    char index[PACKET_ID_LENGTH];

}SendFile;

typedef union {

    char type[PACKET_REQ_LENGTH];

    GetFile get_file;
    SendFile send_file;

    char raw_data[MSG_LENGTH];
}Message;
