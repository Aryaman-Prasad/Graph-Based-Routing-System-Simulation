#MAKEFILE FOR CREATING EXECUTABLES, SUBJECT TO CHANGE

#Makefile
SHELL := /bin/bash

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -Wall -Wextra -Wpedantic -std=c++20 -O3 -march=native -flto=auto -ffast-math -funroll-loops -DNDEBUG -pipe

# Source files
PH1_DIR = Phase-1
PH2_DIR = Phase-2

SOURCES1 = \
    $(PH1_DIR)/SampleDriver.cpp \
    $(PH1_DIR)/graphUtils.cpp \
    $(PH1_DIR)/knn_shortest_euclidean.cpp \
    $(PH1_DIR)/shortest_distance.cpp \
    $(PH1_DIR)/shortest_time.cpp \
    $(PH1_DIR)/knn_shortest_path.cpp

SOURCES2 = \
    $(PH2_DIR)/SampleDriver.cpp \
    $(PH2_DIR)/graphUtils.cpp \
    $(PH2_DIR)/approx_shortest_dist.cpp \
    $(PH2_DIR)/error_bound_dist.cpp \
    $(PH2_DIR)/shortest_distance.cpp \
    $(PH2_DIR)/k_shortest_paths.cpp \
    $(PH2_DIR)/ksp_heuristic.cpp \
    $(PH2_DIR)/p_path.cpp

# Object files
OBJECTS1 = $(SOURCES1:.cpp=.o)
OBJECTS2 = $(SOURCES2:.cpp=.o)

# Executable
EXEC1 = phase1 
EXEC2 = phase2

# HEADERS
HEADERS = $(wildcard Phase-1/*.hpp Phase-2/*.hpp)

# Default target
all: build

build: phase1 phase2 phase3

# Link object files to create the executable
phase1: $(OBJECTS1)
	$(CXX) $(CXXFLAGS) -o $(EXEC1) $(OBJECTS1)

phase2: $(OBJECTS2)
	$(CXX) $(CXXFLAGS) -o $(EXEC2) $(OBJECTS2)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable (Unix-like commands)
clean:
	rm -f $(OBJECTS1) $(EXEC1) $(OBJECTS2) $(EXEC2)
	find . -type f -name '*~'  -exec rm {} +
