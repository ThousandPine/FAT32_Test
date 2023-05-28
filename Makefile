CC=g++

all: main.cpp fat.cpp io.cpp dir.cpp
	$(CC) -o main.bin $^