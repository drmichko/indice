SHELL=/bin/bash
INCLUDE=-I../boole/src 
BIBLIO=-L../boole/src
OPTION = -O2

ifeq ($(shell hostname),serveur-imath01.univ-tln.fr)
	OPTION = -Wall -g
endif

ifeq ($(shell hostname),ou812.univ-tln.fr)
	OPTION = -Wall -g
endif
ifeq ($(shell hostname),port-gillot)
	OPTION = -Wall -g
endif

	OPTION = -Wall -g
CFLAGS=$(OPTION) $(INCLUDE) $(BIBLIO)



all : test.exe sb.exe rd.exe sieve.exe normal.exe search.exe calc.exe hybride.exe
	


sieve.exe :  degrees.o space.o option.o sieve.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp


rd.exe :  degrees.o space.o option.o rd.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp


test.exe :  degrees.o space.o option.o test.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp
sb.exe :  degrees.o space.o option.o sb.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp

normal.exe :  space.o option.o normal.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp


search.exe :  space.o degrees.o option.o search.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp

hybride.exe :  space.o degrees.o option.o hybride.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp

calc.exe :  degrees.o space.o option.o calc.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp


option.o  : option.c
	gcc  $(CFLAGS)  $^ -c 

space.o  : space.c
	gcc  $(CFLAGS)  $^ -c 

degrees.o  : degrees.c
	gcc  $(CFLAGS)  $^ -c 

lib :
	make -BC ../src
	make -B
clean :
	rm -f *.exe

