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


void freelist( list l )
{
while ( l ){ 
	list aux = l;
	l = l->next;
	free( aux->L);
	free( aux->H);
	free( aux );
}	
}

int check32( int t[] )
{ int x;
  int c = 0;
  for ( x = 0; x < 32; x++ ) {
  	if ( t[x] != 0 && abs(t[x]) != 4 && abs( t[x] ) != 8 && abs( t[x] ) !=12  && abs( t[x] ) !=16    )
		return 0;
	if ( abs(t[x]) == 16 ) c++;
	if ( c > 1) return 0;
	}		
  return 1;
}


int acc128( int t[]  ) 
{ int x;
  for ( x = 0; x < 128; x++ ) {
  	if ( t[x] != 0 && abs( t[x] ) !=16    )
		return 0;
	}		
  return 1;
}
int check64( int t[]  ) 
{ int x;
  for ( x = 0; x < 64; x++ ) {
  	if ( t[x] != 0 && abs( t[x] ) != 8 && abs( t[x] ) !=16    )
		return 0;
	}		
 
  return 1; 

  int u, v, w;
  for( u = 0; u < 64; u++)
    if  ( abs( t[u] ) == 16 )
       for( v = u+1; v < 64; v++)
	  if  ( abs( t[v]  ) == 16 )
            for( w = v+1; w < 64; w++)
		    if  ( abs( t[w]  ) == 16  &&  ( u ^v^w ) > w   && abs( t[u^v^w] ) == 16 )     {
	               int signe =  t[u] * t[v] * t[w] * t[u^v^w];
	               if ( signe > 0 ) return 0;
	  } 
  // CHECK THAT POINT
  return 1;
}


typedef struct _node_ {
        int    val;
        vector key ;
} node_t;

static int nodecompare( const void *a,  const void *b)
{
    node_t* fa, *fb;
  fa = (node_t*) a;
  fb = (node_t*) b;
  if ( fa->key < fb->key ) return -1;
  if ( fa->key > fb->key ) return +1;


  return 0;
}

int nouvelle;

int searchkey( vector key,  void **rootp, int* count )
{
node_t  *ptr = malloc( sizeof(node_t) );
ptr->key = key;
const void *ret;
int value;
    ret  = tsearch((void *) ptr, rootp , nodecompare );
    if ( ret == NULL)
        exit(EXIT_FAILURE);
    if ((*(node_t **) ret)->key != ptr->key ) {
        value = (*(node_t**) ret)->val;
        free( ptr );
        nouvelle = 0;
        return value;
    };
    // it's new !
    ptr->val = *count;
    *count   = *count + 1;
    value = ptr->val;
    nouvelle = 1;
    return value;
}

int findkey( vector key,  void **rootp  )
{
node_t  *ptr = malloc( sizeof(node_t) );
ptr->key = key;
const void *ret;
    ret  = tfind((void *) ptr, rootp , nodecompare );
    if ( ret == NULL) return  -1;
    free( ptr );
    return (*(node_t**) ret)->val;
}

/*
int check( int t[], int size )
{
	switch( size ) {
		case 16  : if ( ! acc16( t ) ) return 0; break;
		case 64  : if ( ! acc64( t ) ) return 0; break;
		case 128 : if ( ! acc128( t ) ) return 0; break;
	}

       list aux = malloc( sizeof(enrlist) );
       int x;
       for( x = 0 ; x < size; x++ )
	       aux->tfr[x] = t[x];

       aux->zero = 0;
       for( x = 0 ; x < size; x++ )
	       if ( t[x] == 0 )  xorbit( x, aux->zero );
       aux->eight = 0;
       for( x = 0 ; x < size; x++ )
	       if ( abs(t[x]) == 8 )  xorbit( x, aux->eight );
       aux->seize = 0;
       for( x = 0 ; x < size; x++ )
	       if ( abs(t[x]) == 16 )  xorbit( x, aux->seize );

	aux->next =  result;
	result = aux;
	soluce++;

	return 1;
}
*/

void freetable( list * t, int n )
{
int i;
for( i = 0; i < n; i++ )
	freelist( t[i] );
free( t) ;
}

void mktable( list l, int n )
{
table = calloc( n, sizeof(list) );
rootp  = NULL;
countp = 0;
while ( l ) {
	int val = searchkey( l->key, &rootp, &countp );
	list aux = l;
	l = l-> next;
	aux ->next =  table[ val ];
	table[ val ] = aux; 
}
printf("\n#countp : %d", countp );
}


list  merge( list l1, list l2, int *s)
{
int  L[64], H[64];
list result = NULL;
int  soluce = 0;
printf("\n");
iter = 0;
list  lx;
while ( l1 ) {
	lx =  l2;
	while ( lx ) {
		int x;
		for( x = 0; x < 32; x++ ) {
			L[ x ]    = l1->L[ x ] +  lx->L[ x ];
			L[ x+32 ] = l1->L[ x ] -  lx->L[ x ];
		}
		for( x = 0; x < 32; x++ ){
			H[ x ]    = l1->H[ x ] +  lx->H[ x ];
			H[ x+32 ] = l1->H[ x ] -  lx->H[ x ];
		}
		if ( check64( L ) && check64( H ) ) {
			list aux = malloc( sizeof(enrlist) );	
			aux->L = calloc( 64, sizeof(int) );	
			vector vec = 0;
			int t;
			for( t = 0; t < 64; t++ ){
	     			aux->L[t] =  L[ t ];
				if ( ( abs(L[t] ) == 8 ) ) 
					xorbit( t, vec); 
			}
			aux->key = vec;
			aux->H = calloc( 64, sizeof(int) );
			for( t = 0; t < 64; t++ )
	     			aux->H[t] =  H[ t ];

			aux->next = result;
			result = aux;
			soluce++;
		}
		lx = lx-> next;
	} 
	l1= l1-> next;
} 

printf("\n#sol 64 : %d", soluce );
*s =  soluce;
return result;
}

int final( int val, int L[64], int H[64] )
{
	list aux = table[val];
	int tfr[ 256 ], res = 0;
	while ( aux ) {
		int x;
		for( x = 0; x < 64; x++ ) {
			tfr[x] =  aux->L[x] + L[x];
			tfr[x+64] =  aux->L[x] - L[x];
			tfr[x+128] =  aux->H[x] + H[x];
			tfr[x+192] =  aux->H[x] - H[x];

		}
		for( x = 0; x < 256 && abs( tfr[x] ) == 16 ; x++ ) ;;;
		if ( x > 128) printf("*");
		if ( x == 256 ) res++;
		aux = aux->next;
	}
	return res;
}
void mega( list l1, list l2 )
{
int  L[64], H[64];
int  soluce = 0;
printf("\n");
iter = 0;
list  lx;
while ( l1 ) {
	lx =  l2;
	while ( lx ) {
		int x;
		for( x = 0; x < 32; x++ ) {
			L[ x ]    = l1->L[ x ] +  lx->L[ x ];
			L[ x+32 ] = l1->L[ x ] -  lx->L[ x ];
		}
		for( x = 0; x < 32; x++ ){
			H[ x ]    = l1->H[ x ] +  lx->H[ x ];
			H[ x+32 ] = l1->H[ x ] -  lx->H[ x ];
		}
		if ( check64( L ) && check64( H ) ) {
			vector vec = 0;
			int t;
			for( t = 0; t < 64; t++ ){
				if ( ( abs(L[t] ) == 8 ) ) 
					xorbit( t, vec); 
			}
			int val = findkey( vec, &rootp );
			if ( val >=0 ) 
				soluce += final( val, L, H );
		}
		lx = lx-> next;
	} 
	l1= l1-> next;
} 
total += soluce;
printf("\n#sol 256 : %d / %d ", soluce, total );
}


void pushit( int L[32], int H[32])
{       int tmp[ 32  ];
	int t;
	for( t = 0; t < 32; t++ )
               tmp[ t ] = L[ t ];
        Fourier( tmp, 32 );
	if ( ! check32( tmp ) ) return;
        int dec[ 32  ];
	for( t = 0; t < 32; t++ )
                        dec[ t ] = H[ t ];
        Fourier( dec, 32 );
	if ( ! check32( dec ) ) {
		return;
	}
	list aux = malloc( sizeof(enrlist) );	
	
	aux->L = calloc( 32, sizeof(int) );
	for( t = 0; t < 32; t++ )
	     aux->L[t] =  tmp[ t ];

	aux->H = calloc( 32, sizeof(int) );
	for( t = 0; t < 32; t++ )
	     aux->H[t] =  dec[ t ];

	aux->next = result;
	result = aux;
	soluce++;
}
void  mklist( int L[32], int H[32], vector X )
{ 
	if ( X == 0 ) {
		pushit( L, H );
		return;
	}

	int x = __builtin_ctzll( X );
	xorbit( x , X );
	L[ x ] =  -1;
	H[ x ] =  +1;
		mklist( L, H, X );
	L[ x ] =  +1;
	H[ x ] =  -1;
		mklist( L, H, X );
	L[x] = H[x] = 0;
	xorbit( x , X );
}

void pair( int a, int b, int tfr[] )
{
	int x, cpt = 0;
   for( x = 0; x < 64; x++ )
	   if ( tfr[x] == a && tfr[x] == b ) 
		   cpt++;
   printf("\n%4d %4d : %d\n", a, b, cpt);
}

void initlist( boole f )
{ int tfr[ 256 ];
  int L[ 64 ]={0}, R[ 64 ]={0};
  vector X = 0;
  int x, y, t, cpt = 0;
  for( t = 0; t < ffsize; t++ )
	  tfr[ t ] = f[ t ] ? -1 : 1 ;
   Fourier( tfr, ffsize );
   for( x = 0, y = 64; x < 64; x++, y++ ) {
	   if ( tfr[x] ==0   && tfr[y] == 0 ) cpt++;
	}
   printf("\n0 0 : %d\n", cpt);
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0, count = 0;

    // initdegrees( optT, optR );


 
    while ((f = loadBoole(src))) {
		if (  job == num % mode) {
			printf("\nnum=%d", num );
			panf( stdout, f );
			initlist( f );
	}
	free( f );
	num++;
    }
    printf("\ncount=%d / %d\n", count, num );
    return 0;
}
