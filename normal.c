#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "space.h"
#include "option.h"

listspace lsp = NULL;

#define TAILLE 4294967296LU
#define NA 16

char *trace;


int booledeg(boole f, int size)
{
    int res = -1;
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

int  plainNormality( boole f )
{
    listspace tmp = lsp;
    //int64_t cpt0 = 0, cpt1 = 0, cpt2=0; 
    //int res = r;
    boole g = getboole();

    int limite = 1 << optR;
    int score = 0;
    while ( tmp  ) {
         int v;
	 int w = 0;
         for( v = 0; v < limite;  v++ )
                 if ( f[ tmp->sp[v] ] ) w++;;
         if ( score < w ) score = w;
	 tmp = tmp->next;
    }
    free(g);
    printf("score=%d\n", score);
    return score;
}




int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    int num = 0;


    lsp = spaces( optR , ffdimen ,  tour );

    while ((f = loadBoole(src))) {
	if (job == num % mode) {
	     //int d = degree(f);
	     if ( ! plainNormality(  f  ) ) panf(stdout, f );
	}
	free(f);
	num++;
    }
    
    return 0;
}
