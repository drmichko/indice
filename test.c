#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

       #define _GNU_SOURCE         /* Consultez feature_test_macros(7) */
       #include <search.h>

#define  putbit( t, X ) ( X |= ( ((vector)1) << t ) ) 
#define  xorbit( t, X ) ( X ^= ( ((vector)1) << t ) ) 

int tfr [256];
int soluce = 0, subsize;

int zero( int f[], listspace l )
{
int r = 0;
for( int i = 0; i < subsize; i++) 
       if (  f[ l->sp[i] ^l->a] ) r++;	
return r;
}
int doit( boole f )
{
  int t;
  for( t =  0; t < 128; t++ )
          tfr[ t ] = f[ t ] ? -1 : 1 ;
 
  Fourier( tfr, 128 );

  for( t = 0; t < 128; t++ )
        if ( tfr[ t ] != 0 )
                tfr[ t + 128 ] = tfr[t] = ( tfr[ t ] > 0 )   ? 1 : -1;

    listspace l;
    l = lsh;
    int score = 256;
    while ( l ) {
	    int s = zero( tfr, l);
	    if ( s < score) score = s;
	    l = l-> next;
    }
    printf("score=%d\n", score);
    return score ;
}



int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0, count = 0;

    initboole(8);
    initdegrees( optT, optR );
    initboole(7);

    subsize = 1 << optR;
 
    while ((f = loadBoole(src))) {
		if (  job == num % mode) 
			doit( f );
	free( f );
	num++;
    }
    printf("\ncount=%d / %d\n", count, num );
    return 0;
}
