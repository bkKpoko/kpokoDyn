#!/bin/bash
CXX = g++
CXXFLAGS = -std=gnu++11 -Wall -Wextra
INCLUDES = -I /usr/include/panda3d/ -I /usr/include/eigen3/
LDFLAGS = -L /usr/lib/x86_64-linux-gnu/panda3d
LIBS = -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3direct

SRC = main.cxx
# SRC = test.cpp
TARGET = panda_app

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

install:
	pathcelf --set-rpath '/usr/lib/x86_64-linux-gnu/panda3d' $(TARGET)

rebuild:
	clean all

.PHONY: all clean rebuild install
# g++ -c main.cxx -o main.o  
# g++ main.o -o main  
