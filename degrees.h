#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"

#include "basistools.h" 

void initdegrees( int thres, int r);
int booledeg(boole f, int size);
int restriction( boole f );
int Restriction( boole f , basis_t *base);
void countspace( int c[], boole f );
