CC=g++
CFLAGS=-Wall -g -O3

all: cifra decifra

cifra: cifra.o
	$(CC) $(CFLAGS) -o cifra cifra.o

cifra.o: cifra.cpp
	$(CC) $(CFLAGS) -c cifra.cpp

decifra: decifra.o
	$(CC) $(CFLAGS) -o decifra decifra.o

decifra.o: decifra.cpp
	$(CC) $(CFLAGS) -c decifra.cpp

clean:
	rm -f *.o cifra decifra