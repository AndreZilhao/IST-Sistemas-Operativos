# Makefile do Projecto

par-shell: list.o commandlinereader.o par-shell.o
	gcc -o par-shell list.o commandlinereader.o par-shell.o -lpthread

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

list.o: list.c list.h
	gcc -Wall -g -c list.c

par-shell.o: par-shell.c list.h commandlinereader.h
	gcc -Wall -g -c par-shell.c

clean:
	rm -f *.o main
