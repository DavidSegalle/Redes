CXX := g++
CXXFLAGS := -Wall -O2

# Find all cpp files in the directory
SRCS := $(wildcard *.cpp)

# Exclude files depending on the target
CLIENT_SRCS := $(filter-out server.cpp,$(SRCS))
all: client

client: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $(@).o

clean:
	rm -f server client *.o
