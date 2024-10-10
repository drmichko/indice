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
listspace lsp = NULL;

int accept64(  int v )
{
v = abs( v );
return  ( v == 0 ||  v == 8 || v == 16 );
}

int admis64( int fct []  )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 64; t++ )
          f[ t ] = fct[ t ];
  Fourier( f,  64   );
  for( t = 0; t < 64 ; t++ )
         if ( ! accept64(  f[ t ] ) ) {
                 return 0;
         }
  return 1;
}


void translate( int fct[], int sp[], int W[] )
{
int cover[256]={ 0 };
for( i = 0; i < 64; i++ ) {
	if ( 0 == tfr[ sp[i]  ]  ) {
		if ( sp[i] < 128 ) cover[ sp[i] + 128 ] = 1;
		else cover[ sp[i] - 128 ] = 1;
		cover[ sp[i] ] = 1; 
	}	
}

void   mkblock ( int sp[], int W[]  )
{
  int t;
  int pos[ 32 ] ={ 0 }, nbp = 0;
  int left[ 64 ];
  for( t = 0; t < 64 ; t++ ){
          if ( tfr[  sp[t] ] == 0 ) {
                  pos[ nbp++ ] = t ;
          } else  left[t] = tfr[  sp[t] ] ;
  }

  soluce = 0;
 
  int i, p;
  for( p = 0; p < (1 << nbp) ; p++ ) {
        for( i = 0; i < nbp; i++ ){
                if ( ( 1 <<i ) & p ) {
                              left[ pos[ i ] ] = 1;
                }
                        else  {
                                left[ pos[ i ] ] = -1;
                        }
                }
        if ( admis64( left )  )  {
		        tranlate( left, sp, W );
                        soluce++;
                }
        }
printf("\nblock %d ( %d )\n", soluce, nbp  );
}


int zero(  int f[], listspace l )
{
int c  = 0;
for( int i = 0; i < subsize; i++) 
       			if (  0 == f[  l->sp[ i ]   ] ) c++;
	 

return c;
}

void  doit( boole f )
{
  int t;
  for( t =  0; t < 256; t++ )
	  tfr[t] = 0;

  for( t =  0; t < 128; t++ )
          tfr[ t ] = f[ t ] ? -1 : 1 ;
 
  Fourier( tfr, 128 );

  for( t = 0; t < 128; t++ )
        if ( tfr[ t ] != 0 )
                tfr[ t + 128 ] = tfr[t] = ( tfr[ t ] > 0 )   ? 1 : -1;

    listspace l, best = NULL;
    l = lsp;
    int score = 256;
    while ( l ) {
	    int s = zero( tfr, l);
	    if ( s < score) {
		    score = s;
		    best  = l;
	    }
	    l = l-> next;
    }

    int a, j = 0, W[4];
    for( a = 0; a < 256; a++)  
	    if ( 0 == (a & best->p) ) 
		    W[ j++ ] = a;
    printf("\nscore=%d  \n", score );
    mkblock( best->sp, W );
}



int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );


    
    int num = 0, count = 0;

    option(argc, argv);
   
    lsp  = spaces( optR  , 8 , 0  );


   
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
