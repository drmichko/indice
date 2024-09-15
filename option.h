#ifndef  OPTION_H
#define  OPTION_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

extern int dimen ;
extern  int optr;
extern  int tour  ;
extern  int idt   ;
extern  int iter  ;
extern  int job  ;
extern  int mode  ;
extern  int verbe ;
extern  int optT ;
extern  int optR ;
extern  int optK ;
extern  int optX ;
extern  FILE* src ;
extern  FILE* dst ;

int option(int argc, char *argv[]);
#endif
