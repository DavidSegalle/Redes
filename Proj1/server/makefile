CXX := g++
CXXFLAGS := -Wall -O2

# Find all cpp files in the directory
SRCS := $(wildcard *.cpp)

# Exclude files depending on the target
SERVER_SRCS := $(filter-out client.cpp,$(SRCS))
all: server

server: $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $(@).o

clean:
	rm -f server client *.o
