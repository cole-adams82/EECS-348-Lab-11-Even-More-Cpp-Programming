# EECS 348 – Software Engineering I
# Lab 11: Even More C++ Programming
# File: Makefile
# Author: Cole Adams
#
# Description:
# 	Makefile to compile matrix operations program in main.cpp (lab 11 submission).
#
# Usage:
# 	make		– build lab11
#	make run	– build and run the program
#	make clean	– remove executables

# Compiler and flags.
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Targets.
TARGET = lab11
SRC = main.cpp

# Default target (builds program).
all: $(TARGET)

# Build rule.
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Run rule.
run: $(TARGET)
	./$(TARGET)

# Clean rule.
clean:
	rm -f $(TARGET)