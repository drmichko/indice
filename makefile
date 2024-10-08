SHELL=/bin/bash
INCLUDE=-I../boole/src 
BIBLIO=-L../boole/src

OPTION  = -O3    -march=native   -Wno-unused-result

ifeq ($(shell hostname),localhost.localdomain)
	OPTION = -Wall -g
endif

CFLAGS=$(OPTION) $(INCLUDE) $(BIBLIO)



all : final.exe test.exe sb.exe rd.exe sieve.exe normal.exe search.exe calc.exe hybride.exe
	


sieve.exe :  degrees.o space.o option.o sieve.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp


rd.exe :  degrees.o space.o option.o rd.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp


essai.exe :  degrees.o space.o option.o essai.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp

final.exe :  degrees.o space.o option.o final.c
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

