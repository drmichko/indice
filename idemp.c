#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "code.h"

listspace lsp = NULL;

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


boole randboole( )
{
boole r = getboole();
int x;
for( x = 0; x < ffsize; x++ )
	r[x] = random() & 1;
return r;
}

int rotation( int x ) 
{ int res= x, y = x;
  do {
	 y = y << 1;
	 if ( y >= ffsize ) 
		 y^= ffsize + 1;
	 if ( y < res ) res = y;
  } while ( x != y );
return res;
}

boole getrotation( int x ) 
{ boole res = getboole();
  int y = x;
  do {
	 y = y << 1;
	 if ( y >= ffsize ) 
		 y^= ffsize + 1;
	 res[x] = 1;
  } while ( x != y );
return res;
}

code idempotent( void )
{
int n = 0;
int x;
for( x = 0; x < ffsize; x++ ){
	if ( x == rotation( x ) ) n++;
}
code cc = getcode( n, ffsize );
n = 0;
for( x = 0; x < ffsize; x++ ){
        if ( x == rotation( x ) ) {
		cc.fct[n] = getrotation( x); 
		projboole( 2, 6,  cc.fct[n] );
		n++;
	}
}
int r = pivotage( cc );
cc.nbl = r;
return cc ;
}

int main(int argc, char *argv[])
{

    option(argc, argv);

    int DR[9] = { -2, -2, -2, -2, -2, -2, -2, -2, -2 };
    boole SP[9] = { NULL };

    int r = optR;

    lsp = spaces(r, ffdimen);

    code cc = idempotent( );
    printf("\ndim = %d", cc.nbl );
    int x;
    int score = 0;
   
    size_t limite = 1;
    limite <<= cc.nbl;
    size_t   N = 32;
    size_t  iter   = limite / N * mode;
    limite =  limite / N * (1+mode); 
    boole f = getboole();
    if ( iter == 0 ) iter++;
    size_t start =  iter ^ ( iter / 2 );
    int i = 0;
    while ( start ) {
	    if ( start & 1 ) 
		    for( x = 0; x < ffsize; x++ )
                    	f[x] ^= cc.fct[i][x];
	    i++;
	    start >>=1;
    }
    while ( iter < limite ) {
	    i = __builtin_ctzll(iter);
	    for( x = 0; x < ffsize; x++ )
		    f[x] ^= cc.fct[i][x];
	    iter++;
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
		 fflush( stdout );
		}
	    }
    }
    free(f);
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
