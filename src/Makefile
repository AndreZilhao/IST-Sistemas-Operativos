# Makefile do Projecto

par-shell: list.o commandlinereader.o par-shell-terminal.o par-shell.o
	gcc -o par-shell list.o commandlinereader.o par-shell.o -lpthread
	gcc -o par-shell-terminal commandlinereader.o par-shell-terminal.o -lpthread

par-shell-terminal.o: par-shell-terminal.c commandlinereader.h
	gcc -Wall -g -c par-shell-terminal.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

list.o: list.c list.h
	gcc -Wall -g -c list.c

par-shell.o: par-shell.c list.h commandlinereader.h
	gcc -Wall -g -c par-shell.c

clean:
	rm -f *.o main par-shell par-shell-terminal par-shell-in *.txt pipe-out*
