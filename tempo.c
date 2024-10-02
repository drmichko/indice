#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

int subsize = 128;
       #define _GNU_SOURCE         /* Consultez feature_test_macros(7) */
       #include <search.h>

#define  putbit( t, X ) ( X |= ( ((vector)1) << t ) ) 
#define  xorbit( t, X ) ( X ^= ( ((vector)1) << t ) ) 


listspace lsp = NULL;


int Soluce[ 8 ];
int soluce;

int    countp = 0, total = 0;
void  * rootp = NULL;

typedef struct list {
	int* L, *H;
	vector key;
	struct list * next;
} enrlist, *list;

list  lf[ 8 ] = {NULL}, result;
list* table;



#define  ZZ  ( '?' )
int EXIT = 0;
int ok( int v )
{
	v  = abs( v );
	if ( v != 0  && v != 2  ) return 0;
	return 1;
}
void lift( int x, listspace l, int res[ ], int tfr[]	 )
{
if ( EXIT ) return; 
if ( x == subsize  ) {
	int f[ 256  ];
	for( x = 0; x < 256  ; x++) 
		f[x] = res[ x ];
	Fourier( f,  128 );
	for( x = 0; x < 256; x++ )
		f[x] /= subsize;
	printf("\n");for( x = 0; x < 256 ; x++) printf("%3d", f[x] );
	for( x = 0; x < 256  && ok( f[ x ] ) ; x++);;
	if ( x == 256  ) EXIT = 1;
	return;
}
if ( tfr[ l->sp[x] ] !=  ZZ ) {
	res[  l->sp[x]  ] = tfr[ l->sp[x] ] ;
	lift( x+1, l, res, tfr );
}
else {
   res[  l->sp[x]  ] = +16 ; lift( x+1, l, res, tfr );
   res[  l->sp[x]  ] = -16 ; lift( x+1, l, res, tfr );
   res[  l->sp[x]  ] = 0;
}

}
int admis( listspace l, int tfr[ 256 ]  )
{ 
  int  res[ 256 ];
  EXIT = 0;
  int t;
  for( t = 0; t < 256 ;  t++ )
	  res[ t ] = 0; 
  lift( 0,  l, res, tfr  );
  return EXIT;
}

int score( listspace l, int tfr[ 256 ]  )
{ 
  int  res[ 256  ];
  int t;
  for( t = 0; t < subsize ; t++ )
          res[ t ] = tfr[ l->sp[t]  ];
  int z = 0;
  for ( t = 0; t < subsize ; t++ )
          if ( res[ t ] == ZZ ) z++;
  return z;
}



int test( boole f )
{ int tfr[ 256 ];
  vector X = 0;
  int x, y, t, cpt = 0;
  for( t = 0; t < ffsize; t++ )
	  tfr[ t ] = f[ t ] ? -1 : 1 ;
   Fourier( tfr, ffsize );
   for( t = 0; t < ffsize; t++ )
	tfr[t + ffsize] = (tfr[t] == 0) ? ZZ : 0;  
   listspace aux = lsp, auy = NULL;
   int best = 256;
   while ( aux ) {
	  int tmp = score( aux, tfr ); 
	  if ( tmp > 0 && tmp < best ) {
		  auy  = aux;
		  best = tmp;
	  }
	  aux = aux->next;
   }

   return admis( auy, tfr );
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0, count = 0;


    lsp = spaces(  7 ,  8,  0 );

    subsize = 128; 

    while ((f = loadBoole(src))) {
		if (  job == num % mode) {
		      if ( test( f ) ) count++;
    		      printf("\ncount=%d / %d\n", count, num );
	}
	free( f );
	num++;
    }
    printf("\ncount=%d / %d\n", count, num );
    return 0;
}
