#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"

listspace lsp = NULL;
listspace lsh = NULL;

#define TAILLE 4294967296LU
#define NA 16

char * trace;

int subrelativeDegree( int r, boole f )
{ listspace tmp = lsp;
 
  int res = r;
  int ff = ffdimen;
  initboole( r );

  boole g = getboole( );
  while ( tmp   ){
	  int t;
	  for( t = 0; t < 64; t++ )
		if ( (tmp->p & t) == 0 ) {
	  	int v = 0;
		int a = t;
		int iter = 1;
  		int limite  = 1 << r;
		g[ 0 ] = f[ a ];
		while ( iter < limite ) {
		    int i = __builtin_ctz( iter );
                    a ^= tmp->V[ i ];
		    v ^= 1 << i;
        	    g[v] = f[a];
		    iter++;
		}
		int d = degree( g );
		if ( d < res ) 
			res = d;
  	   }
	  tmp = tmp->next;
  }
  free( g );
  initboole( ff );

  return res;
}

int booledeg( boole f, int size )
{
	int res = 0;
  int wt;
	xform( f, size );
	int u;
	for( u = 0; u < size; u++ )
		if ( f[u] ) {
		wt  = __builtin_popcount( u );
		if ( wt >  res  ) res = wt;
	}
	xform( f, size );
   return res;
}

size_t key( boole f, int size )
{ size_t res;
  int v;
  for( v = 0; v < size; v++ )
	  res ^=   (size_t) f[v]  << v;
  return res;
}

int relativeDegree( int r, boole f )
{ listspace tmp = lsh;
 
  int res = r;

   boole g = getboole( );
  while ( tmp   ){
	  int t;
	  for( t = 0; t < ffsize; t++ )
		if ( (tmp->p & t) == 0 ) {
	  	int v = 0;
		int a = t;
		int iter = 1;
  		int limite  = 1 << (ffsize/2);
		g[ 0 ] = f[ a ];
		while ( iter < limite ) {
		    int i = __builtin_ctz( iter );
                    a ^= tmp->V[ i ];
		    v ^= 1 << i;
        	    g[v] = f[ a ];
		    iter++;
		}
		size_t pos = key( g, ffsize / 2 );
		if (  trace[pos] == NA ) {
			trace[pos] = booledeg( g , ffsize /2 );
		if ( trace[pos] < res ) 
			res = trace[pos];
		}
  	   }
	  tmp = tmp->next;
  }
  free( g );

  return res;
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );
    int dim =  atoi( argv[1] );
    int r = atoi( argv[2] );
    FILE * src = fopen( argv[3], "r" );
    int num = 0;
    initboole( dim  );
    int DR[ 7 ] = { -2, -2, -2, -2,-2, -2, -2 };

    lsp = spaces(  r , ffdimen - 1 );
    lsh = spaces(  ffdimen-1 , ffdimen  );

    size_t x;
    trace = calloc( TAILLE, 1 );
    for( x = 0; x < TAILLE; x++ )
	    trace[x] = NA; 

    while ((f = loadBoole(src))) {
	int k  = degree( f );
	if (  DR[k] < k ) {
		int dr = relativeDegree( r, f ); 
		if ( dr > DR[ k ] ) {
			DR[ k ] = dr;
			int d;
			for( d = 0; d <= ffdimen; d++ )
				printf("%3d", DR[ d ]  );
			panf( stdout, f );
			printf("\n");
			
		}
        }
	free(f);
	num++;
    }

    printf("\n#boole : %d\n", num );
    return 0;
}
