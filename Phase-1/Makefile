#MAKEFILE FOR CREATING EXECUTABLES, SUBJECT TO CHANGE

#Makefile
SHELL := /bin/bash

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -Wall -std=c++20

# Source files
SOURCES = SampleDriver.cpp graphUtils.cpp shortest_distance.cpp shortest_time.cpp knn_shortest_euclidean.cpp knn_shortest_path.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable
EXEC = phase1

# Default target
all: build

build: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable (Unix-like commands)
clean:
	rm -f $(OBJECTS) $(EXEC)
	find . -type f -name '*~'  -exec rm {} +