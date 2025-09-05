#pragma once

#define MSG_LENGTH 1024
#define PACKET_REQ_LENGTH 4
#define CHECKSUM_LENGTH 4
#define PACKET_ID_LENGTH 4
#define FILENAME_LENGTH 64
#define DATA_LENGTH MSG_LENGTH - PACKET_REQ_LENGTH - FILENAME_LENGTH - CHECKSUM_LENGTH

/** 
Each request and response has an associated struct which occuppies 1024 bytes and is stored in a Union to be interpreted as needed.
 */

namespace ClientRequests {
    inline constexpr const char* getfile = "getf";
    inline constexpr const char* getid = "geti";
}

namespace ServerResponses {
    inline constexpr const char* packetinfo = "pinf";
    inline constexpr const char* packetdata = "pdat";
    inline constexpr const char* errorreply = "inva";
}

// Ask the server for info on filename
typedef struct {

    char type[PACKET_REQ_LENGTH];
    char filename[FILENAME_LENGTH];
    char checksum[CHECKSUM_LENGTH];

}GetFile;

// Ask the server for nth data packet of filename
typedef struct {

    char type[PACKET_REQ_LENGTH];
    char filename[FILENAME_LENGTH];
    char index[PACKET_ID_LENGTH];
    char checksum[CHECKSUM_LENGTH];

}GetIndex;

// Server response with the number of packets that the file fits into
typedef struct {

    char type[PACKET_REQ_LENGTH];
    char filename[FILENAME_LENGTH];
    char packet_count[PACKET_ID_LENGTH];
    char checksum[CHECKSUM_LENGTH];

}SendFileInfo;

// Server response with the nth packet of data for filename
typedef struct {

    char type[PACKET_REQ_LENGTH];
    char filename[FILENAME_LENGTH];
    char packet_id[PACKET_ID_LENGTH];
    char data[DATA_LENGTH];
    char checksum[CHECKSUM_LENGTH];

}SendFileData;


typedef union {

    char type[PACKET_REQ_LENGTH];

    GetFile get_file;
    GetIndex get_index;

    SendFileInfo send_file_info;
    SendFileData send_file_data;

    char raw_data[MSG_LENGTH];
}Message;
