# Custom Protocol

The first 4 letter character is the type of message as defined in message_headers.hpp

- inva (for invalid request)

### First request contains

- getf (getfile)
- textfilename up to 64 characters

All in form 4 bytes for getf, 64 bytes for getftextfilename followed by '\0''\0''\0', checksum

### Server returns

- pinf (packet info)
- Number of packets that are required to send the data (0 if file does not exist)
- Size of last packet

All in form: 4 bytes with pinf 64 bytes for packet name 4 bytes for packet_count followed by '\0''\0''\0', checksum

Here the server must load the file into memory to avoid issues

### Client sends

- getI (getid)
- textfilename
- Requested packet ID

All in form: 4 bytes for geti 64 bytes for textfilename 4 bytes for packet id being requested, followed by '\0''\0', checksum

### Server returns

- pdat (packet data)
- Textfilename(0) if not in memory
- Packet Id
- File block

All in form: 4 bytes for pdat, 64 bytes for textfilename, 4 bytes for packet id, rest for data except last 4 for checksum.
