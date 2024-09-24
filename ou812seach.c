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

int plainRelative(int r, boole f)
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
    while (tmp) {
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

boole randboole( )
{
boole r = getboole();
int x;
for( x = 0; x < ffsize; x++ )
	r[x] = random() & 1;
return r;
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    int num = 0;

    int DR[9] = { -2, -2, -2, -2, -2, -2, -2, -2, -2 };
    boole SP[9] = { NULL };

    int r = tour;

    lsp = spaces(r, ffdimen);

    lsh = spaces(ffdimen - 1, ffdimen);

    size_t x;
    /*
     * trace = calloc( TAILLE, 1 );
     for( x = 0; x < TAILLE; x++ )
     trace[x] = NA; 
     */
    int score = 0;
    if ( mode == 0 ) mode = 1;
    srandom( time(NULL)) ;
    while ( num < mode ) {
	    f = randboole( );
	    int k = degree(f);
	    int dr = k;
	    if (DR[k] < k)
		dr = plainRelative(k, f);
	    if (DR[k] < dr) {
		DR[k] = dr;
		if ( SP[ k ] ) free( SP[k] );
		SP[k] = getboolecpy( f );
		if ( verbe ){
		 printf("\ndr=%d k=%d", dr, k );
		 panf( stdout, f );
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
