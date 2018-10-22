all: udplinktestclient.o
	gcc -o udplinktestclient udplinktestclient.o 

udplinktestclient: udplinktestclient.o
	gcc -o udplinktestclient udplinktestclient.o 

udplinktestclient.o: udplinktestclient.c
	gcc -c udplinktestclient.c 

clean:
	rm *.o udplinktestclient
