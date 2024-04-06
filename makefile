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

CFLAGS=$(OPTION) $(INCLUDE) $(BIBLIO)



all : normal.exe 


normal.exe :  space.o normal.c
	gcc $(CFLAGS)  $^  -o $@  -lboole -lgmp



space.o  : space.c
	gcc  $(CFLAGS)  $^ -c 

lib :
	make -BC ../src
	make -B
clean :
	rm -f *.exe

