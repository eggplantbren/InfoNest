CC = g++
CFLAGS = -O3 -std=c++14 -Wall -Wextra -pedantic

default:
	$(CC) $(CFLAGS) -c main.cpp
	$(CC) $(CFLAGS) -c RNG.cpp
	$(CC) $(CFLAGS) -c Examples/Normal.cpp
	$(CC) -o main *.o
	rm -f *.o

