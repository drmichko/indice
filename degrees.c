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

int boolewalsh(boole f, int size)
{
    int res = 0;
    int tfr[ size ];
    int u;
    for (u = 0; u < size; u++)
	tfr[u] = f[u] ? -1 : +1;
    Fourier( tfr, size );
    for( u = 0; u < size; u++ )
	    if ( abs( tfr[u] ) > res )
		    res= abs( tfr[u] );
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

vector mypreImage( boole g, vector V[21], code Q )
{
galois x, p;
boole tmp;
vector rho = 0;

code G = copyCode( Q );
int r  =  G.nbl;

int i  =  0, j;
while ( i < r ){
   for( p = 0; p < G.nbc && G.fct[i][p]==0; p++ );
   if ( p < G.nbc ) {
       G.piv[i] = p;
       for( j = i+1; j < r; j++ )
          if ( G.fct[j][p] ){
            for( x = 0; x < G.nbc ; x++ )
                G.fct[j][x] ^=  G.fct[i][x];
	    V[j] ^= V[i];
	  }
       i++;
   } else {
        r--;
        tmp = G.fct[i];
        G.fct[i] = G.fct[r];
        G.fct[r] = tmp;
	vector tempo = V[i];
	V[i] = V[r];
	V[r] = tempo;
   }
}
for( i = 0; i < r; i++ )
	if ( g[ G.piv[i] ] ) {
		rho ^= V[ i ];
		for( j = 0; j < G.nbc; j++ )
			g[j] ^= G.fct[i][j] ;
	}
for ( j = 0; j < G.nbc && (g[j]==0) ; j++ ) ;;
assert( j == G.nbc );
G.nbl  =  Q.nbl;
freecode( G );
return rho;
}

void biftable( boole g, int  sp[] , basis_t *base, int *count )
{ 
	
  code L, Q;
  
  vector bp[21];
  L = getcode( 8,  16 ); 
  Q = getcode( 21, 16  );
  int x, k, dim = 0;

  for( k = 0; k < 7; k++ )
  	for( x = 0; x < 16; x++ )
                L.fct[k][x] = ( sp[x]  & (1<<k) ) > 0;

  for( x = 0; x < 16; x++ )
	  	L.fct[7][x] = 1;

  int r  = pivotage( L );

  assert( r == 5 );  

  reduce( g, L );
  vector V[21];
  for( k = 0; k < base->mat.nbl; k++ ) {
	V[ k ] = booleVector( base->mat.fct[ k ] , base ) ;
        for( x = 0; x < 16; x++ )
		Q.fct[k][x] = base->mat.fct[ k ][ sp[x] ];	
	reduce( Q.fct[k] , L );

  }
  
  vector vec = mypreImage( g, V, Q );

  code B = copyCode( base->mat );
  code K = kernel( B, Q ); 
  freecode( L );
  freecode( B );
  freecode( Q );
  dim = pivotage( K );
  
  for( k = 0; k < dim; k++ )
	   bp[ k ]  = booleVector( K.fct[k], base ) ;
  freecode( K );

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
	    panf( dst , g );
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
    // base->table[0] = 1;
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
	    fprintf(dst, "\n#tour=%d\n", tour );
    
    if ( count < 2097152  ){ 
	    fprintf(dst, "\n#candidat=%d count=%d\n", num, 2097152  - count );
	    num++;
	    sample( f , base );
    }
    return tour;
}


int testRestriction( boole f  )
{
    int res = 0;
    listspace tmp = lsh;

    boole g = getboole();

    int limite = 1 << subdim;
    // base->table[0] = 1;
    printf("\n\n---\n");
    int cpt=0, sum=0, sos = 0;;
    while ( tmp ) {
         int v;
         for( v = 0; v < limite;  v++ )
                 g[v] = f[ tmp->sp[v] ^ tmp->a ];
         int tfr =  0;
	 for ( v = 0; v < limite; v++ )
		 tfr   += (1 -2*g[v] );
	 printf(" %d", tfr );
	 sum += tfr;
	 sos += tfr*tfr; 
         cpt++;
	 if ( cpt % limite ==  0 ) {
		 printf("sum=%d sos=%d\n", sum, sos );
		 sum=0; 
		 sos = 0;
	 }
	 tmp = tmp->next;
    }
    free(g);
    return 0;
}
