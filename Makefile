CC=g++
BIN=main.bin

all: main.cpp fat.cpp io.cpp dir.cpp
	$(CC) -o $(BIN) $^

run: all
	sudo ./$(BIN)