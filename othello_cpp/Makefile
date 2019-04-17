### Othello makefile

# g++-7 -g -std=c++17 -march=native -O3 -Wall -Wextra -o othello src/*.cpp

CXX=g++
CXXFLAGS=-g -std=c++17 -march=native -O3 -Wall -Wextra
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=othello

all: $(EXECUTABLE)
  
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
