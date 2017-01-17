CC = g++
CFLAGS = -O3 -std=c++14 -Wall -Wextra -pedantic

default: lib main tests
	rm -f *.o

lib:
	$(CC) $(CFLAGS) -c RNG.cpp
	$(CC) $(CFLAGS) -c Utils.cpp
	$(CC) $(CFLAGS) -c examples/Normal.cpp
	$(CC) $(CFLAGS) -c examples/Sinusoid.cpp
	ar rcs libinfonest.a Normal.o RNG.o Sinusoid.o Utils.o

main: lib
	$(CC) $(CFLAGS) -c main.cpp
	$(CC) -o main main.o -linfonest

tests: lib
	$(CC) $(CFLAGS) -c tests/normal.cpp
	$(CC) $(CFLAGS) -c tests/sinusoid.cpp
	$(CC) -o tests/normal normal.o -linfonest
	$(CC) -o tests/sinusoid sinusoid.o -linfonest

