# Custom Protocol

## Error handling

### Server side

If checksum is invalid server simply won't respond.

If a request is made for an invalid filename the server will respond with a message with empty filename.

If a request is made for an invalid index of a file (file isn't big enough for the index) server will respond with empty filename and data fields.

### Client side

Not implemented.

## Message

All messages are 1024 bytes long filled with padding, all struct declarations are found in [this file](message_headers.hpp) and usage comes from typecasting the Message Union pointer.

### Get File

Client sends this request. It is formatted as follows:

| Description       | Size     |
|-------------------|----------|
| file_request type | 4 bytes  |
| filename          | 64 bytes |
| checksum          | 4 bytes  |
| padding           |          |

### Get index packet from file

Client sends this request. It is formatted as follows:

| Description | Size     |
|-------------|----------|
| getid type  | 4 bytes  |
| filename    | 64 bytes |
| index       |  4 bytes |
| padding     |          |
| checksum    | 4 bytes  |

### Send file info

Server sends this request. It is formatted as follows:

| Description     | Size     |
|-------------    |----------|
| packetinfo type | 4 bytes  |
| filename        | 64 bytes |
| packet_count    | 4 bytes  |
| last_chunk_size | 4 bytes  |
| padding         |          |
| checksum        | 4 bytes  |

### Send file data

Server sends this request. It is formatted as follows:

| Description      | Size      |
|-------------     |---------- |
| packetdata type  | 4 bytes   |
| filename         | 64 bytes  |
| index            | 4 bytes   |
| data             | 948 bytes |
| padding if final |           |
| checksum         | 4 bytes   |
