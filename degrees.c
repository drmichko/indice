#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"

listspace lsh = NULL;

#define TAILLE 4294967296LU
#define NA 16


int threshold  = 3;
int subdim     = 0;
void initdegrees( int thres, int r)
{
	threshold  = thres;
        subdim     = r;
	lsh = spaces( r , ffdimen); 
}

int booledeg(boole f, int size)
{
    int res = 0;
    int wt;
    xform(f, size);
    int u;
    for (u = 0; u < size; u++)
	if (f[u]) {
	    wt = __builtin_popcount(u);
	    if (wt > res)
		res = wt;
	}
    xform(f, size);
    return res;
}

int restriction( boole f )
{
    listspace tmp = lsh;

    int res = degree(f);
    boole g = getboole();
    
    int limite = 1 << subdim;

    while ( tmp && res > threshold   ) {
	 int v;
	 for( v = 0; v < limite;  v++ )
		 g[v] = f[ tmp->sp[v] ];
	 int d = booledeg(g, limite);
	 if ( d < res)
		    res = d;
	 tmp = tmp->next;
    }
    free(g);

    return res;
}

