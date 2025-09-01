CXX := g++
CXXFLAGS := -Wall -O2

# Find all cpp files in the directory
SRCS := $(wildcard *.cpp)

# Exclude files depending on the target
SERVER_SRCS := $(filter-out client.cpp,$(SRCS))
CLIENT_SRCS := $(filter-out server.cpp,$(SRCS))

all: server client

server: $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

client: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f server client *.o
