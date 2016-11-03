a.out: dirinfo.c
	gcc dirinfo.c -o a.out

clean:
	rm *~

run: a.out
	./a.out
