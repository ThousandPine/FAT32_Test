CC=g++

all: main.cpp fat.cpp io.cpp
	$(CC) -o main.bin $^