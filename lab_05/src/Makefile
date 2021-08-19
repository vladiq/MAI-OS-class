CC = gcc
FLAGS = -std=c99 -w -O2 -Wextra -Werror -Wall -Wno-sign-compare -pedantic

all: run

run: libqueue.so staticmain.o dynamicmain.o
	$(CC) $(FLAGS) -o run-stat staticmain.o -L. -lqueue -Wl,-rpath,. 
	$(CC) $(FLAGS) -o run-dyn dynamicmain.o -ldl

staticmain.o: staticmain.c
	$(CC) -c $(FLAGS) staticmain.c

dynamicmain.o: dynamicmain.c
	$(CC) -c $(FLAGS) dynamicmain.c

libqueue.so: queue.o
	$(CC) $(FLAGS) -shared -o libqueue.so queue.o

queue.o: queue.c
	$(CC) -c $(FLAGS) queue.c

clean:
	rm -f *.o run-stat run-dyn *.so 
