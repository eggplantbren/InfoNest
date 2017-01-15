CC = g++
CFLAGS = -O3 -std=c++14 -Wall -Wextra -pedantic

default:
	$(CC) $(CFLAGS) -c main.cpp
	$(CC) $(CFLAGS) -c RNG.cpp
	$(CC) $(CFLAGS) -c Utils.cpp
	$(CC) $(CFLAGS) -c examples/Normal.cpp
	$(CC) $(CFLAGS) -c examples/Sinusoid.cpp
	$(CC) -o main *.o
	rm -f *.o

