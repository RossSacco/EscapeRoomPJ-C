all: client server other

client: client.o strutture.o funzioniUtilita.o
		gcc -Wall client.o strutture.o funzioniUtilita.o -o client

server: server.o strutture.o funzioniUtilita.o
		gcc -Wall server.o strutture.o funzioniUtilita.o -o server

other: other.o strutture.o funzioniUtilita.o 
		gcc -Wall other.o strutture.o funzioniUtilita.o -o other

clean: 
		rm *.o client server other


strutture.o: strutture.c strutture.h
		gcc -Wall -c strutture.c -o strutture.o

funzioniUtilita.o: funzioniUtilita.c funzioniUtilita.h 
		gcc -Wall -c  funzioniUtilita.c -o funzioniUtilita.o