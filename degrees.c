#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "basistools.h"
#include "orbitools.h"

listspace lsh = NULL;

#define TAILLE 4294967296LU
#define NA 16


int threshold  = -1;
int subdim     = 0;
void initdegrees( int thres, int r)
{
	threshold  = thres;
        subdim     = r;
	lsh = spaces( r , ffdimen, 1 ); 
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

    while ( tmp &&  ( res > threshold)   ) {
	 int v;
	 for( v = 0; v < limite;  v++ )
		 g[v] = f[ tmp->sp[v] ^ tmp->a  ];
	 int d = booledeg(g, limite);
	 if ( d < res)
		    res = d;
	 tmp = tmp->next;
    }
    free(g);

    return res;
}

void countspace( int c[], boole f )
{
    listspace tmp = lsh;
    c[0]=c[1]=c[2] = c[3] = 0;
    boole g = getboole();
    
    int limite = 1 << subdim;

    while ( tmp    ) {
	 int v;
	 for( v = 0; v < limite;  v++ )
		 g[v] = f[ tmp->sp[v] ^ tmp->a  ];
	 int d = booledeg(g, limite);
	 if ( d <= 2 )
		    c[ d+1]++;
	 tmp = tmp->next;
    }
    free(g);
}


void biftable( boole g, int  sp[] , basis_t *base, int *count )
{ 
	
  code L, Q;
  
  vector bp[21];
  L = rmcode( 0, 1, 4 ); 
  Q = getcode( 21, 16  );
  int x, k, dim = 0;
  x = pivotage( L );
  

  reduce( g, L );

  for( k = 0; k < base->mat.nbl; k++ ) {
        for( x = 0; x < 16; x++ )
		Q.fct[k][x] = base->mat.fct[ k ][ sp[x] ];	
	reduce( Q.fct[k] , L );

  }
  code B = copyCode( base->mat );
  code K = kernel( B, Q ); 
  freecode( L );
  freecode( B );
  freecode( Q );
  dim = pivotage( K );
  
  for( k = 0; k < dim; k++ )
	   bp[ k ]  = booleVector( K.fct[k], base ) ;
  freecode( K );

  vector  vec = booleVector( g, base ) ;

  if ( ! base->table[ vec ]  ) {
	   base->table[ vec ] = 1;
            *count = *count   + 1;

  }
  vector  step = 1;
  vector limite = 1 << dim;
  while ( step < limite ) {
    int k = __builtin_ctzll( step );
    vec ^= bp[ k ];
    if ( ! base->table[ vec ]  ) {
	    base->table[ vec ] = 1;
	    *count = *count   + 1;
    }
    step++;
  }
}

int num = 0;

void sample( boole f , basis_t *base )
{
vector v;
for ( v = 0; v < base->size; v++ )
	if ( ! base->table[ v ] ) {
	    boole g = vectortoboole( v , base );
	    int x;
	    for( x = 0; x < ffsize; x++ )
		    g[x] ^=  f[x];
	    panf( stdout, g );
	    free( g );
	}
}

int Restriction( boole f, basis_t *base )
{
    listspace tmp = lsh;

    int tour = 0;
    boole g = getboole();

    int limite = 1 << subdim;
    int count  = 0;
    base->table[0] = 1;
    while ( tmp  &&  count < 2097152  ) {
         int v;
         for( v = 0; v < limite;  v++ )
                 g[v] = f[ tmp->sp[v] ^ tmp->a ];
         int d = booledeg(g, limite);
         if ( d <= 2) {
                    biftable( g, tmp->sp , base, &count );
                    tour++;
         }
         tmp = tmp->next;
    }
    free(g);
    if ( verbe )
	    printf("\n#tour=%d\n", tour );
    
    if ( count < 2097152  ){ 
	    printf("\n#candidat=%d count=%d\n", num, 2097152  - count );
	    num++;
	    sample( f , base );
    }
    return tour;
}

