# Custom Protocol

The first 4 letter character is the type of message

### First request contains

- getf (getfile)
- textfilename

All in form getf'\n'textfilename'\n'

### Server returns

- pinf (packet info)
- Number of packets that are required to send the data (0 if file does not exist)
- Size of last packet

All in form: pin'\n'packet_count'\n'last_size'\n'
Here the server must load the file into memory to avoid issues

### Client sends

- getI (getid)
- textfilename
- Requested packet ID

All in form: getI'\n'textfilename'\n'number'\n' <- This \n will be converted to '\0' by the server

### Server returns

- pdat (packet data)
- Textfilename(0) if not in memory
- Packet Id
- File block

All in form: pdat'\n'textfilename'\n'ID'\n'data'\n'.

### Client sends

- getc (get checksum)
- Textfilename
- Checksum request

All in form: getc'\n'textfilename'\n'check_req'\n'

Unload file from memory
