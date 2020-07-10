project1: main.o scheduler.o
	gcc main.o scheduler.o -o project1

main.o:
	gcc -c main.c -o main.o

scheduler.o:
	gcc -c scheduler.c -o scheduler.o

clean:
	rm -f main.o scheduler.o project1

run:
	./project1
