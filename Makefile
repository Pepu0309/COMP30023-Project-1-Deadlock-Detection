detect: detect.o node.o hashtable.o
	gcc -Wall -o detect detect.o -g node.o hashtable.o

detect.o: detect.c detect.h
	gcc -Wall -o detect.o -c detect.c -g

node.o: node.c node.h
	gcc -Wall -o node.o -c node.c -g

hashtable.o: hashtable.c hashtable.h
	gcc -Wall -o hashtable.o -c hashtable.c -g

clean: detect
	rm -f *.o detect