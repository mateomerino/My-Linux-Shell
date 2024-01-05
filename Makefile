shell: shell.o myfunctions.a 
		gcc -o shell shell.o myfunctions.a

shell.o: shell.c functions.h 
		gcc -c shell.c

myfunctions.a: functions.o utilities.o
		ar 		rcs myfunctions.a functions.o utilities.o

functions.o: functions.c functions.h utilities.h
		gcc -c functions.c 

utilities.o: utilities.c utilities.h
		gcc -c utilities.c

clean:
	rm -f *.o *.a *.so shell 
