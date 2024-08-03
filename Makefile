CC = gcc
CFLAGS = -Wall -std=c++20 -O3
LFLAGS = -lstdc++
SRCFILES = main.cc string_ranker.cc

all: build run

build: $(SRCFILES)
	$(CC) $(CFLAGS) $(LFLAGS) -o main $(SRCFILES)

run:
	./main

clean:
	rm main