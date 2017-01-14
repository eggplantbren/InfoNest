CC = g++
CFLAGS = -O3 -std=c++14 -Wall -Wextra -pedantic

default:
	$(CC) $(CFLAGS) -c RNG.cpp
	ar rcs libinfonest.a *.o
	rm -f *.o

