CC = g++
CFLAGS = -O3 -I. -std=c++14 -Wall -Wextra -pedantic

default: lib main tests
	rm -f *.o

lib:
	$(CC) $(CFLAGS) -c RNG.cpp
	$(CC) $(CFLAGS) -c Utils.cpp
	$(CC) $(CFLAGS) -c examples/GroupDiff.cpp
	$(CC) $(CFLAGS) -c examples/Normal.cpp
	$(CC) $(CFLAGS) -c examples/Sinusoid.cpp
	ar rcs libinfonest.a GroupDiff.o Normal.o RNG.o Sinusoid.o Utils.o

main: lib
	$(CC) $(CFLAGS) -c main.cpp
	$(CC) -L. -o main main.o -linfonest

tests: lib
	$(CC) $(CFLAGS) -c tests/groupdiff.cpp
	$(CC) $(CFLAGS) -c tests/normal.cpp
	$(CC) $(CFLAGS) -c tests/sinusoid.cpp
	$(CC) -L. -o tests/groupdiff groupdiff.o -linfonest
	$(CC) -L. -o tests/normal normal.o -linfonest
	$(CC) -L. -o tests/sinusoid sinusoid.o -linfonest

