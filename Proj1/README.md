# Custom Protocol

### First request contains

- textfilename

### Server returns

- Number of packets that are required to send the data (0 if file does not exist)
- Size of last packet

All in form: packet_count'\n'last_size'\n'
Here the server must load the file into memory to avoid issues

### Client sends

- textfilename
- Requested packet ID

All in form: textfilename'\n'number'\n'

### Server returns

- Textfilename(0) if not in memory
- Packet Id
- File block

All in form: textfilename'\n'ID'\n'data. No marking \n so there's an extra bit, for the final packet only consider length

###

Client sends

- Textfilename
- Checksum request

All in form: textfilename'\n'check_req'\n'

Unload file from memory
