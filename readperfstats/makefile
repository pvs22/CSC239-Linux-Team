default: main

main: main.o readperfstats.o
	gcc -o main main.o readperfstats.o

main.o: main.c readperfstats.o
	gcc -g -c -W main.c 

readperfstats.o: readperfstats.c
	gcc -g -c -W readperfstats.c

clean: 
	rm -rf *.o main