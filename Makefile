all:
	gcc imageBlocking.c -Wall -g -O3 -o imageBlocking

clean:
	rm imageBlocking
