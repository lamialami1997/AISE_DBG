run: crash main
	./main ./crash

run_crash: crash
	./crash

compile: main crash

crash: crash.c	
	gcc crash.c -o crash -g
main: main.c
	gcc main.c -o main -g
clean: 
	rm -f main crash
