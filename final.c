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


int value[ 9 ][256];

void mkvalue( void )
{
int r;
for( r = 0; r < 9; r++ ){
	int v;
	for( v = 0; v < 256; v++ )
		value[ r ][ v ] = 0;
	int q = 1 << ( 8 - r );
	int p;
	for( p = 0; 2*p <= q; p++ )
		value[ r ][(q - 2*p)  ] =  1;
}
}
int accept( int r , int v )
{
v = abs( v );
return  ( v < 256  &&  value[ r ][ v ] );
}

int admis( int r, int tfr[]    )
{ int f[ 256 ] = { 0 };
  int t, q = 1 << r;
  for( t = 0; t < q; t++ )
	  f[t] = tfr[t];
  // printf("\nadmis %d :", r ); for( t = 0; t < 256 ; t++ ) printf("%3d", tfr[t] ); printf("\n");
  Fourier( f, 256 );
  //printf("\ntfr %d :", r ); for( t = 0; t < q ; t++ ) printf("%3d", f[t] ); printf("\n");
  // for( t = 0; t < q; t++ ) printf(" %3d", bad( f[t], r ) ); printf("\n");
  for( t = 0; t < q; t++ )
	  f[ t ] =  f[ t ] / q;
  for( t = 0; t < q; t++ )
	 if ( ! accept( r, f[ t ] ) ) {
		 //printf("\n#bad at %d :  %d\n", r, f[t] );
		 return 0;
	 }
  return 1;
}

int ctrl( int r, int tfr[]    )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 256; t++ )
	  f[t] = tfr[ t ];
  Fourier( f, 256 );
  for( t = 0; t < 256; t++ )
	  f[ t ] =  f[ t ] / 256;
  for( t = 0; t < 256; t++ ) 
	  if ( ! accept( 8, f[t] ) ) {
		  //printf("\n#bad :  %d\n", f[t] );
		  break;
	  }
  for( t = 0; t < 256; t++ )
	 if ( ! accept( f[ t ], 8 ) ) return 0;
  return 1;
}
void search( int r, int tfr[256]   )
{ 
  int t;

  if ( r == 6  ) {
  	  //printf("at %d :", r ); for( t = 0; t < 256 ; t++ ) printf("%3d", tfr[t] ); printf("\n");
	  if ( admis( 5, tfr ) )
		  soluce++;
	  return;
  }
  if ( r == 8  ) {
  	  //printf("at %d :", r ); for( t = 0; t < 256 ; t++ ) printf("%3d", tfr[t] ); printf("\n");
	  if ( ctrl( 8, tfr ) )
		  soluce++;
	  return;
  }
  int q = 1 << r;
  int pos[ 256] ={ 0 }, nbp = 0;
  for( t = 0; t < q; t++ ){
	  if ( tfr[ t  ] == 0 ) {
		  pos[ nbp++ ] = t;
	  }
  }
  int i, p;
  //printf("\nr=%d nbp=%d :", r, nbp );
  //for( p = 0; p < nbp; p++ )
//	  printf(" %d", pos[p] );

  if ( nbp > 0 ){
  	for( p = 0; p < (1 << nbp) ; p++ ) {
     		for( i = 0; i < nbp; i++ ){
	     		if ( ( 1 <<i ) & p ) 
			      tfr[ pos[ i ] ] = 16;
	     		else  tfr[ pos[ i ] ] = -16;
	     	tfr [ pos[ i ] + 128  ] =  - tfr[ pos[ i ] ];
     		}
     		if ( admis( r , tfr) )  
			search( r + 1, tfr );
     }
     for( i = 0; i < nbp; i++ ) tfr [ pos[ i ] + 128  ] = tfr[ pos[ i ] ]  = 0;
  } else search( r + 1, tfr );
}

int test( boole f )
{ 
  int t;
  int tfr[ 256 ] = { 0 };
  for( t =  0; t < ffsize; t++ )
	  tfr[ t ] = f[ t ] ? -1 : 1 ;
   Fourier( tfr, 128 );
   for( t = 0; t < 128; t++ )
	if ( tfr[ t ] != 0 ) 
		tfr[ t + 128 ] = tfr[ t ];

   soluce   = 0;
   search( 2   , tfr);
	  printf("\ns=%d\n", soluce );
   return soluce > 0 ;
}


int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0, count = 0;


    mkvalue( );

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
   
	  printf("\ns=%d\n", soluce );
    return 0;
}
