SHELL=/bin/bash
INCLUDE=-I../boole/src 
BIBLIO=-L../boole/src

OPTION  = -O3    -march=native   -Wno-unused-result

ifeq ($(shell hostname),localhost.localdomain)
	OPTION = -Wall -g
endif

CFLAGS=$(OPTION) $(INCLUDE) $(BIBLIO)



all : sample.exe expand.exe rd.exe sieve.exe  search.exe calc.exe hybride.exe
	

sample.exe :  sample.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp

sieve.exe :  degrees.o space.o option.o sieve.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp


rd.exe :  degrees.o space.o option.o rd.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp

expand.exe :  degrees.o space.o option.o expand.c
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

