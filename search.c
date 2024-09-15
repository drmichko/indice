#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

listspace lsp = NULL;

#define TAILLE 4294967296LU
#define NA 16

char *trace;


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
/*
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
*/
boole randboole( int d )
{
boole r = getboole(  d );
int x;
for( x = 0; x < ffsize; x++ )
	if ( weight(x) <= d ) 
		r[ x ] = random() & 1;

ANFtoTT( r );
return r;
}

boole Randboole( )
{
boole r = getboole();
boole f = getboole();
int demi = ffdimen / 2, dsize, x, y;
dsize = 1 << demi;
for( y = 0; y < dsize; y++ ) {
	do {
		for( x = 0; x < dsize; x++ )
        		r[x] = random() & 1;
	} while ( booledeg( r, dsize ) != 2  ) ;
	for( x = 0; x < dsize; x++ )
		f[ x + (y << demi) ] = r[x];
}
return f;
}


int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    int num = 0;

    int DR[9] = { -2, -2, -2, -2, -2, -2, -2, -2, -2 };
    boole SP[9] = { NULL };

    
    initdegrees( optT, optR );

    
    if ( mode == 0 ) mode = 1;
    srandom( time(NULL) + getpid() ) ;
    if ( ! optK ) optK = ffdimen;

    while ( num < mode ) {
	    f = randboole(  optK  );
	    int k = degree( f );
	    int dr = k;
	    dr = restriction( f );
	    if ( DR[ k ] < dr) {
		DR[ k ] = dr;
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
    int d;
    printf("----");
    for (d = 0; d <= ffdimen; d++)
	printf("%3d :", d );
    printf("\n");
    printf("%2d !", optR );
    for (d = 0; d <= ffdimen; d++)
	printf("%3d :", DR[d]);
    printf("\n");

    FILE *save;
    char fn[32];
    sprintf( fn, "rand/line-%d-%d-%d.txt", optR, job, ffdimen );

    save = fopen( fn, "a" ); 
    fprintf( save, "%2d !", optR );
    for (d = 0; d <= ffdimen; d++)
        fprintf( save, "%3d :", DR[d]);

    for( d = 1; d <= ffdimen; d++ )
	    if ( SP[d] ) {
	   		fprintf(save, "\n#sample : k=%d dr=%d", d , DR[d] );
			panf( save, SP[d] );
		        fprintf(save, "\n");
	    }
    fclose( save );
    return 0;
}
