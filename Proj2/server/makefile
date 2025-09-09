CXX := g++
CXXFLAGS := -Wall -O2
LDFLAGS := -lcrypto -lssl   # OpenSSL libraries

# Find all cpp files in the directory
SRCS := $(wildcard *.cpp)

# Exclude files depending on the target
SERVER_SRCS := $(filter-out client.cpp,$(SRCS))

all: server

server: $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f server client *.o
