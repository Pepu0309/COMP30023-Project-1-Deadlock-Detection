detect: detect.o node.o hashtable.o array.o process_allocation.o
	gcc -Wall -o detect detect.o -g node.o hashtable.o array.o process_allocation.o

detect.o: detect.c detect.h
	gcc -Wall -o detect.o -c detect.c -g

node.o: node.c node.h
	gcc -Wall -o node.o -c node.c -g

hashtable.o: hashtable.c hashtable.h
	gcc -Wall -o hashtable.o -c hashtable.c -g

array.o: array.c array.h
	gcc -Wall -o array.o -c array.c -g

process_allocation.o: process_allocation.c process_allocation.h
	gcc -Wall -o process_allocation.o -c process_allocation.c -g

clean:
	rm -f *.o detect