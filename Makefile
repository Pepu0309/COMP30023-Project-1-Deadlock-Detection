detect: detect.o node.o
	gcc -Wall -o detect detect.o -g node.o

detect.o: detect.c detect.h
	gcc -Wall -o detect.o -c detect.c -g

node.o: node.c node.h
	gcc -Wall -o node.o -c node.c -g