CC = gcc
CFLAGS = -Wall

all: itinerary

itinerary: main.o itinerary.o
	$(CC) $(CFLAGS) main.o itinerary.o -o itinerary

main.o: main.c itinerary.h
	$(CC) $(CFLAGS) -c main.c

itinerary.o: itinerary.c itinerary.h
	$(CC) $(CFLAGS) -c itinerary.c

clean:
	rm -f *.o itinerary
