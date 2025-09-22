CC=g++
CFLAGS=-Wall -g -O3

all: cifra decifra

cifra: cifra.cpp
	$(CC) $(CFLAGS) -o cifra cifra.cpp

decifra: decifra.cpp
	$(CC) $(CFLAGS) -o decifra decifra.cpp

aesEncrypt: aesEncrypt.cpp
	$(CC) $(CFLAGS) -o aesEncrypt aesEncrypt.cpp -lssl -lcrypto


clean:
	rm -f *.o cifra decifra aesEncrypt aesDecrypt