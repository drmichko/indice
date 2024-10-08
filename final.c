#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"


int tfr[ 256 ],  sgn[ 256 ], K [ 256 ];

typedef struct list {
        int * fct;
        vector key;
        struct list * next;
} enrlist, *list;

list  lf[ 4 ] = {NULL}, result;


void freelist( list l )
{
while ( l ){
        list aux = l;
        l = l->next;
        free( aux->fct);
        free( aux );
}
}


void print( int f[], int q )
{
printf("\nfct:");
int t;
for( t = 0; t < q; t++ )
	printf(" %d", f[t] );
printf("\n");
}

void plist( list l, int q  )
{
while ( l ){
        list aux = l;
        l = l->next;
        print( aux->fct, q );
}
}

void push32( int f[] )
{ list aux;
  aux = malloc( sizeof(enrlist) );
  aux->fct = calloc( 32, sizeof(int ) ); 
  int t;
  for( t = 0; t < 32; t++ )
  	aux->fct[t] = f[ t ];
  aux->next = result;
  result = aux;
}

int subsize = 128;
       #define _GNU_SOURCE         /* Consultez feature_test_macros(7) */
       #include <search.h>

#define  putbit( t, X ) ( X |= ( ((vector)1) << t ) ) 
#define  xorbit( t, X ) ( X ^= ( ((vector)1) << t ) ) 


listspace lsp = NULL;


int soluce;
int accept32(  int v )
{
v = abs( v );
return  ( v == 0 || v == 4 || v == 8 || v == 12 || v == 16 );
}

int accept64(  int v )
{
v = abs( v );
return  ( v == 0 ||  v == 8 || v == 16 );
}

int admis32( int fct[] )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 32; t++ )
	  f[ t ] = fct[ t ];

  Fourier( f,  32   );
  
  for( t = 0; t < 32 ; t++ )
	 if ( ! accept32(  f[ t ] ) ) {
		 return 0;
	 }
  return 1;
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

list  mkblock ( int offset  )
{ 
  int t;
  int pos[ 32 ] ={ 0 }, nbp = 0;
  int left[ 64 ];
  for( t = 0; t < 32 ; t++ ){
	  if ( tfr[ t + offset  ] == 0 ) {
		  pos[ nbp++ ] = t ;
	  } else  left[t] = left[ t + 32 ] = tfr[ t + offset ]; 
  }

  soluce = 0;
  result = NULL;
  int i, p;
  for( p = 0; p < (1 << nbp) ; p++ ) {
     	for( i = 0; i < nbp; i++ ){
	     	if ( ( 1 <<i ) & p ) { 
			      left[ pos[ i ] ] = 1;
			      left[ pos[ i ] + 32 ] = -1;
		}
	     		else  {
				left[ pos[ i ] ] = -1;
			        left[ pos[ i ] + 32 ] = +1;
			}
     		}
     	if ( admis64( left )  )  {
			push32( left );
			soluce++;
		}
	}
printf(" %d ( %d ) ", soluce, nbp  );
return result;
}

int check64( int offset, int fct []  )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 64; t++ )
	  if( tfr[ t + offset ] )  f[ t ] = fct[ t ];
          else f[ t ] = - fct[ t ];

  Fourier( f,  64   );
  for( t = 0; t < 64 ; t++ )
	 if ( ! accept64(  f[ t ] ) ) {
		 return 0;
	 }
  return 1;
}

void glue64( int offset, list ll, list lr)
{
list lx;
int fct[64];
soluce = 0;
while ( ll ) {
	int t;
	for( t = 0; t < 32; t++ )
                        fct[t] = ll->fct[t];
	lx = lr;
	while ( lx ) {
		for( t = 0; t < 32; t++ )
			fct[t + 32 ] = lx->fct[t];
		if ( admis64( fct ) ) soluce++;
		//&& check64(offset, fct) ) soluce++;
		lx = lx -> next;
	}
	ll = ll-> next;
}
printf("\nglue 64 : %d", soluce );
}
int test( boole f )
{ 
  int t;
  for( t =  0; t < 128; t++ )
	  sgn[ t ] = f[ t ] ? -1 : 1 ;

  for( t =  0; t < 128; t++ )
	  tfr[ t ] = f[ t ] ? -1 : 1 ;
   Fourier( tfr, 128 );
   for( t = 0; t < 128; t++ )
	if ( tfr[ t ] != 0 ) 
		tfr[ t + 128 ] = tfr[t] = ( tfr[ t ] > 0 )   ? 1 : -1;

     printf("\n");
     int q; 
   for ( q = 0; q < 4; q++ ) 
   	lf[ q ] =  mkblock( q * 32 );

    for( q = 0; q < 2; q++ ) glue64( q * 64 ,  lf[ 2*q ], lf[ 2*q +1] ); 
   
   return soluce > 0 ;
}


int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0, count = 0;



    while ((f = loadBoole(src))) {
		if (  job == num % mode) {
		      assert( isnearbent( f ) == 1 ); 
		      if ( test( f ) ) {
			      count++;
		      }
	}
	free( f );
	num++;
    }
    printf("\ncount=%d / %d\n", count, num );
   
    return 0;
}
