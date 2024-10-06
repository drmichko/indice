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


int Soluce;
int soluce;


int value[ 9 ][256];

void mkvalue( void )
{
int r;
for( r = 4 ; r < 9; r++ ){
	int v;
	for( v = 0; v < 256; v++ )
		value[ r ][ v ] = 0;
	int q = 1 << ( 8 - r );
	int p;
	for( p = 0; 2*p <= q; p++ )
		value[ r ][(q - 2*p)  *  ( 1 << (r-4) )] =  1;
}
}
int accept( int r , int v )
{
v = abs( v );
return  ( v < 256  &&  value[ r ][ v ] );
}

int admis( int s, int r, int tfr[]    )
{ int f[ 256 ] = { 0 };
  int t, q = 1 << r;
  
  for( t = 0; t < q; t++ )
	  f[ t ] = tfr[ s +  t];

  // printf("\nadmis %d :", r ); for( t = 0; t < 256 ; t++ ) printf("%3d", tfr[t] ); printf("\n");
  Fourier( f,  q   );

  //printf("\ntfr %d :", r ); for( t = 0; t < q ; t++ ) printf("%3d", f[t] ); printf("\n");
  //for( t = 0; t < q; t++ ) printf(" %3d", accept( r, f[t] ) ); printf("\n");
  
  for( t = 0; t < q; t++ )
	 if ( ! accept( r, f[ t ] ) ) {
		 //printf("\n#bad at %d :  %d\n", r, f[t] );
		 return 0;
	 }
  return 1;
}

void filling( int s , int r, int tfr[256]   )
{ 
  int t;
  int pos[ 256 ] ={ 0 }, nbp = 0;
  int q = 1 << r;
  for( t = 0; t < q ; t++ ){
	  if ( tfr[ t + s  ] == 0 ) {
		  pos[ nbp++ ] = t + s;
	  }
  }
  int i, p;

  	for( p = 0; p < (1 << nbp) ; p++ ) {
     		for( i = 0; i < nbp; i++ ){
	     		if ( ( 1 <<i ) & p ) 
			      tfr[ pos[ i ] ] = 1;
	     		else  tfr[ pos[ i ] ] = -1;
	     	tfr [ pos[ i ] + 128  ] =  - tfr[ pos[ i ] ];
     		}
     		if ( admis( s - q  , 6  , tfr)  &&  admis( 128 + s - q  , 6 , tfr ) )  
			  soluce++;
     }
}
void search( int s , int r, int tfr[256]   )
{ 
  int t;
  int pos[ 256 ] ={ 0 }, nbp = 0;
  int q = 1 << r;
  for( t = 0; t < q ; t++ ){
	  if ( tfr[ t + s  ] == 0 ) {
		  pos[ nbp++ ] = t + s;
	  }
  }
  int i, p;
  	for( p = 0; p < (1 << nbp) ; p++ ) {
     		for( i = 0; i < nbp; i++ ){
	     		if ( ( 1 <<i ) & p ) 
			      tfr[ pos[ i ] ] = 1;
	     		else  tfr[ pos[ i ] ] = -1;
	     	tfr [ pos[ i ] + 128  ] =  - tfr[ pos[ i ] ];
     		}
     		if ( admis( s  , 5  , tfr)  &&  admis( 128 + s , 5, tfr ) )  
			 filling( s + q, 5, tfr );
		}
}

void search64(  0, int tfr[256]   )
{ 
  search64( 0, tfr );
     		}
     		if ( admis( s  , 5  , tfr)  &&  admis( 128 + s , 5, tfr ) )  
			 filling( s + q, 5, tfr );
		}
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
		tfr[ t + 128 ] = tfr[t] = ( tfr[ t ] > 0 )   ? 1 : -1;

   soluce   = 0; 
   search( 0 , 64, tfr );
   printf("\n#soluce=%d\n", soluce );
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
   
    return 0;
}
