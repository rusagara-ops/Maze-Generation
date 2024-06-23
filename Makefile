all: mazegen

list.o: list.c list.h
	gcc -c list.c -o list.o

main.o: main.c main.h
	gcc -c main.c -o main.o

mazegen: list.o main.o
	gcc list.o main.o -o mazegen

clean: 
	rm -rf list.o main.o mazegen
