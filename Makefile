
run: crash main
	./main ./crash

erreur: main
	./main
	
run_crash: crash
	./crash

compile: main crash

crash: crash.c	
	gcc crash.c -o crash -g
main: main.c
	gcc main.c -o main -g -lunwind-ptrace -lunwind-generic -lunwind
clean: 
	rm -f main crash
