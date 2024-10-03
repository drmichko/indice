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

int tfr[ 256 ];
int zzz[ 256  ];
int spc[ 256  ];
int fct[ 256 ];

#define  ZZ  ( '?' )
int EXIT = 0;

int ok ( int v )
{
v = abs( v );
if ( v % subsize) return 0;
v = v / subsize;
if ( subsize == 128 ) 
	return ( v == 0 || v == 2  );
if ( subsize == 64 )
	return ( v == 0 || v == 2 || v == 4  );
return ( v== 8 || v == 6  || v == 4 || v == 2  || v == 0 );
}

int good(  )
{
	int f[ 256 ], a;
	for( a = 0; a < 256; a++ )
		f[ a ] = fct[ a ];
        Fourier( f, 256 );	
	//printf("\n");for( a = 0; a < 256 ; a++) printf(" %3d", f[a]/subsize );
	for( a = 0; a < 256  && ok( f[ a ]  ) ; a++);;
	return ( a == 256  );
}

int trial;

void lift( int x )
{

if ( EXIT ) return; 

if ( x == 256   ) {
	trial++;
	if ( good(  ) ) 
		EXIT = 1;
	return;
}

if ( spc[ x ]  && zzz[ x ]  ) {
   if ( x < 128  || spc[ x - 128 ] == 0 ) {
	   fct[ x ]  = +16 ; lift( x+1 );
   	   fct[ x ]  = -16 ; lift( x+1 );
           fct[ x ] = 0;
   } else lift( x+1 );
} else   lift( x+1 );

}
void  admis(  )
{ 
  EXIT = 0;
  int t;
  for( t = 0; t < 256 ;  t++ )
	  fct[ t ] = spc[ t ] * tfr[ t] ; 
  trial = 0;
  lift( 0   );
  //printf("#trials : %d\n", trial );
 
}

int score(    )
{ 
  int t;
  int z = 0;
  for ( t = 0; t < 128 ; t++ )
          if ( spc[ t ] + spc[ t + 128 ] - spc[ t ]*spc[ t + 128] ) z++;

  return z;
}



int test( boole f )
{ 
  int x, y, t, cpt = 0;
  for( t =  0; t < ffsize; t++ )
	  tfr[ t ] = f[ t ] ? -1 : 1 ;
   Fourier( tfr, 128 );
   for( t = 0; t < 128; t++ )
	if ( tfr[ t ] != 0 ) tfr[ t + 128 ] = tfr[ t ];
        else zzz[ t ] = 1;

  int iter = 0;
  while ( iter--) {
	  do { t = random() % 256; } while ( tfr[t] != 16 && tfr[t] != -16 ) ; tfr[t] = ZZ; 
  }
   listspace aux = lsp;
   int best = 256, mult;
   while ( aux ) {
  	  for( t = 0; t < 256; t++ )
		  spc[ t ] = 0;
	  for( t = 0; t < subsize; t++ )
		  spc[ aux->sp[t] ] = 1; 
	  int tmp = score(   ); 
	  if ( tmp > 0 && tmp < best ) {
		  best = tmp;
		  mult = 0;
	  }
	  if ( tmp == best ) mult++;
	  aux = aux->next;
   }
   printf("\nbest : %d (%d)\n", best , mult);
   aux = lsp;
   while ( aux ) {
	   for( t = 0; t < 256; t++ )
                  spc[ t ] = 0;
          for( t = 0; t < subsize; t++ )
                  spc[ aux->sp[t] ] = 1;

	  int tmp = score(  ); 
	  if ( tmp == best  ) {
		  admis(  );
		  if ( ! EXIT ) 
			  return 0;
	  }
	  aux = aux->next;
   }
   return 1;
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0, count = 0;


    lsp = spaces(  optR  ,  8,  0 );

    subsize = 1 << optR; 

    while ((f = loadBoole(src))) {
		if (  job == num % mode) {
		      assert( isnearbent( f ) == 1 ); 
		      if ( test( f ) ) {
			      panf( stdout, f );
			      count++;
		      }
	}
	free( f );
	num++;
    }
    printf("\ncount=%d / %d\n", count, num );
   
    return 0;
}
