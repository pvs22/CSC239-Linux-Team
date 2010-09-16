default: main

main: main.o perfstats.o
	gcc -o main main.o perfstats.o

main.o: main.c perfstats.o
	gcc -g -c -W main.c 

perfstats.o: perfstats.c
	gcc -g -c -W perfstats.c

clean: 
	rm -rf *.o main