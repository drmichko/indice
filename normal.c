#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"

listspace lsp = NULL;
listspace lsh = NULL;

#define TAILLE 4294967296LU
#define NA 16

char *trace;


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

int plainRelative( int r, boole f )
{
    listspace tmp = lsh;

    int res = r;
    boole g = getboole();

    int limite = 1 << r;

    while ( tmp && res > 0 ) {
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


int oldplainRelative(int r, boole f)
{
    listspace tmp = lsp;

    int res = r;

    boole g = getboole();
    int limite = 1 << r;
    int size = ffsize / limite;
    while (tmp) {
	int t;
	for (t = 0; t < size; t++)
	    if ((tmp->p & t) == 0) {
		int v = 0;
		int a = t;
		int iter = 1;
		g[0] = f[a];
		while (iter < limite) {
		    int i = __builtin_ctz(iter);
		    a ^= tmp->V[i];
		    v ^= 1 << i;
		    g[v] = f[a];
		    iter++;
		}
		int d = booledeg(g, limite);
		if (d < res)
		    res = d;
	    }
	tmp = tmp->next;
    }
    free(g);

    return res;
}

size_t key(boole f, int size)
{
    size_t res;
    int v;
    for (v = 0; v < size; v++)
	res ^= (size_t) f[v] << v;
    return res;
}


int relativeDegree(int r, boole f)
{
    listspace tmp = lsh;

    int res = r;

    boole g = getboole();
    while ( tmp && res >= 0 ) {
	int t;
	for (t = 0; t < ffsize; t++)
	    if ((tmp->p & t) == 0) {
		int v = 0;
		int a = t;
		int iter = 1;
		int limite = 1 << (ffsize / 2);
		g[0] = f[a];
		while (iter < limite) {
		    int i = __builtin_ctz(iter);
		    a ^= tmp->V[i];
		    v ^= 1 << i;
		    g[v] = f[a];
		    iter++;
		}
		int d = booledeg(g, ffsize / 2);
		if (d < res)
		    res = d;
	    }
	tmp = tmp->next;
    }
    free(g);

    return res;
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    int num = 0;

    int DR[8] = { -2, -2, -2, -2, -2, -2, -2, -2 };
    boole SP[8] = { NULL };

    int r = tour;

    lsp = spaces(r, ffdimen);
    lsh = spaces(ffdimen - 1, ffdimen);

    while ((f = loadBoole(src))) {
	if (job == num % mode) {
	    int k = degree(f);
	    int dr = k;
	    if (DR[k] < k)
		dr = plainRelative(k, f);
	    if (DR[k] < dr) {
		DR[k] = dr;
		if ( SP[ k ] ) free( SP[k] );
		SP[k] = getboolecpy( f );
		}
	}
	free(f);
	num++;
    }
    printf("\n#boole : %d\n", num);
    int d;
    for (d = 1; d <= ffdimen; d++)
	printf("%3d :", DR[d]);
    printf("\n");

    if ( verbe ) 
	for( d = 1; d <= ffdimen; d++ )
	    if ( SP[d] ) {
	   		printf("\n#sample : k=%d dr=%d", d , DR[d] );
			panf( stdout, SP[d] );
		        printf("\n");
	    }
    return 0;
}
