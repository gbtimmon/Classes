
lib/mythread.a: obj/mythread.o
	ar rcs $@ $^

obj/mythread.o : src/mythread.c
	gcc -c src/mythread.c -o obj/mythread.o -I./inc
