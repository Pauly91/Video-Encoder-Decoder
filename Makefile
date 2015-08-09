all:
	gcc imageBlocking.c -Wall -g -O3 -o imageBLocking

clean:
	rm imageBLocking
