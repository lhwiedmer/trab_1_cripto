CC=g++
CFLAGS=-Wall -g -O3

all: cifra decifra aesDecrypt aesEncrypt

cifra: cifra.o commons.o
	$(CC) $(CFLAGS) -o cifra cifra.o commons.o

cifra.o: cifra.cpp commons.hpp
	$(CC) $(CFLAGS) -c cifra.cpp

decifra: decifra.o commons.o
	$(CC) $(CFLAGS) -o decifra decifra.o commons.o

decifra.o: decifra.cpp commons.hpp
	$(CC) $(CFLAGS) -c decifra.cpp

commons.o: commons.cpp commons.hpp
	$(CC) $(CFLAGS) -c commons.cpp

aesEncrypt: aesEncrypt.cpp
	$(CC) $(CFLAGS) -o aesEncrypt aesEncrypt.cpp -lssl -lcrypto

aesDecrypt: aesDecrypt.cpp
	$(CC) $(CFLAGS) -o aesDecrypt aesDecrypt.cpp -lssl -lcrypto

clean:
	rm -f *.o cifra decifra aesEncrypt aesDecrypt